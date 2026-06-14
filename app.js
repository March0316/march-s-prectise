App({
  onLaunch() {
    if (!wx.cloud) {
      console.error("请使用 2.2.3 或以上的基础库以使用云能力");
      return;
    }

    wx.cloud.init({
      // TODO: 替换为你的微信云开发环境 ID。
      env: "your-cloud-env-id",
      traceUser: true
    });
  },

  globalData: {
    photoCollection: "photos"
  }
});
