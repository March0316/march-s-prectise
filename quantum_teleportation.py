"""Quantum teleportation protocol implemented with pyqpanda3.

This script builds and verifies the standard single-qubit teleportation
protocol.  It only uses pyqpanda3, numpy (installed as a pyqpanda3 dependency)
and Python standard-library modules.
"""

from __future__ import annotations

import cmath
import math
from typing import Iterable

from pyqpanda3.core import CNOT, CPUQVM, H, QCircuit, QProg, RY, X, Z, measure, qif


UNKNOWN_STATE_THETA = math.pi / 3.0
ALPHA = math.cos(math.pi / 6.0)
BETA = math.sin(math.pi / 6.0)
TARGET_STATE = (complex(ALPHA, 0.0), complex(BETA, 0.0))


def _qubit_index(qubit: int) -> int:
    """Return a pyqpanda3 integer qubit address from an int-like qubit object."""
    if isinstance(qubit, int):
        return qubit
    if hasattr(qubit, "get_qubit_addr"):
        return int(qubit.get_qubit_addr())
    raise TypeError(f"Unsupported qubit reference: {qubit!r}")


def prepare_bell_state(q1: int, q2: int) -> QCircuit:
    """Prepare |psi+> = (|00> + |11>) / sqrt(2) on qubits q1 and q2."""
    q1 = _qubit_index(q1)
    q2 = _qubit_index(q2)

    circuit = QCircuit()
    circuit << H(q1) << CNOT(q1, q2)
    return circuit


def teleportation_circuit(q0: int, q1: int, q2: int) -> QProg:
    """Build the complete teleportation circuit.

    q0 starts as the unknown state
        |psi> = cos(pi/6)|0> + sin(pi/6)|1>,
    q1 is Alice's entangled qubit, and q2 is Bob's entangled qubit.
    Alice's measurement results are written to classical bits c0 and c1.
    Bob applies X when c1 = 1 and Z when c0 = 1.
    """
    q0 = _qubit_index(q0)
    q1 = _qubit_index(q1)
    q2 = _qubit_index(q2)

    prog = QProg()
    prog << RY(q0, UNKNOWN_STATE_THETA)
    prog << prepare_bell_state(q1, q2)
    prog << CNOT(q0, q1) << H(q0)
    prog << measure(q0, 0) << measure(q1, 1)

    x_correction = QProg()
    x_correction << X(q2)
    z_correction = QProg()
    z_correction << Z(q2)
    prog << qif([1]).then(x_correction).qendif()
    prog << qif([0]).then(z_correction).qendif()
    return prog


def _pre_measurement_circuit(q0: int, q1: int, q2: int) -> QProg:
    """Circuit immediately before Alice measures q0 and q1."""
    prog = QProg()
    prog << RY(q0, UNKNOWN_STATE_THETA)
    prog << prepare_bell_state(q1, q2)
    prog << CNOT(q0, q1) << H(q0)
    return prog


def _run_state_vector(prog: QProg, shots: int = 1) -> list[complex]:
    qvm = CPUQVM()
    qvm.run(prog, shots)
    return list(qvm.result().get_state_vector())


def _normalize(state: Iterable[complex]) -> tuple[complex, ...]:
    values = tuple(complex(v) for v in state)
    norm = math.sqrt(sum(abs(v) ** 2 for v in values))
    if norm == 0.0:
        raise ValueError("Cannot normalize a zero vector.")
    return tuple(v / norm for v in values)


def _remove_global_phase(state: Iterable[complex]) -> tuple[complex, ...]:
    normalized = _normalize(state)
    pivot = next((v for v in normalized if abs(v) > 1e-12), 0j)
    if abs(pivot) <= 1e-12:
        return normalized
    phase = cmath.exp(-1j * cmath.phase(pivot))
    cleaned = tuple(v * phase for v in normalized)
    return tuple(0j if abs(v) < 1e-12 else v for v in cleaned)


def fidelity(reference: Iterable[complex], recovered: Iterable[complex]) -> float:
    """Pure-state fidelity |<reference|recovered>|^2."""
    ref = _normalize(reference)
    rec = _normalize(recovered)
    inner = sum(a.conjugate() * b for a, b in zip(ref, rec))
    return float(abs(inner) ** 2)


