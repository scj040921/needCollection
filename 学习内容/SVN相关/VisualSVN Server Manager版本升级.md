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
1. **确认服务器上安装的操作系统仍然受支持**：
    从VisualSVN Server 5.0开始，最低支持的操作系统为Windows Server 2012 R2 和 Windows 8.1。如果当前操作系统不再受支持， 则需要升级操作系统或 将 VisualSVN Server 实例迁移到另一台计算机。
2. **不要卸载当前的VisualSVN服务器实例**：
    安装新版本之前，请勿**卸载**现有的 VisualSVN Server 实例。VisualSVN Server 的设计初衷是始终覆盖安装现有版本。卸载 VisualSVN Server 实例可能会导致重要的服务器设置丢失。
3. **确保拥有计算机的管理员权限**：
    此步骤适用于所有 VisualSVN Server 版本的升级。要成功升级，应该使用具有管理员权限的帐户运行 VisualSVN Server 安装包。
4. **从 httpd-custom.conf 中移除与 SSL 3.0 协议版本相关的自定义设置**：
    从 VisualSVN Server 5.0 版本开始，已移除对已弃用的 SSL 3.0 协议的支持。但在 VisualSVN Server 5.3 版本之前，可以通过自定义**httpd-custom.conf**文件强制重新启用此功能。从 5.3 版本开始，**已无法再启用 SSL 3.0**，尝试启用会导致 HTTP 服务启动失败。
    升级到 VisualSVN Server 5.3 之前，必须删除 **httpd-custom.conf**文件 中以下与 SSL 3.0 相关的自定义设置 ：
   SSLProtocol——**通常情况下，必须完全移除此指令。如果无法移除此指令，则必须移除其`SSLv3`值，以防止尝试启用 SSL 3.0。
5. **确认未使用 Internet Explorer 访问 VisualSVN 服务器 Web 界面**：
    从 5.0 版本开始，VisualSVN 服务器 Web 界面 **不再支持 Internet Explorer**。如果您使用 Internet Explorer 访问 Web 界面，则需要升级到 Microsoft Edge 或切换到其他受支持的浏览器。微软鼓励用户迁移到具有 Internet Explorer (IE) 模式的 Microsoft Edge 浏览器。
    *此更改仅影响客户端存储库 Web 界面，不影响任何服务器管理功能或 VisualSVN 服务器管理器控制台。*
6. **检查 TLS/SSL 兼容级别设置（不再支持 SSL 3.0）**：
    VisualSVN Server 5.0 将不再支持 SSL 3.0 协议（即使启用了旧版 TLS/SSL 兼容级别）。对于几乎所有与服务器交互的现代客户端而言，这不会造成任何负面影响。但是，不支持 SSL 3.0 以上协议的旧版客户端在服务器升级后会收到访问错误。如果您有此类客户端（例如基于 Java 5 或更早版本的客户端），则需要事先将其升级。
7. **从 httpd-custom.conf 中移除与 MaxMemFree 指令相关的自定义设置**：
    此步骤适用于从 VisualSVN Server 4.3 及更早版本升级，这些版本在 %VISUALSVN_SERVER%conf\ **httpd-custom.conf**文件中进行了自定义。
    VisualSVN Server 5.0 切换到了一种新的内存分配方案，该方案不再包含`MaxMemFree`配置指令。升级到 VisualSVN Server 5.0 之前，您应该检查**httpd-custom.conf**文件并删除以下指令： **最大内存自由**。
