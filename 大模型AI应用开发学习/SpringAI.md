# SpriinigAI简介
## SpringAI是什么
1. SpringAI是一个AI工程领域的应用程序框架。
2. 其目标是将Spring生态系统的设计原则应用于AI领域，比如Spring生态的可移植性和模块化设计，并促进使用POJO作为应用程序的构建块到AI领域
3. SpringAI的核心是提供了开发AI大模型应用所需的基本抽象模型，这些抽象有多种实现方式，使得开发者仅用很少的代码改动就能轻松的实现组件替换
## Spring AI的主要功能
- 对主流AI大模型供应商提供了支持
- 支持AI大模型类型包括：聊天，文生图，文本到声音等
- 支持主流的Embedding Models（嵌入模型）和向量数据库
- 把AI大模型的输出映射到简单Java对象上
- 支持函数调用功能
- 为数据工程提供ETL（数据抽取、转换和加载）框架
- 支持SpingBoot自动配置和快速起动，便于管理AI模型和管理向量库
# Spring AI快速入门
我的deepseek API key:  sk-f92c680bffa34db5bd2f3491cbaded7d
# SpringAI的聊天模型
## 概述
- SpringAI的聊天模型API为开发者提供了一条便捷通道，能够将强大的AI驱动的聊天完成功能无缝集成到各类应用中。这一API不仅工作机制高效，而且设计理念极为先进，旨在实现简单易用与高度可移植性，让开发者能以极少的代码改动在不同AI模型间自由切换，充分契合Spring框架一贯秉承的模块化与可互换性原则。
## ChatClient接口
ChatClient是一个接口，它定义了一个与聊天服务交互的客户端。这个接口主要用于创建聊天接口客户端对象，设置请求规范，以及发起聊天请求。
## ChatModel接口
- ChatModel接口作为核心，定义了与AI模型交互的基本方法。它继承自Model<Prompt,ChatResponse>,提供了两个重载的call方法：
     1. call(String message)
     2. cal(Prompt prompt)
- 在此接口中，带有String参数的call方法简化了实际的使用，避免了更复杂的Prompt和ChatResponse类。但是在实际应用中，更常见的是使用ChatResponse call()方法，该方法采用Prompt实例并返回ChatResponse。
- ChatClient底层是使用ChatModel为属性的，在初始化ChatClient时可以指定ChatModle。
