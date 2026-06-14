const PHOTO_COLLECTION = "photos";

const getFileExtension = (tempFilePath) => {
  const match = /\.([a-zA-Z0-9]+)(?:\?|$)/.exec(tempFilePath);
  return match ? match[1].toLowerCase() : "jpg";
};

const buildPhotoCloudPath = (tempFilePath) => {
  const extension = getFileExtension(tempFilePath);
  const timestamp = Date.now();
  const random = Math.random().toString(36).slice(2, 10);
  return `photos/original/${timestamp}-${random}.${extension}`;
};

const chooseOriginalImages = (count = 9) => {
  return wx.chooseMedia({
    count,
    mediaType: ["image"],
    sourceType: ["album", "camera"],
    sizeType: ["original"]
  });
};

const uploadOriginalPhotoFile = async (tempFilePath) => {
  const cloudPath = buildPhotoCloudPath(tempFilePath);

  return wx.cloud.uploadFile({
    cloudPath,
    filePath: tempFilePath
  });
};

const addPhotoRecord = async (photo) => {
  const db = wx.cloud.database();
  return db.collection(PHOTO_COLLECTION).add({
    data: {
      ...photo,
      createdAt: db.serverDate()
    }
  });
};

const attachPhotoDisplayUrls = async (photos) => {
  const fileIDs = photos.map((photo) => photo.fileID).filter(Boolean);
  if (!fileIDs.length) {
    return photos;
  }

  const { fileList } = await wx.cloud.getTempFileURL({ fileList: fileIDs });
  const urlMap = fileList.reduce((map, file) => {
    map[file.fileID] = file.tempFileURL;
    return map;
  }, {});

  return photos.map((photo) => ({
    ...photo,
    displayUrl: urlMap[photo.fileID] || ""
  }));
};

const uploadOriginalPhotos = async (metadata, count = 9) => {
  const result = await chooseOriginalImages(count);
  const uploads = result.tempFiles.map(async (file) => {
    const upload = await uploadOriginalPhotoFile(file.tempFilePath);
    await addPhotoRecord({
      ...metadata,
      fileID: upload.fileID
    });
    return upload.fileID;
  });

  return Promise.all(uploads);
};

module.exports = {
  PHOTO_COLLECTION,
  addPhotoRecord,
  attachPhotoDisplayUrls,
  chooseOriginalImages,
  uploadOriginalPhotoFile,
  uploadOriginalPhotos
};
