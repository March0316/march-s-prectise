/*#include<stdio.h>
#include<math.h>
int main(){
	double a,b,c,delta,m,n,x1,x2;
	printf("请输入a，b，c的值！");
	scanf("%lf %lf %lf",&a,&b,&c);
	delta=b*b-4*a*c;
	if(delta>0){
		m=-b/(2*a);
		n=sqrt(delta)/(2*a);
		x1=m+n;
		x2=m-n;
		printf("x1=%.2lf\nx2=%.2lf\n",x1,x2);
	}
	return 0;
}*/


/*#include<stdio.h>
int main(){
	int x,a,b,c,sum=0;
	printf("请输入一个整数：\n");
	scanf("%d",&x);
	a=x/100;
	b=x%100/10;
	c=x%10;
	sum=a+b+c;
	printf("%d\n",sum);
	return 0;
}*/


/*#include<stdio.h>
int main(){
	int a,b;
	int max=a;
	printf("请输入两个整数a，b的值：\n");
	scanf("%d%d",&a,&b);
	if(b>max){
		max=b;
		printf("%d",max);
	}
	else if(b<max){
		printf("%d",max);
	}
	return 0;
}*/


/*#include<stdio.h>
int main(){
	int a;
	printf("请输入一个整数：");
	scanf("%d",&a);
	if(a>0){
		printf("%d",a);
	}
	else if(a<0){
		a=-a;
		printf("%d",a);
	}
	return 0;
}*/


/*#include<stdio.h>
int main(){
	int a;
	printf("请输入一个整数：\n");
	scanf("%d",&a);
	if(a%7==0){
		if(a%11==0){
			printf("a既为7的倍数也为11的倍数"); 
		}
		else if(a%11!=0){
			printf("a只为7的倍数不为11的倍数");
		} 
	}
	else if(a%11==0){
		if(a%7!=0){
			printf("a只为11的倍数不为7的倍数");
		}
		else if(a%7==0){
			printf("a既为7的倍数也为11的倍数");
		}
	}
	else if(a%7!=0&&a%11!=0){
		printf("a既不为7的倍数又不为11的倍数");
	}
	return 0;
}*/


/*#include<stdio.h>
int main(){
	int a;
	printf("请输入一个整数：\n");
	scanf("%d",&a);
	if(a>=5&&a<=15){
		a=a+17;
		printf("a的值为%d，在区间[22,32]之间\n",a);
	}
	else if(a>=51&&a<=58){
		a=a+10;
		printf("a的值为%d，在区间[61,68]之间\n",a);
	}
	else{
		printf("输入的整数不在要求的区间内！");
	}
	return 0;
}*/


/*#include<stdio.h>
int main(){
	int i=1,sum;
	while(i<=5){
		sum=i*i;
		printf("%d\n",sum);
		i++;
	}
	return 0;
}*/


/*#include<stdio.h>
int main(){
	int i,sum=0,num=0;
	while(1){
		printf("请输入整数：\n");
		scanf("%d",&i);
		if(i==0){
			break;
		}
		else if(i%2==0){
		sum+=i;
		num++;
		}
		
	}
	float eva=(float)sum/num;
	printf("偶数的和%d\n",sum);
	printf("偶数的平均值%.2lf\n",eva);
	printf("偶数的个数%d\n",num);
	return 0;上机练习：输入整数直到输入0为止，输出偶数的个数以及所有输入偶数的平均值。 
} */


/*#include<stdio.h>
int main(){
	int n,score[50];
	int a,sum1=0,sum2=0;
	scanf("%d\n",&n);
	for(a=0;a<n;a++){
		scanf("%d",&score[a]);
		if(a>=60){
			sum1++;
		}
		if(a>=90){
			sum2++;
		}
	}
	printf("通过考试的人数为%d\n",sum1);
	printf("优秀学生人数为%d\n",sum2);
	return 0;
}*/


/*#include<stdio.h>
int main(){
	int i,p,j;
	int a[10];
	for(i=0;i<10;i++){
		scanf("%d",&a[i]);}
	printf("\n");
	for(j=0;j<9;j++){
		for(i=0;i<9-j;i++){
		if(a[i]>a[i+1]){
			p=a[i];
			a[i]=a[i+1];
			a[i+1]=p;	
			}
		}
	}
	for(i=0;i<10;i++){
		printf("%d ",a[i]);
	}
	return 0;
}还要好好练习才行啊，数组太烂了*/


/*#include<stdio.h>
int main(){
	int i,j,t;
	int a[10];
	for(i=0;i<10;i++){
		scanf("%d",&a[i]);
	}
	printf("\n");
	for(j=0;j<9;j++){
		int min=j;
		for(i=j+1;i<10;i++){选择法排序的核心在于从当前位置开始找最小值，然后把这个最小值交换到该次循环的的最小下标处
			if(a[i]<a[min]){
				min=i;
				t=a[j];
				a[j]=a[min];
				a[min]=t;
				
			}
		}
	}
	for(i=0;i<10;i++){
		printf("%d ",a[i]);
	}
	return 0;
}*/


