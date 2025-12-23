## 📋 更新日志

[点击此处](https://github.com/chenxin527/uboot-ipq60xx-nand-build/blob/main/changelog.md) 查看完整更新日志。

### ✨ 功能更新

- 添加对 Qihoo 360V6、CMIOT AX18、ZN M2 的支持
- 添加 bootconfig 命令，用于切换启动分区
- 添加 untar 命令，用于解析 sysupgrade tar image
- 调整相关代码，适配 NAND 机型

### 📢 其他更新

- 调整文件上传完成后内存填充的起始地址，改用 0 填充内存

## 📡 支持设备

- CMIOT AX18
- Qihoo 360V6
- ZN M2

## 📸 网页截图

[点击此处](https://github.com/chenxin527/uboot-ipq60xx-nand-build/blob/d875ac5a73d6271862721000dc3658f837c4cc46/screenshots.md) 查看所有网页截图。

![uboot-index-page](https://github.com/chenxin527/uboot-ipq60xx-nand-build/blob/4957dd6b99e1388f303ed02af72f20510658c603/screenshots/uboot-index-page.png)

> [!NOTE]
>
> 360V6 靠近 USB 的第二个网口（紧挨着 WAN 的 LAN 口）无法进入 U-Boot Web 界面，请使用其他网口。
