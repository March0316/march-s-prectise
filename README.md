# MapAlbum

MapAlbum 是一个基于微信云开发的微信小程序基础骨架，用于按拍摄位置和内容分类浏览照片。

## 页面

- `pages/map/map`：地图页，在地图上展示带拍摄位置的照片，点击标记后查看大图、上传者和描述。
- `pages/categories/categories`：分类页，按上传者选择的 `category` 字段浏览照片。
- `pages/photographer/photographer`：拍摄者主页，查看某位用户上传的全部照片。

## 云开发配置

1. 在微信开发者工具中开通云开发。
2. 将 `app.js` 中的 `your-cloud-env-id` 替换为实际云环境 ID。
3. 创建云数据库集合 `photos`。
4. 照片原图保存到云存储，推荐路径前缀为 `photos/original/`。

## 照片数据结构

`photos` 集合建议字段：

```js
{
  fileID: "cloud://xxx/photos/original/photo.jpg",
  uploaderOpenId: "user-openid",
  uploaderName: "拍摄者昵称",
  uploaderAvatar: "头像 URL",
  description: "照片描述",
  category: "风景",
  location: {
    latitude: 31.2304,
    longitude: 121.4737,
    name: "拍摄地点"
  },
  createdAt: Date
}
```

## 原画质上传

`utils/cloud.js` 中的 `chooseOriginalImages` 使用：

```js
wx.chooseMedia({
  mediaType: ["image"],
  sourceType: ["album", "camera"],
  sizeType: ["original"]
})
```

随后通过 `wx.cloud.uploadFile` 直接上传临时文件，不做压缩、裁剪或转码。
