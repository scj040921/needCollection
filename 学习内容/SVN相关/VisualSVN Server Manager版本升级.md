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
5. **确认未使用 Internet Explorer 访问 VisualSVN 服务器 Web 界面**：从 5.0 版本开始，VisualSVN 服务器 Web 界面 **不再支持 Internet Explorer**。如果您使用 Internet Explorer 访问 Web 界面，则需要升级到 Microsoft Edge 或切换到其他受支持的浏览器。微软鼓励用户迁移到具有 Internet Explorer (IE) 模式的 Microsoft Edge 浏览器。
   *此更改仅影响客户端存储库 Web 界面，不影响任何服务器管理功能或 VisualSVN 服务器管理器控制台。*
6. **检查 TLS/SSL 兼容级别设置（不再支持 SSL 3.0）**：VisualSVN Server 5.0 将不再支持 SSL 3.0 协议（即使启用了旧版 TLS/SSL 兼容级别）。对于几乎所有与服务器交互的现代客户端而言，这不会造成任何负面影响。但是，不支持 SSL 3.0 以上协议的旧版客户端在服务器升级后会收到访问错误。如果您有此类客户端（例如基于 Java 5 或更早版本的客户端），则需要事先将其升级。
7. **从 httpd-custom.conf 中移除与 MaxMemFree 指令相关的自定义设置**：此步骤适用于从 VisualSVN Server 4.3 及更早版本升级，这些版本在 %VISUALSVN_SERVER%conf\ **httpd-custom.conf**文件中进行了自定义。
   VisualSVN Server 5.0 切换到了一种新的内存分配方案，该方案不再包含`MaxMemFree`配置指令。升级到 VisualSVN Server 5.0 之前，您应该检查**httpd-custom.conf**文件并删除以下指令： **最大内存自由**。
8. **如果需要自定义权限才能访问存储库，请跳过在安装程序中启用搜索索引的步骤**：如果需要自定义权限才能访问服务器上的存储库（这种情况通常发生在存储库 [存储在网络共享上](https://www.visualsvn.com/support/topic/00022/ "知识库22：将存储库存储在网络共享上")且权限已授予[专用帐户](https://www.visualsvn.com/support/topic/00024/ "KB24：配置 VisualSVN HTTP 服务以在专用用户帐户下运行")时），请跳过在安装程序中启用搜索索引的步骤。
9. **从 httpd-custom.conf 中移除与 mod_mpm_winnt 相关的自定义设置**：此步骤适用于从 VisualSVN Server 4.1 或更早版本升级，并且 %VISUALSVN_SERVER%conf\ **httpd-custom.conf**文件中也有自定义设置的情况。
   VisualSVN Server 4.2 使用的是**mod_mpm_visualsvn**模块而不是 **mod_mpm_winnt**模块，因此某些自定义配置会导致配置错误，VisualSVN HTTP 服务将无法启动。您应该仔细检查并移除这些自定义配置。
   **升级前，请考虑从httpd-custom.conf**文件 中删除以下不受支持的指令：
     - **每个子节点的线程数**
     - **线程限制**
10. **检查您是否需要更新 VisualSVN 服务器许可证**：此步骤适用于从 VisualSVN Server 3.9 及更早版本升级。
    从 4.0 版本开始，VisualSVN Server 可以在三种类型的许可证下运行：社区版、基础版或企业版。
    如果您使用的是 VisualSVN Server 企业版，并且拥有未过期的维护订阅许可证，则升级到新的许可模式是完全透明的。
    如果您使用的是**标准版**，升级到 4.1 或更高版本将自动激活功能几乎相同的社区版许可证。但是，在以下两种情况下，升级期间社区版许可证将不可用：
    - 当**Subversion 用户帐户超过 15 个**时，社区版许可证最多允许使用 15 个 Subversion 用户帐户。
    - 当您使用**Windows（基本）身份验证**时。从 4.0 版本开始，Windows 身份验证仅适用于企业版许可证。
    对于上述例外情况，您可以选择提供足够的许可证，或者生成一个免费的评估许可证，允许您在 45 天内使用 VisualSVN Server 的所有功能。如果选择后者，您需要在评估期结束前提供足够的 Essential 或 Enterprise 许可证。
11. **确认未使用 Internet Explorer 10 访问 VisualSVN 服务器 Web 界面**：从 VisualSVN Server 4.0 开始，最低支持的 IE 版本为 **Internet Explorer 11**如果您仍在使用 Internet Explorer 10，则需要升级您的浏览器。
12. **从 httpd-custom.conf 中移除与 mod_authz_svn 相关的自定义设置**：