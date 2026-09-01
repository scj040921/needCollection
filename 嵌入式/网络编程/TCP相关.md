# 基本概念
## 1.网络模型
### OSI七层模型
- 7. **应用层**：为应用程序提供网络服务接口，直接面向用户需求
- 6.**表示层**：处理数据的格式转换、加密解密、压缩解压缩，确保通信双方能识别数据
- 5.**会话层**：建立管理和终止双方的会话连接，控制会话交互方式
- 4.**传输层**：负责端到端的数据传输，处理分段、重传、流量控制
- 3.**网络层**：实现不同网络间的寻址和路由选择，将分组转发到目标网络
- 2.**数据链路层**：将物理层的路径封装成帧，处理MAC地址寻址和链路级错误检测
- 1.**物理层**：定义物理介质的电气、机械特性，传输二进制比特流
### TCP/IP模型
- 应用层（原本的应用层、会话层、表示层合并）
- 传输层
- 网际层/IP层
- 网络接口层（数据链路层和物理层合并）
## 2.ipv4和ipv6
- ipv4：32位的ip地址，用三个小数点把ip划分为四个部分（点分十进制），每个部分占一个字节
- ipv6：解决ipv4地址不够用的问题，128位的ip地址
## 3.端口号
- **作用**：区分一台主机内部不同的网络进程
- 本质是无符号的短整型（0-65535）
- 程序员编程时可以指定端口号，但是不能指定1024以内的因为很多都被系统占用了
## 4.不同主机之间网络编程的注意事项
- 不同主机之间网络必须联通
# TCP通信协议
## 1.通信流程
- 创建tcp套接字
- 绑定ip地址和端口号
- 客户端连接服务器，服务器监听客户端
- 服务器接收客户端的连接请求accept
- 收发消息
- 关闭连接
## 2.相关的接口函数
### 创建tcp套接字
int socket(int domain,int type,int protocol);
- **参数**：
	- domain：地址协议类型
	- type：套接字的类型
		- tcp：SOCK_STREAM
		- udp：SOCK_DGRAM
	- protocol：扩展协议一般设置0
### 绑定ip和端口值
int bind(int sockfd,const struct sockaddr\*addr,socklen_t addrlen)
- **参数**：
	- sockfd：套接字的文件描述符
	- addr：
		- struct sockaddr 通用地址结构体
		- struct sockaddr_in6 ipv6地址结构体
		- struct sockaddr_in ipv4地址结构体
		- 结构体内容：
			- sin_family：存放地址协议AF_INET或者PF_INET
			- struct in_addr sinaddr：存放要绑定的ip地址
			- sin_port：存放要绑定的端口号
		- addrlen：地址结构体的大小
- **注意**：
	- 宏定义INADDR_ANY在绑定的时候会帮助我们寻找本地主机中的任意ip地址帮我们绑定
### 字节序的转换
- **概念**：
	- 计算机网络中数据采用大端序存放：网络字节序
	- Linux中数据采用小端序存放：主机字节序
- bind函数绑定本地主机时将ip和端口号转为大端序in_addr_t inet_addr(const char*)
- accept成功之后打印客户端的ip和端口需要大端序转小端序char \*inet_ntoa(strut in_addr )
### 连接
int connect(int socket,const struct sockaddr \*address,socklen_t address_len)
- **参数**：address：存放服务器的ip和端口号