/*#include<stdio.h>
int main(){
	int j,i,a[11][11];
	for(i=1;i<11;i++){
		a[i][1]=1;
		a[i][i]=1;
	}
	for(i=3;i<11;i++){
		for(j=2;j<=i-1;j++){
			a[i][1]=1;
			a[i][j]=a[i-1][j-1]+a[i-1][j];
			a[i][i]=1;
		}
	}
	for(i=1;i<11;i++){
		for(j=1;j<=i;j++){
			printf("%6d",a[i][j]);}
			printf("\n");
	}
	return 0;
	
}杨辉三角练习题：是二维数组，从哪里开始计数很关键，下去再练习静态二维数组！*/
 
 
 /*#include<stdio.h>
 int main(){
 	char a[15],b[5],c[5];
 	scanf("%s%s%s",a,b,c);
 	printf("a=%s\nb=%s\nc=%s\n",a,b,c);
 	scanf("%s",a);
 	printf("a=%s\n",a);
 }*/
 
 
 /*#include<stdio.h>
 #include<string.h>
 int main(){
 	char str1[]="Hello！",
 	str2[]="How are you?",str[20];
 	int len1,len2,len3;
 	len1=strlen(str1);
 	len2=strlen(str2);
 	if(strcmp(str1,str2)>0){
 		strcpy(str1,str2);
 		strcat(str,str1);
	 }
	 else if(strcmp(str1,str2)<0){
	 	strcpy(str,str2);
	 	strcat(str,str1);
	 }
	 else{
	 	strcpy(str,str1);
	 }
	 len3=strlen(str);
	 puts(str);
	 printf("Len1=%d,Len2=%d,Len3=%d\n",len1,len2,len3);
	 return 0;
 } 这个不是自己写的，纯抄。*/
 
 
 /*#include<stdio.h>
 #include<string.h>
 int main(){
 	char str[5][20],min[20];
 	int i;
 	for(i=0;i<5;i++){
 		gets(str[i]);}
 	strcpy(min,str[0]);
 	for(i=0;i<5;i++){
 		if(strcmp(str[i],min)<0){
 			strcpy(min,str[i]);
		 }
	 }
	 printf("%s",min);
	 return 0;
 }跟ppt一模一样还是输不出来我服了*/
 
 
 /*#include<stdio.h>
 #include<string.h>
 int main(){
 	char str1[10]="iamarch",str2[10]="himyname！";
 	int i;
 	for(i=0;i<10;i++){
 		gets(str2[10]);
 		gets(str1[10]);
 		if(str1[i]!='\0'){
		 i++;
		 }
		for(int j=0;j<10;j++){
		if(str2[j]!='\0'){
			str1[i++]=str2[j++];
		}
		if(str[i]='\0'){
			printf("%s",str1);
		} }
		return 0;
	}*/
	
	
/*#include<stdio.h>
#include<string.h>
int main(){
	char s1[100],s2[100];
	printf("请输入字符串1:\n");
	gets(s1);
	printf("请输入字符串2：\n");
	gets(s2);
	int i,resu;
		while(s1[i]==s2[i]&&s1[i]!='\0'){
			i++;
		if(s1[i]=='\0'&&s2[i]=='\0'){
			resu=0;
		}
		else{
			resu=s1[i]-s2[i];
			}
	}
	printf("%s和%s比较结果为%d\n",s1,s2,resu);
	return 0;
}*/


/*#include<stdio.h>
int main(){
	int a[5]={2,3,8,6,1};
	int i,j,count=0;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			if(i<j&&a[i]>a[j]){
				count++;
			}
		}
	}
	printf("%d\n",count);
	return 0; 
}*/


/*#include<stdio.h>
int main(){
	int a[10]={100,100,56,88,78,99,75,56,67,87};
	int i,count1=0,count2=0,count3=0,count4=0,count5=0,count6=0,count7=0;
	for(i=0;i<10;i++){
		if(a[i]==100){
			count1++;
		}
		else if(a[i]<100&&a[i]>89){
			count2++;
		}
		else if(a[i]<90&&a[i]>79){
			count3++;
		}
		else if(a[i]<80&&a[i]>69){
			count4++;
		}
		else if(a[i]<70&&a[i]>59){
			count5++;
		}
		else if(a[i]<60&&a[i]>49){
			count6++;
		}
		else if(a[i]<50&&a[i]>39){
			count7++;
		}
	}
	printf("100分的人数%d\n",count1);
	printf("90-100分的人数%d\n",count2);
	printf("80-90分的人数%d\n",count3);
	printf("70-80分的人数%d\n",count4);
	printf("60-70分的人数%d\n",count5);
	printf("50-60分的人数%d\n",count6);
}*/


/*#include<stdio.h>
#include<string.h>
struct IDcheck {
	char name[20];
	int ID[100];
	double score[5];
};
int read(struct IDcheck a[]){
	int i=0;
	while(1){
		scanf("%s",a[i].name);
		if(strcmp(a[i].name,"#")){
			break;
		}
		scanf("%s",a[i].ID);
		scanf("%lf",&a[i].score);
		i++;
	}
	return i;
}
int main(){
	printf("请输入学生ID：\n");
	
}*/


/*#include<stdio.h>
void yue(int n,int m){
	int a[n];
	int i,count=0,k=0;
	for(i=0;i<n;i++){
		a[i]=1;
	}
	i=0;
	while(count<n){
		if(a[i]){
			k++;
			if(k==m){
				a[i]=0;
				k=0;
				count++;
				printf("%d ",i+1);
				continue;
			}
		}
		i=(i+1)%n;
	}
} 
int main(){
	int n,m;
	scanf("%d %d",&n,&m);
	yue(n,m);
	return 0;
}约瑟夫问题*/

#include<stdio.h>
int main(){
	int a[10];
	printf("please input 10 numbers:\n");
	int i;
	for(i=0;i<10;i++){
		scanf("%d",&a[i]);
	}
	printf("\n");
	int p,q,t;
	for(p=0;p<9;p++){
		for(q=0;q<9-i;q++){
			if(a[q]>a[q+1]){
			t=a[q];
			a[q]=a[q+1];
			a[q+1]=t;	
			}
		}
	}
	for(i=0;i<10;i++){
		printf("%d ",a[i]);
	}
	return 0;
} 

