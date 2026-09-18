---
title: UDP与套接字实践
tags: [嵌入式, 网络编程, UDP, Wireshark]
source: 有道公开笔记 09.网络编程（UDP与抓包）
---

# UDP 与套接字实践

## TCP/UDP 如何选

| 项目 | UDP | TCP |
| --- | --- | --- |
| 连接与开销 | 无连接、头小 | 建连和维护状态 |
| 可靠性/顺序 | 不保证，可能丢/乱序/重复 | 内核保证可靠、有序字节流 |
| 边界 | 一次接收对应一个数据报 | 无业务消息边界 |
| 典型场景 | 发现、广播、实时遥测 | 控制指令、文件、配置下发 |

UDP 不是“不可靠就不能用”：若业务可容忍偶尔旧数据，或自己有序号、超时和重传策略，它常更简单、更低延迟。

## 最小通信流程

接收端 `socket(AF_INET, SOCK_DGRAM, 0)` → `bind`；发送端 `socket` → `sendto`。接收端用 `recvfrom`，它会同时给出发送方地址，回复时可直接 `sendto` 回该地址。

```c
char buf[512]; struct sockaddr_in peer; socklen_t n = sizeof peer;
ssize_t r = recvfrom(fd, buf, sizeof buf, 0,
                     (struct sockaddr *)&peer, &n);
if (r >= 0) { /* buf[0..r-1] 才是有效数据 */ }
```

## 实战坑

- 缓冲不足时 UDP 报文可能被截断；设计协议时限制最大报文，检查接收长度。
- UDP 不会替你确认“对方应用已处理”；需要确认时在应用层加序号/ACK/超时重发，并限制重试次数。
- 广播/组播需要匹配网络设备、路由和 socket 选项；不要把私有控制报文暴露到不可信网段。
- `inet_pton`/`inet_ntop` 负责文本 IP 与二进制地址转换，比老接口更适合 IPv4/IPv6。

## Wireshark 的正确用法

抓包用来验证“实际字节”，不是只看程序日志。过滤例子：`udp.port == 9000`、`tcp.port == 8080`、`ip.addr == 192.168.1.10`。观察源/目的端口、长度、序号、重传和时间间隔，再回到代码定位问题。
