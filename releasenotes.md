## 📋 更新日志

[点击此处](https://github.com/chenxin527/uboot-ipq60xx-emmc-build/blob/main/changelog.md) 查看完整更新日志。

### ✨ 功能更新

- 雅典娜支持通过 Screen 键启动 HTTP Server

### 🐛 BUG 修复

- 修复无法启动超过 30MB 的 uImage 的问题

### 📢 其他更新

- 修改 bootcmd 为 bootipq（[点击此处](https://github.com/chenxin527/uboot-ipq60xx-emmc-build/commit/b1682e34c931779f1f46ff4d210777e9b1256f1a) 查看详细说明）
- 修改 smeminfo 命令的输出内容

## 📡 支持设备

- 京东云太乙（RE-CS-07）
- 京东云亚瑟（RE-SS-01）
- 京东云雅典娜（RE-CS-02）
- 连我 NN6000 V1（未测试）
- 连我 NN6000 V2
- 红米 AX5 JDCloud（RA50）

## 📸 网页截图

[点击此处](https://github.com/chenxin527/uboot-ipq60xx-emmc-build/blob/5bda947116b50186274a11367528f257d4f52c56/screenshots.md) 查看所有网页截图。

![uboot-index-page](https://github.com/chenxin527/uboot-ipq60xx-emmc-build/blob/42af5d1c143ea42dd02023db5bef45f56677674f/screenshots/uboot-index-page.png)

> [!NOTE]
>
> 连我 NN6000 V1 的 U-Boot 未测试过，因为没有机器。V1 和 V2 的 U-Boot 只是网口配置不同，其他都一样。若发现 V1 U-Boot 不能正常使用，可刷写 V2 的 U-Boot 测试，看看每个网口是否能正常进 Web。每换一个网口都要断电并重新按 RESET / WPS 键启动 HTTP Server，不要在 HTTP Server 已经启动的时候换网口，这样是进不了 Web 的。