def extract_qubit_state(state_vector: list[complex], target_qubit: int) -> tuple[complex, complex]:
    """Extract a pure target-qubit state from a product final state vector.

    pyqpanda3 indexes basis states in little-endian order, so qubit k is read
    from bit k of the state-vector index.
    """
    target_qubit = _qubit_index(target_qubit)
    mask = 1 << target_qubit
    best_index = None
    best_weight = -1.0

    for index, amp in enumerate(state_vector):
        if index & mask:
            continue
        partner = index | mask
        weight = abs(amp) ** 2 + abs(state_vector[partner]) ** 2
        if weight > best_weight:
            best_index = index
            best_weight = weight

    if best_index is None or best_weight <= 1e-18:
        raise ValueError("Target qubit has no non-zero amplitude pair.")

    return _remove_global_phase((state_vector[best_index], state_vector[best_index | mask]))


def analyze_measurement_branches(
    state_vector: list[complex], q0: int, q1: int, q2: int
) -> list[dict[str, object]]:
    """Analyze all four Alice measurement branches before Bob's correction."""
    rows = []
    for c0 in (0, 1):
        for c1 in (0, 1):
            bob = [0j, 0j]
            probability = 0.0
            for index, amp in enumerate(state_vector):
                if ((index >> q0) & 1) == c0 and ((index >> q1) & 1) == c1:
                    bob_bit = (index >> q2) & 1
                    bob[bob_bit] += amp
                    probability += abs(amp) ** 2

            before = _normalize(bob)
            after = list(before)
            if c1 == 1:
                after = [after[1], after[0]]
            if c0 == 1:
                after = [after[0], -after[1]]
            after = _remove_global_phase(after)

            rows.append(
                {
                    "alice_bits": f"{c0}{c1}",
                    "probability": probability,
                    "bob_before": before,
                    "bob_after": after,
                    "fidelity": fidelity(TARGET_STATE, after),
                }
            )
    return rows


def _format_complex(value: complex) -> str:
    value = 0j if abs(value) < 1e-12 else value
    real = 0.0 if abs(value.real) < 1e-12 else value.real
    imag = 0.0 if abs(value.imag) < 1e-12 else value.imag
    if imag == 0.0:
        return f"{real:.12f}"
    if real == 0.0:
        return f"{imag:.12f}i"
    sign = "+" if imag >= 0 else "-"
    return f"{real:.12f}{sign}{abs(imag):.12f}i"


def main() -> None:
    q0, q1, q2 = 0, 1, 2

    print("=== Quantum Teleportation with pyqpanda3 ===")
    print(f"Initial state: {ALPHA:.12f}|0> + {BETA:.12f}|1>")
    print()

    bell = prepare_bell_state(q1, q2)
    print("Bell-state circuit:")
    print(bell.draw())
    print()

    prog = teleportation_circuit(q0, q1, q2)
    print("Complete teleportation circuit:")
    print(prog.draw())

    qvm = CPUQVM()
    qvm.run(prog, 1)
    result = qvm.result()
    final_state_vector = list(result.get_state_vector())
    bob_state = extract_qubit_state(final_state_vector, q2)
    final_fidelity = fidelity(TARGET_STATE, bob_state)
    print("Single-shot Alice measurement counts:", result.get_counts())
    print(
        "Recovered Bob state amplitudes:",
        f"{_format_complex(bob_state[0])}|0> + {_format_complex(bob_state[1])}|1>",
    )
    print(
        "Recovered Bob probabilities:",
        f"P(0)={abs(bob_state[0]) ** 2:.12f}, P(1)={abs(bob_state[1]) ** 2:.12f}",
    )
    print(f"Single-shot fidelity: {final_fidelity:.12f}")
    print()

    pre_state_vector = _run_state_vector(_pre_measurement_circuit(q0, q1, q2))
    branch_rows = analyze_measurement_branches(pre_state_vector, q0, q1, q2)
    print("Branch-by-branch fidelity analysis:")
    print("Alice bits | Probability | Bob state after correction | Fidelity")
    for row in branch_rows:
        state = row["bob_after"]
        print(
            f"{row['alice_bits']:>10} | "
            f"{row['probability']:.12f} | "
            f"{_format_complex(state[0])}|0> + {_format_complex(state[1])}|1> | "
            f"{row['fidelity']:.12f}"
        )

    print()
    print(
        "Conclusion: every Alice measurement branch occurs with probability 1/4, "
        "and Bob's corrected qubit matches the original state with fidelity 1."
    )


if __name__ == "__main__":
    main()
