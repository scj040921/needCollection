# 聊天程序与 LVGL 前端改动记录

> 项目：`qianrushixuexi_scj/chatproject`
>
> LVGL 工程：`Downloads/lv_port_pc_eclipse-release-v9.2`
>
> 记录时间：2026-09-05

## 一、聊天程序已完成的修改

### 1. 统一代码风格

参考 `C:/Users/24656/Downloads/阶段项目` 中的 C 代码风格，对聊天程序进行了最小范围排版调整：

- 控制结构采用分行大括号。
- 条件判断和循环中的单条语句分行书写。
- 增加与参考项目一致的中文说明注释。
- 调整 `list.c`、`list.h`、`myhead.h` 的 include 和结构体注释格式。
- 清理新增修改中的行尾空格。
- 保留原有通信协议、端口、字符串、判断逻辑和调用顺序。

涉及文件：

- `chatproject/client.c`
- `chatproject/client2.c`
- `chatproject/server.c`
- `chatproject/list.c`
- `chatproject/list.h`
- `chatproject/myhead.h`

### 2. 使用 `ret` 管理函数返回值

将部分直接写在条件表达式中的函数调用改为先保存返回值，再判断：

```c
int ret;
ret=bind(...);
if(ret < 0)
{
    ...
}
```

已应用于：

- `bind()`
- `listen()`
- `accept()`
- `send_all()`
- `relay_data()`
- `recv_line()`
- `pthread_create()` 的返回值保存

同时修正了接收循环，避免使用 `continue` 时跳过下一次接收：

```c
while(1)
{
    ret=recv_line(tcpsock, line, sizeof(line));
    if(ret != 0)
        break;

    ...
}
```

### 3. 保留文件原名和后缀

发送文件时：

- 完整路径仍用于本地 `fopen()`，因此可以读取任意目录中的文件。
- 发送给服务器的协议字段改为文件名 basename。
- 例如：

```text
本地路径：C:\Users\24656\Documents\...\emoji_1.jpg
协议文件名：emoji_1.jpg
```

客户端使用 `strrchr(path, '/')` 提取文件名，解决服务器因检测到路径中的 `/` 或 `\\` 而提示“非法文件名，已拒绝”的问题。

涉及文件：

- `chatproject/client.c`
- `chatproject/client2.c`

### 4. 文件重复发送的现状

接收端仍然使用原文件名保存：

```c
FILE *fp = fopen(name, "wb");
```

因此反复发送同名文件时，后一次会覆盖前一次。要像社交软件一样保留每条历史消息，需要后续增加“显示文件名”和“实际存储名”的分离，例如使用消息 ID 生成内部存储名；该功能目前没有修改，以遵守暂不改变原功能逻辑的要求。

## 二、验证结果

已在 Ubuntu-24.04 WSL 中进行严格编译验证：

```bash
gcc -std=c11 -Wall -Wextra -Werror -pthread client.c list.c -o client
gcc -std=c11 -Wall -Wextra -Werror -pthread client2.c list.c -o client2
gcc -std=c11 -Wall -Wextra -Werror -pthread server.c list.c -o server
```

结果：

- `client.c` 编译通过。
- `client2.c` 编译通过。
- `server.c` 编译通过。
- 文件名提取检查通过。
- `git diff --check` 通过。

## 三、LVGL 前端工作状态

已检查 `lv_port_pc_eclipse-release-v9.2` 工程：

- LVGL 版本为 9.2.3。
- 使用 SDL PC 模拟器。
- 当前入口文件为 `main.c`。
- 自定义 C 代码目录为 `mycode/`。
- CMake 会自动编译 `mycode/` 目录下的 C 文件。
- 当前模拟器窗口初始化尺寸为 `800x480`。

已创建设计文档：

- `lv_port_pc_eclipse-release-v9.2/docs/chat-ui-design.md`

设计方向为三栏社交软件布局：

- 左栏：会话列表和在线客户端。
- 中栏：当前聊天窗口和消息气泡。
- 右栏：客户端详情与文件传输状态。
- 点击不同客户端后切换独立的会话记录。
- 网络接收线程通过消息队列向 UI 线程传递消息，不直接操作 LVGL。

## 四、尚未完成的内容

截至本记录生成时，以下内容尚未实现：

- 尚未将客户端代码复制到 LVGL 工程。
- 尚未创建 `mycode/chat/chat_ui.c`。
- 尚未创建 `mycode/chat/chat_adapter.c`。
- 尚未修改 LVGL 工程的 `main.c` 接入聊天界面。
- 尚未完成客户端连接、会话切换、消息发送和文件发送的 LVGL 控件绑定。
- 尚未构建并运行最终的 LVGL 聊天界面。

因此，目前不能将 LVGL 前端描述为已经完成；当前完成的是工程勘察和界面设计规格。

## 五、重要约束

后续实现 LVGL 前端时应遵循：

1. 不修改原聊天项目中的源程序文件。
2. 需要适配的客户端代码复制到 LVGL 工程后再修改。
3. 不改变原通信协议和已有网络功能逻辑。
4. 每个客户端对应独立会话窗口，切换客户端时保留各自消息记录。
5. 完整路径只用于本地文件读取，发送协议只携带原文件名和后缀。
6. 网络线程不直接调用 LVGL API。
7. 对非编译错误和代码逻辑错误只做一轮测试；失败后记录错误原因，不反复测试。
