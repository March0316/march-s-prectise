const { PHOTO_COLLECTION, attachPhotoDisplayUrls } = require("../../utils/cloud");

Page({
  data: {
    loading: true,
    activeUploaderOpenId: "",
    photographers: [],
    currentPhotographer: null,
    photos: []
  },

  onLoad(options) {
    this.setData({
      activeUploaderOpenId: options.uploaderOpenId ? decodeURIComponent(options.uploaderOpenId) : ""
    });
    this.loadPhotographers();
  },

  onPullDownRefresh() {
    this.loadPhotographers().finally(() => wx.stopPullDownRefresh());
  },

  async loadPhotographers() {
    this.setData({ loading: true });

    try {
      const db = wx.cloud.database();
      const { data } = await db
        .collection(PHOTO_COLLECTION)
        .orderBy("createdAt", "desc")
        .limit(100)
        .get();

      const photos = await attachPhotoDisplayUrls(data);
      const photographers = this.groupByPhotographer(photos);
      const activeUploaderOpenId =
        this.data.activeUploaderOpenId ||
        (photographers[0] && photographers[0].uploaderOpenId) ||
        "";

      this.setActivePhotographer(photographers, activeUploaderOpenId);
      this.setData({
        photographers,
        loading: false
      });
    } catch (error) {
      console.error("加载拍摄者照片失败", error);
      this.setData({ loading: false });
      wx.showToast({
        title: "加载失败",
        icon: "none"
      });
    }
  },

  groupByPhotographer(photos) {
    const grouped = photos.reduce((result, photo) => {
      const uploaderOpenId = photo.uploaderOpenId || photo._openid || "anonymous";
      if (!result[uploaderOpenId]) {
        result[uploaderOpenId] = {
          uploaderOpenId,
          name: photo.uploaderName || "匿名拍摄者",
          avatar: photo.uploaderAvatar || "",
          photos: []
        };
      }
      result[uploaderOpenId].photos.push(photo);
      return result;
    }, {});

    return Object.keys(grouped).map((uploaderOpenId) => ({
      ...grouped[uploaderOpenId],
      count: grouped[uploaderOpenId].photos.length
    }));
  },

  setActivePhotographer(photographers, uploaderOpenId) {
    const currentPhotographer = photographers.find(
      (photographer) => photographer.uploaderOpenId === uploaderOpenId
    );

    this.setData({
      activeUploaderOpenId: uploaderOpenId,
      currentPhotographer: currentPhotographer || null,
      photos: currentPhotographer ? currentPhotographer.photos : []
    });
  },

  switchPhotographer(event) {
    const uploaderOpenId = event.currentTarget.dataset.openid;
    this.setActivePhotographer(this.data.photographers, uploaderOpenId);
  },

  previewPhoto(event) {
    const current = event.currentTarget.dataset.url;
    if (!current) {
      return;
    }

    wx.previewImage({
      current,
      urls: this.data.photos.map((photo) => photo.displayUrl).filter(Boolean)
    });
  }
});
