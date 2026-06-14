const { PHOTO_COLLECTION, attachPhotoDisplayUrls } = require("../../utils/cloud");

const DEFAULT_CENTER = {
  latitude: 31.2304,
  longitude: 121.4737
};

Page({
  data: {
    latitude: DEFAULT_CENTER.latitude,
    longitude: DEFAULT_CENTER.longitude,
    loading: true,
    markers: [],
    photos: [],
    selectedPhoto: null
  },

  onLoad() {
    this.initLocation();
    this.loadPhotos();
  },

  onPullDownRefresh() {
    this.loadPhotos().finally(() => wx.stopPullDownRefresh());
  },

  initLocation() {
    wx.getLocation({
      type: "gcj02",
      success: ({ latitude, longitude }) => {
        this.setData({ latitude, longitude });
      }
    });
  },

  async loadPhotos() {
    this.setData({ loading: true });

    try {
      const db = wx.cloud.database();
      const { data } = await db
        .collection(PHOTO_COLLECTION)
        .orderBy("createdAt", "desc")
        .limit(100)
        .get();

      const photosWithUrls = await attachPhotoDisplayUrls(data);
      const photos = photosWithUrls.map((photo, index) => ({
        ...photo,
        markerId: index + 1
      }));
      this.setData({
        photos,
        markers: this.buildMarkers(photos),
        loading: false
      });
    } catch (error) {
      console.error("加载地图照片失败", error);
      this.setData({ loading: false });
      wx.showToast({
        title: "加载失败",
        icon: "none"
      });
    }
  },

  buildMarkers(photos) {
    return photos
      .filter(
        (photo) =>
          photo.location &&
          typeof photo.location.latitude === "number" &&
          typeof photo.location.longitude === "number" &&
          photo.displayUrl
      )
      .map((photo) => ({
        id: photo.markerId,
        latitude: photo.location.latitude,
        longitude: photo.location.longitude,
        width: 48,
        height: 48,
        iconPath: photo.displayUrl,
        callout: {
          content: photo.description || "MapAlbum 照片",
          color: "#1f2329",
          fontSize: 12,
          borderRadius: 8,
          bgColor: "#ffffff",
          padding: 8,
          display: "BYCLICK"
        }
      }));
  },

  handleMarkerTap(event) {
    const markerId = event.detail.markerId;
    const selectedPhoto = this.data.photos.find((photo) => photo.markerId === markerId);
    this.setData({ selectedPhoto });
  },

  closePhotoCard() {
    this.setData({ selectedPhoto: null });
  },

  previewSelectedPhoto() {
    const { selectedPhoto, photos } = this.data;
    if (!selectedPhoto || !selectedPhoto.displayUrl) {
      return;
    }

    wx.previewImage({
      current: selectedPhoto.displayUrl,
      urls: photos.map((photo) => photo.displayUrl).filter(Boolean)
    });
  },

  openPhotographer() {
    const { selectedPhoto } = this.data;
    if (!selectedPhoto || !selectedPhoto.uploaderOpenId) {
      return;
    }

    wx.navigateTo({
      url: `/pages/photographer/photographer?uploaderOpenId=${encodeURIComponent(
        selectedPhoto.uploaderOpenId
      )}`
    });
  }
});
