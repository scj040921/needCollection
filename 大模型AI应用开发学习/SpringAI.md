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
        *call方法是非流式输出，会等待大模型完全生成完结果后直接输出给用户，stream方法可以进行流式输出（逐个字符进行输出）可以提升用户体验*
- 在此接口中，带有String参数的call方法简化了实际的使用，避免了更复杂的Prompt和ChatResponse类。但是在实际应用中，更常见的是使用ChatResponse call()方法，该方法采用Prompt实例并返回ChatResponse。
- ChatClient底层是使用ChatModel为属性的，在初始化ChatClient时可以指定ChatModle。
## 提示词
- 提示词是引导大模型生成特定输出的输入，提示词的设计和措辞会极大地影响模型地响应结果
- Prompt提示词是与模型交互的一种数据输入组织形式，本质上是一种复合结构的输入，在Prompt可以包含多组不同角色的信息，如何管理好Prompt是简化AI应用开发的关键环节
- SpringAI提供了Prompt Template提示词模板管理抽象，开发者可以预先定义好模板，并在运行时替换模板中的关键词。在SpringAI与大模型的交互过程中，处理提示词首先要首先要创建包含动态内容的占位符的模板，然后，这些占位符会根据用户请求或应用程序中的其他代码进行替换。在提示词模板中，占位符可以用Map中的变量动态替换
# SpringAI的函数调用
## 概述
- Spring AI的函数调用功能允许大语言模型在生成回答时触发预定义的外部函数，从而实现动态数据获取或业务逻辑操作
- Spring AI帮我们规范了函数定义、注册等过程，并在发起模型请求之前自动将函数注入到Prompt中，而当模型决策在某个合适的时候去调用某个函数时，Spring AI完成函数调用动作，最终将函数执行结果与原始问题再一次发送给模型，模型根据新的输入决策下一步动作。一次函数调用就是一次完整的交互过程。
- **函数调用的核心流程**
	- 第一步：定义函数，声明可供模型调用的函数
	- 第二步：模型交互，将函数信息与用户输入一起发送给模型，模型决定是否需要调用函数
	- 第三步：执行函数，解析模型的函数调用请求，执行对应的业务逻辑
	- 第四步：返回结果，将函数执行结果返回给模型，生成最终回答
## 函数调用实现
### 创建自定义的Function
- Spring AI使自定义函数这个过程变得简单，只需定义一个返回java.util.Function的@Bean定义，并在调用ChatModel时将bean名称作为选项进行注册即可。在底层，Spring会用适当的适配器代码包装你的函数，以便与AI模型进行交互，免去了繁琐的代码。
### 编写Controller 
- 为了让模型知道并调用自定义函数，需要在Prompt请求中启用它，在tools()中告知ChatClient要使用哪些自定义函数。
- 另外指定了System Prompt:要求AI模型被设定为一个函数功能代理，能够执行相关的操作，并要求用户提供参数。
# Spring Ai Alibaba
## 概述
- Spring Ai Alibaba是阿里云通义系列模型以及服务在JAVA AI应用开发领域的最佳实践，通过提供一种方便的API抽象，帮助java开发者加速和简化java智能体应用的开发。同时，提供了完整的开源配套，包括可观测、网关、消息队列、配置中心等。
- Spring Ai Alibaba已完整提供Model、Prompt、RAG、Tools等AI应用开发所需的必备能力，将兼具提示词模板、函数调用、格式化输出等低层次抽象，以及RAG、智能体、对话记忆等高层次抽象
- 阿里API Key：sk-b11f9437fcc240b485876e9342b00e8f
# Spring AI的其它模型
## 图像模型
### Image Model API概述
- 在Spring AI中，Image Model API旨在为与专注于图形生成的各种模型进行交互提供一个简单且可移植的接口，使开发者可以以最小的代码改动切换不同的图像相关模型。这一设计符合spring模块化和互换性的理念，确保开发者可以快速调整其应用程序以适应不同的图像处理相关的AI能力
- 通过支持ImagePrompt这样的辅助类来进行输入封装以及使用ImageResponse来处理输出，图像模型API统一了与致力于图像生成的AI模型之间的通信。它管理请求准备和响应解析的复杂性，为图像生成功能提供直接而简化的API交互
- ImageModelAPI建立在SpringAI的通用模型API之上，提供了特定于图像的抽象和实现
### 接口及相关类
1. ImageModel图像模型
2. ImagePrompt图像提示：
   是一个封装了ImageMessage对象列表及可选模型请求选项的ModelRequest.
3. ImageMessage图像消息:
   封装了用于影响生成图像的文本及其权重。对于支持权重的模型他们可以是正数或负数
4. ImageOptions图像选项：
   表示可以传递给图像生成模型的选项。扩展了ModelOptions接口，并用于定义可以传递给AI模型的一些可移植选项
5. ImageResponse图像响应：
   持有AI模型的输出，每个ImageGeneration实例包含来自单一提示的多个可能输出结果之一
6. ImageGeneration图像生成 
   此类扩展自ModelResult,代表输出响应及有关此结果的元数据
### SpringAI 实现生成图像

## 语音模型