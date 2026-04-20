# 参考文档
官网文档：
[Upgrading to VisualSVN Server 5.4 \| VisualSVN Help Center](https://www.visualsvn.com/support/topic/00233/)
# 说明
## 1.版本选择：
- 推荐最新的5.4.6版本，更好用也更稳定
- 但是当前使用版本为2.5.6，比较古老跨服过大可能出现一些问题
## 2.推荐更新方法
 - 直接升级（官方推荐）
## 3.注意事项
### 升级前检查清单：
1. **确认服务器上安装的操作系统仍然受支持**：从VisualSVN Server 5.0开始，最低支持的操作系统为Windows Server 2012 R2 和 Windows 8.1。如果当前操作系统不再受支持， 则需要升级操作系统或 将 VisualSVN Server 实例迁移到另一台计算机。
2. **不要卸载当前的VisualSVN服务器实例**：安装新版本之前，请勿**卸载**现有的 VisualSVN Server 实例。VisualSVN Server 的设计初衷是始终覆盖安装现有版本。卸载 VisualSVN Server 实例可能会导致重要的服务器设置丢失。
3. **确保拥有计算机的管理员权限**：此步骤适用于所有 VisualSVN Server 版本的升级。要成功升级，应该使用具有管理员权限的帐户运行 VisualSVN Server 安装包。
4. **从 httpd-custom.conf 中移除与 SSL 3.0 协议版本相关的自定义设置**：从 VisualSVN Server 5.0 版本开始，已移除对已弃用的 SSL 3.0 协议的支持。但在 VisualSVN Server 5.3 版本之前，可以通过自定义**httpd-custom.conf**文件强制重新启用此功能。从 5.3 版本开始，**已无法再启用 SSL 3.0**，尝试启用会导致 HTTP 服务启动失败。
   升级到 VisualSVN Server 5.3 之前，必须删除 **httpd-custom.conf**文件 中以下与 SSL 3.0 相关的自定义设置 ：
   SSLProtocol——**通常情况下，必须完全移除此指令。如果无法移除此指令，则必须移除其`SSLv3`值，以防止尝试启用 SSL 3.0。
5. **确认未使用 Internet Explorer 访问 VisualSVN 服务器 Web 界面**：从 5.0 版本开始，VisualSVN 服务器 Web 界面 **不再支持 Internet Explorer**。如果您使用 Internet Explorer 访问 Web 界面，则需要升级到 Microsoft Edge 或切换到其他受支持的浏览器。