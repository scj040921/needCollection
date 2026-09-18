---
title: 网络基础与TCP
tags: [嵌入式, 网络编程, TCP]
source: 有道公开笔记 09.网络编程（TCP与套接字属性）
---

# 网络基础与 TCP

## 先分清三个地址

- **MAC**：局域网链路上找网卡；**IP**：网络层找主机；**端口**：传输层找主机内进程。
- `sockaddr_in` 保存 IPv4 地址和端口；`htons/htonl` 转为网络字节序，`ntohs/ntohl` 转回主机字节序。
- DNS 把域名解析成 IP；嵌入式排障应优先验证 IP/路由，再验证 DNS 与服务端口。

## TCP：可靠字节流，不是“消息”

TCP 通过序号、确认、重传、流量控制与拥塞控制提供有序可靠传输。三次握手确认双向收发能力；主动关闭方进入 `TIME_WAIT`，用于避免旧报文干扰新连接。

```text
服务端：socket → bind → listen → accept → recv/send → close
客户端：socket → connect → send/recv → close
```

`accept` 返回新的已连接 fd；监听 fd 继续接收新连接。`recv == 0` 表示对端已正常关闭；`< 0` 时检查 `errno`，例如被信号中断的 `EINTR`。

## 必须自己定义帧

发送两次不等于接收两次。推荐“固定长度头 + 载荷”的协议：头中含魔数、版本、长度、类型；接收端先累积到完整头部，校验长度上限，再累积载荷。

```c
struct Header { uint32_t magic, len; uint16_t type, ver; };
// 所有多字节字段发送前 htonl/htons；len 必须有合理上限。
```

> [!tip] 面试回答
> “粘包/半包不是 TCP 出错，而是应用把字节流误当消息；用长度字段或明确分隔规则在应用层恢复消息边界。”

## 常用套接字属性

| 选项 | 用途 | 不能误解为 |
| --- | --- | --- |
| `SO_REUSEADDR` | 服务重启时降低地址占用问题 | 允许任意多个服务共享端口 |
| `SO_KEEPALIVE` | 内核级存活探测 | 业务心跳与重连策略 |
| `SO_RCVTIMEO`/`SO_SNDTIMEO` | 为阻塞收发设置超时 | 自动完成业务重试 |
| `TCP_NODELAY` | 关闭 Nagle，降低小包延迟 | 必然提升总吞吐 |

嵌入式端要有连接超时、读写超时、心跳、断线重连和发送队列上限；网络抖动不能无限堆积内存。

## 深入讲解：半包、粘包与接收缓冲

`recv()` 的返回值只是“当前内核缓冲中取到了多少字节”，不是“取到一条消息”。一个长度为 100 的业务包可能分三次到达；反过来，两条业务包也可能一次到达。因此接收缓冲必须跨多次调用保存数据。

```c
while (rx_size() >= HEADER_SIZE) {
    uint32_t len = read_be32(rx_data() + 4); // 取网络序长度
    if (len == 0 || len > 4096) { close_bad_peer(); break; }
    if (rx_size() < HEADER_SIZE + len) break; // 半包：等下一次 EPOLLIN
    handle_frame(rx_data() + HEADER_SIZE, len);
    rx_consume(HEADER_SIZE + len);            // 继续处理缓冲中的下一帧
}
```

帧头至少应有长度、版本和消息类型；长度必须设置上限，否则恶意对端可宣称超大载荷，造成内存耗尽。处理函数若需异步保存数据，必须复制/转移所有权，不能保留即将被缓冲复用的裸地址。

## 深入讲解：短写与优雅关闭

`send()` 也可能只写入部分数据。非阻塞 fd 遇到 `EAGAIN` 意味着暂时写不进，应保留“已发送偏移”，等待 `EPOLLOUT` 后续传，不能把未发送的尾部丢掉。`EINTR` 通常重试；`EPIPE/ECONNRESET` 说明连接已不可用。

主动关闭前若需要尽力送完回复：停止接收新请求 → 刷发送队列 → `shutdown(fd, SHUT_WR)` → 等待对端 EOF/超时 → `close`。实时控制场景则可以直接关闭，关键是把策略定义清楚。
