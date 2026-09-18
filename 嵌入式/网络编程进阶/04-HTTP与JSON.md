---
title: HTTP与JSON设备接口
tags: [嵌入式, 网络编程, HTTP, JSON]
source: 有道公开笔记 09.网络编程（HTTP、云API与JSON）
---

# HTTP 与 JSON 设备接口

## HTTP 消息结构

请求：请求行（如 `POST /api/report HTTP/1.1`）+ 首部 + 空行 + 可选正文。响应：状态行（如 `HTTP/1.1 200 OK`）+ 首部 + 空行 + 可选正文。

- `Content-Type` 描述正文格式；JSON 常用 `application/json`。
- `Content-Length` 或 `Transfer-Encoding: chunked` 决定正文边界；不能以一次 `recv` 作为 HTTP 请求结束。
- 常见状态：`200` 成功、`400` 请求格式错、`401/403` 身份/权限问题、`404` 路径不存在、`5xx` 服务端异常。

```http
POST /v1/telemetry HTTP/1.1
Content-Type: application/json
Content-Length: 28

{"temp":26.4,"device":"lcd-01"}
```

## JSON：语法正确还不够

JSON 有对象、数组、字符串、数字、布尔和 `null`。解析后还要验证：字段存在性、类型、数值范围、字符串长度、数组数量和版本兼容性。

```c
// 伪代码：先检查，再使用；绝不把网络字段直接拷贝到固定数组。
if (!has_number(root, "temp") || get_number(root, "temp") < -40 ||
    get_number(root, "temp") > 125) return BAD_REQUEST;
```

## 嵌入式接云 API 的设计

- 把“采集、编码、传输、重试、持久化”拆开；网络线程不直接阻塞采集主循环。
- 设置连接/读取超时和正文上限；失败用指数退避，离线缓存要有容量与丢弃策略。
- TLS 需要校验证书和时间；不要为调试永久关闭证书校验。
- 记录请求 ID、状态码和有限长度错误摘要，避免日志泄露令牌、密码或完整隐私数据。

## 深入讲解：HTTP 是增量状态机

先在缓冲中寻找 `\r\n\r\n` 得到完整首部；逐行限制首部长度与数量；根据 `Content-Length` 等待完整正文，或按 chunked 规则逐块读取。一次 `recv` 绝不等于一次 HTTP 请求；缓冲不足必须保留到下次读取。

服务器还应有“首部读取超时、请求正文上限、连接空闲超时”，避免慢速客户端长期占着有限的嵌入式连接资源。

## JSON 解析后的业务校验

```json
{"version":1,"deviceId":"stm32-01","tempC":26.4}
```

解析成功仅表示语法正确。还要检查：`version` 是否支持、`deviceId` 是否超长/含非法字符、`tempC` 是否为数值且在传感器范围、未知字段如何兼容。超时重试时要考虑“服务器可能已执行请求但响应丢失”；上报可用 `requestId` 让服务端去重，并只重试幂等操作。