8. **如果需要自定义权限才能访问存储库，请跳过在安装程序中启用搜索索引的步骤**：
   如果需要自定义权限才能访问服务器上的存储库（这种情况通常发生在存储库 [存储在网络共享上](https://www.visualsvn.com/support/topic/00022/ "知识库22：将存储库存储在网络共享上")且权限已授予[专用帐户](https://www.visualsvn.com/support/topic/00024/ "KB24：配置 VisualSVN HTTP 服务以在专用用户帐户下运行")时），请跳过在安装程序中启用搜索索引的步骤。
9. **从 httpd-custom.conf 中移除与 mod_mpm_winnt 相关的自定义设置**：
    此步骤适用于从 VisualSVN Server 4.1 或更早版本升级，并%VISUALSVN_SERVER%conf\ **httpd-custom.conf**文件中也有自定义设置的情况。
    VisualSVN Server 4.2 使用的是**mod_mpm_visualsvn**模块而不是 **mod_mpm_winnt**模块，因此某些自定义配置会导致配置错误，VisualSVN HTTP 服务将无法启动。您应该仔细检查并移除这些自定义配置。
   **升级前，请考虑从httpd-custom.conf**文件 中删除以下不受支持的指令：
     - **每个子节点的线程数**
     - **线程限制**
10. **检查您是否需要更新 VisualSVN 服务器许可证**：
    此步骤适用于从 VisualSVN Server 3.9 及更早版本升级。
    从 4.0 版本开始，VisualSVN Server 可以在三种类型的许可证下运行：社区版、基础版或企业版。
    如果您使用的是 VisualSVN Server 企业版，并且拥有未过期的维护订阅许可证，则升级到新的许可模式是完全透明的。
    如果您使用的是**标准版**，升级到 4.1 或更高版本将自动激活功能几乎相同的社区版许可证。但是，在以下两种情况下，升级期间社区版许可证将不可用：
    - 当**Subversion 用户帐户超过 15 个**时，社区版许可证最多允许使用 15 个 Subversion 用户帐户。
    - 当您使用**Windows（基本）身份验证**时。从 4.0 版本开始，Windows 身份验证仅适用于企业版许可证。
    对于上述例外情况，您可以选择提供足够的许可证，或者生成一个免费的评估许可证，允许您在 45 天内使用 VisualSVN Server 的所有功能。如果选择后者，您需要在评估期结束前提供足够的 Essential 或 Enterprise 许可证。
11. **确认未使用 Internet Explorer 10 访问 VisualSVN 服务器 Web 界面**：
    从 VisualSVN Server 4.0 开始，最低支持的 IE 版本为 **Internet Explorer 11**如果您仍在使用 Internet Explorer 10，则需要升级您的浏览器。
12. **从 httpd-custom.conf 中移除与 mod_authz_svn 相关的自定义设置**：
    VisualSVN Server 3.9 不包含**mod_authz_svn模块，因此所有使用此模块指令的自定义设置都将停止工作。您应该仔细检查并移除这些自定义设置。以下是升级到 VisualSVN Server 4.1 之前，您应该考虑从httpd-custom.conf**文件 中移除的指令列表：
     - **AuthzForceUsernameCase**
     - **授权SVN组文件**
     - **授权SVN访问文件**
     - **AuthzSVNAnonymous**
     - **AuthzSVN权威**
     - **AuthzSVNNoAuthWhenAnonymousAllowed**
     - **授权SVN存储库相对访问文件**
13. **确认没有与新版本不兼容的自定义设置**:
    **如果您在httpd-custom.conf**文件 中进行了任何自定义设置，则此步骤适用于 VisualSVN Server 的升级。VisualSVN Server 3.7 将**Apache HTTP Server**升级到了2.4 版本，并对相关的 httpd.conf 文件进行了重大更改。因此，升级后您的自定义设置很可能会失效。
    **如果您的 VisualSVN Server 实例中的 %VISUALSVN_SERVER%conf\ httpd-custom.conf**文件不为空， 强烈建议在预生产环境中运行测试升级。
### 升级后检查清单：
1. **启用基于 IP 的基本 Windows 身份验证限制**：
   从 5.4 版本开始，您可以将基本 Windows 身份验证的使用范围限制为仅允许特定 IP 地址的客户端使用，这些客户端无法使用集成 Windows 身份验证。这样，您可以仅为某些用户或客户端启用基本 Windows 身份验证，而不会降低其他用户的安全级别。
2. **确定您的环境是否依赖于捆绑的 openssl.exe 命令行实用程序**：
   从 5.4 版本开始，VisualSVN Server默认 不再安装 **openssl.exe命令行实用程序**。而且，默认情况下，升级到 5.4 版本也会移除该可执行文件。
   需要确定您或您的环境是否依赖于服务器安装目录中的 openssl.exe 文件。如果您发现 VisualSVN Server 安装中必须包含 openssl.exe，请联系 [support@visualsvn.com](mailto:support@visualsvn.com)获取进一步帮助。
   *运行或管理 VisualSVN 服务器不需要 openssl.exe 命令行实用程序。*
3. **启用密码猜测攻击防护**：
   [密码猜测攻击防护](https://www.visualsvn.com/server/features/password-attack-protection/) 是一种反制措施，旨在防止攻击者通过系统地尝试大量不同的用户名和密码组合来发现 VisualSVN 服务器的用户登录/身份验证凭据。
   启用此功能后，VisualSVN 服务器可以暂时阻止在短时间内产生过多身份验证失败的 IP 地址。
   *强烈建议**使用独立**Subversion 身份验证**模式或**具有基本身份验证的 Windows 身份验证模式**的 VisualSVN Server 安装执行 此步骤。*
4. **安排存储库备份验证**：
   “验证备份”计划后台作业有助于确保 VisualSVN 服务器创建的定期备份可读、可还原，并在必要时随时可用。该作业会自动验证“备份存储库”后台作业创建的备份。
   **强烈建议**所有安装了 VisualSVN Server 并配置了计划存储库备份作业的用户 创建“验证备份”作业。
5. **检查 TLS/SSL 兼容级别设置（TLS 1.0 和 TLS 1.1 现在处于旧版模式）**：
   **VisualSVN Server 5.0 中的中级**TLS/SSL 兼容级别不再启用已弃用的 TLS 1.0 和 1.1 协议。这对与服务器交互的现代客户端没有负面影响。但是，不支持 TLS 1.2 的旧版客户端在服务器升级后会收到访问错误。
   请关注以下客户：
    - TortoiseSVN 版本 1.7.8 或更早版本。
    - Subversion 命令行客户端版本 1.8.x 或更早版本。
    - 所有基于 Java 7 或更早版本的客户端。
   如果旧版本客户端出现访问错误，请考虑将其更新到支持 TLS 1.2 的新版本。如果由于某种原因无法立即更新，请将 TLS/SSL 设置切换为**旧版**配置。
   *VisualSVN Server 5.0 中的 现代TLS/SSL 兼容级别仅支持 TLS 1.3。*
6. **启用存储库全文搜索**：
   VisualSVN Server 5.0 提供了全新的 [**全文搜索功能**](https://www.visualsvn.com/server/features/search) ，允许用户通过 Web 界面查找包含指定内容的文件。*如果需要自定义权限才能访问服务器上的存储库（当存储库 [存储在网络共享上](https://www.visualsvn.com/support/topic/00022/ "知识库22：将存储库存储在网络共享上")并且权限授予[专用帐户](https://www.visualsvn.com/support/topic/00024/ "KB24：配置 VisualSVN HTTP 服务以在专用用户帐户下运行")时，这种情况经常发生），请不要忘记在启用搜索索引之前 [为 VisualSVN 搜索索引服务配置适当的权限](https://www.visualsvn.com/support/topic/00197/ "KB197：配置 VisualSVN 搜索索引服务以在专用用户帐户下运行") 。*
7. **启用管理员电子邮件通知**：
   VisualSVN Server 5.0 支持**管理员电子邮件通知**。这些通知会在后台作业完成后发送，告知管理员作业的运行状态。如果您的服务器配置了任何后台作业，建议启用管理员电子邮件通知。
8. **检查 TLS/SSL 兼容级别设置（3DES 现在属于传统模式）**：
   VisualSVN Server 4.2**中级**TLS/SSL 配置不再使用过时的 3DES 加密套件。这对与服务器交互的现代客户端没有任何负面影响。但是，仅支持 3DES 的旧版客户端（例如 Windows XP 或更早版本上的 Internet Explorer 8）在服务器升级后会收到访问错误。
   如果由于客户端版本过旧而收到访问错误，请将其更新到更新的兼容版本（支持现代 AES 加密套件）。如果由于某种原因无法立即更新，请将 TLS/SSL 设置切换为传统配置。
9. **更新自定义 HTTP 和 HTTPS 防火墙规则（服务可执行文件现为 vsvnhttpsvc.exe）**：
   如果您有**自定义**防火墙规则，其中引用了 VisualSVN HTTP 服务的**可执行文件名**，则需要更新这些规则。请注意，这不适用于 Windows 防火墙预配置的“VisualSVN 服务器 (TCP-In)”规则，该规则将由安装包自动更新。
10. **请确保您的 HTTPS 证书允许使用 TLS 1.3**：
    您需要仔细检查您的证书是否允许使用 TLS 1.3。在 VisualSVN Server 4.0 或更早版本中生成的自签名证书始终需要重新生成才能在服务器上启用 TLS 1.3。
    VisualSVN Server 4.1 在服务器端增加了对 TLS 1.3 协议的支持。通常情况下，升级后无需任何特殊步骤即可使用 TLS 1.3 协议。但是，当前安装的证书可能会阻止 TLS 1.3 协议的使用。在这种情况下，您需要生成并安装新的证书。
    请按照以下步骤查看您的证书是否支持 TLS 1.3，以及是否需要采取其他步骤：
    启动**VisualSVN 服务器管理器**控制台。 点击**操作 | 属性**。 点击“证书”选项卡。
     - 如果在“证书”选项卡上没有看到警告，则表示该证书符合 TLS 1.3 标准，无需采取其他步骤。
    - 如果您看到以下警告，则说明证书存在问题，导致服务器无法使用 TLS 1.3**：
         此证书目前阻止服务器上使用 TLS 1.3。
        您需要生成并安装新的 TLS 证书来解决此问题。
11. **将存储库升级到 Subversion 1.10 格式**：
    您需要将存储库升级到 Subversion 1.10 格式，才能享受 VisualSVN Server 3.9 和 Subversion 1.10 中实现的性能改进。
12. **请确保服务器计算机上已安装 PowerShell 4.0 或更高版本**：
    [使用Backup-SvnRepository](https://www.visualsvn.com/support/topic/00088/#Backup-SvnRepository) 和 [Restore-SvnRepository](https://www.visualsvn.com/support/topic/00088/#Restore-SvnRepository) PowerShell cmdlet 创建和还原加密备份，或为远程备份目标提供自定义凭据时，需要 PowerShell 4.0 或更高版本 。我们建议您确保服务器计算机上已安装 PowerShell 4.0 或更高版本。
    PowerShell 没有独立安装程序，它仅作为 Windows 管理框架 (WMF) 的一部分提供。您需要升级到 WMF 4.0 才能升级到 PowerShell 4.0。
    *安装 WMF 4.0 时可能会出现以下问题：在未运行 .NET Framework 4.5 的计算机上安装 WMF 4.0 时，系统会报告安装成功，但 WMF 4.0 的组件（例如 Windows PowerShell、WMI 等）实际上并未更新。要解决此问题，请安装 .NET Framework 4.5 并再次运行 WMF 4.0 安装程序。有关此问题的更多信息，请阅读 MSDN 上 PowerShell 团队的博客文章“ [WMF 4.0 – 已知问题：未安装 .NET Framework 4.5 时部分安装”。](https://blogs.msdn.microsoft.com/powershell/2013/10/28/wmf-4-0-known-issue-partial-installation-without-net-framework-4-5/)*
13. **从 httpd-custom.conf 中移除对动态 HTTP 压缩所做的自定义设置**：
    VisualSVN Server 3.7 对动态 HTTP 压缩进行了重大改进，并提供了一个用户界面来调整相关设置。如果您的服务器 httpd-custom.conf文件被修改以自定义动态 HTTP 压缩设置，这些自定义设置可能会与核心**httpd.conf配置文件**中的设置冲突 。您应该仔细检查并移除这些自定义设置。
    以下是升级到 VisualSVN Server 4.1 后， 您应该考虑从**httpd-custom.conf**文件 中移除的指令列表 ：
    - **SVN压缩级别**
    - **加载模块 mod_deflate**
    从 VisualSVN Server 3.7 开始，动态 HTTP 压缩设置只能通过 VisualSVN Server 的用户界面进行调整。
14. **从 httpd-custom.conf 文件中移除对 TLS/SSL 安全设置的自定义项**：
    VisualSVN Server 3.6 提供了一个用户界面，用于自定义 TLS/SSL 安全兼容级别。如果您的服务器 **httpd-custom.conf 文件**被修改以自定义 TLS/SSL 设置，这些自定义设置可能会与核心 **httpd.conf**配置文件中的设置冲突。您应该仔细检查并移除这些自定义设置。
    以下是升级到 VisualSVN Server 4.1 后， 您应该考虑从**httpd-custom.conf**文件 中移除的指令列表 ：
    - **SSL协议**
    - **SSL密码套件**
    从 VisualSVN Server 3.6 版本开始，TLS/SSL 安全设置需要通过 VisualSVN Server 的用户界面进行调整。
15. **从 httpd-custom.conf 中移除对 Subversion 内存缓存大小的自定义设置**：
    VisualSVN Server 3.6 提供了一个用户界面，用于自定义 Subversion 内存对象缓存的大小，以优化服务器性能。如果您的服务器**httpd-custom.conf文件被修改以更改 Subversion 内存缓存大小，这些自定义设置可能会与核心httpd.conf配置文件中的设置冲突。您应该仔细检查并移除这些自定义设置。

升级到 VisualSVN Server 4.1 后， 您应该考虑从**httpd-custom.conf**文件 中移除以下指令 ：

- **SVN内存缓存大小**

从 VisualSVN Server 3.6 开始，Subversion 内存缓存大小需要通过 VisualSVN Server 的用户界面进行调整。