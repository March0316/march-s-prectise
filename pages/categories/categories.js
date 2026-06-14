const { PHOTO_COLLECTION, attachPhotoDisplayUrls } = require("../../utils/cloud");

Page({
  data: {
    loading: true,
    categories: [],
    activeCategory: "",
    activePhotos: []
  },

  onLoad() {
    this.loadCategories();
  },

  onPullDownRefresh() {
    this.loadCategories().finally(() => wx.stopPullDownRefresh());
  },

  async loadCategories() {
    this.setData({ loading: true });

    try {
      const db = wx.cloud.database();
      const { data } = await db
        .collection(PHOTO_COLLECTION)
        .orderBy("createdAt", "desc")
        .limit(100)
        .get();

      const photos = await attachPhotoDisplayUrls(data);
      const categories = this.groupByCategory(photos);
      const activeCategory = this.data.activeCategory || (categories[0] && categories[0].name) || "";

      this.setData({
        categories,
        activeCategory,
        activePhotos: this.getPhotosByCategory(categories, activeCategory),
        loading: false
      });
    } catch (error) {
      console.error("加载分类照片失败", error);
      this.setData({ loading: false });
      wx.showToast({
        title: "加载失败",
        icon: "none"
      });
    }
  },

  groupByCategory(photos) {
    const grouped = photos.reduce((result, photo) => {
      const category = photo.category || "未分类";
      if (!result[category]) {
        result[category] = [];
      }
      result[category].push(photo);
      return result;
    }, {});

    return Object.keys(grouped).map((name) => ({
      name,
      photos: grouped[name],
      count: grouped[name].length
    }));
  },

  getPhotosByCategory(categories, categoryName) {
    const category = categories.find((item) => item.name === categoryName);
    return category ? category.photos : [];
  },

  switchCategory(event) {
    const categoryName = event.currentTarget.dataset.category;
    this.setData({
      activeCategory: categoryName,
      activePhotos: this.getPhotosByCategory(this.data.categories, categoryName)
    });
  },

  previewPhoto(event) {
    const current = event.currentTarget.dataset.url;
    if (!current) {
      return;
    }

    wx.previewImage({
      current,
      urls: this.data.activePhotos.map((photo) => photo.displayUrl).filter(Boolean)
    });
  },

  openPhotographer(event) {
    const uploaderOpenId = event.currentTarget.dataset.openid;
    if (!uploaderOpenId) {
      return;
    }

    wx.navigateTo({
      url: `/pages/photographer/photographer?uploaderOpenId=${encodeURIComponent(uploaderOpenId)}`
    });
  }
});
