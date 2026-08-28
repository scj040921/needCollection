# 01.消息队列
## IPC对象概述
- 在系统中使用所谓键值（key）来唯一确定，类似于文件系统中的文件路径
- 当某个进程创建（或打开）一个IPC对象时，将会获得一个整型ID，类似于文件描述符
- IPC对象属于系统，而不是进程，因此在没有明确的删除操作的情况下，IPC对象不会因为进程的退出而消失
## 相关命令
### 查看IPC对象
- 查看系统当前所有IPC对象
	ipcs -a
- 查看系统当前所有消息队列对象
	ipcs -q
- 查看系统当前所有共享内存对象
	ipcs -m
- 查看系统当前所有信号量组对象
	ipcs -s
### 删除IPC对象
ipcrm -Q key : 删除指定的消息队列
ipcrm -q id : 删除指定的消息队列

ipcrm -M key : 删除指定的共享内存
ipcrm -m id： 删除指定的共享内存

ipcrm -S key : 删除指定的信号量
ipcrm -s id： 删除指定的信号量

## 消息队列
### 基本逻辑
- 具有相同类型的数据在消息队列内部排队，读取的时候也要指定类型，然后依次读出数据，让消息队列更类似于一个多管道集合
- 由于每个消息都带有类型，相同的类型组成一队，因此读取方可以通过类型来选择不同的队列，也因此消息队列适用于多对一的场景
### 应用场景
- 作为读写不同步的两个进程间的缓冲区
- 消息队列实现了对消息发送方和消息接收方的解耦，使双方可以异步的处理数据
### 函数接口
#### 创建或打开MSG对象
- int msgget(key_t key,int msgflg);
- 接口说明：
	- 返回值：消息队列MSG对象ID
	- 参数key：键值，全局唯一标识，可由ftok()产生
	- 参数msgflg：操作模式与读写权限
- key实质上就是一个整数
- key_t ftok(const char \*pathname,int proj_id);
- 对于ftok函数参数，路径和序号一样时产生的key也是一样的
- ftok()函数参数中的路径仅仅是产生键值key的参数，与实际文件系统并无关系。
- 若 msgget() 中的key写成 IPC_PRIVATE，那意味着新建一个私有的IPC对象，该对象只在本进程内部可见，与外部的系统MSG对象不会冲突。
#### 向MSG对象发送消息
- int msgsnd(int msqid,const void \*msgp,size_t msgsz,int msgflg);
- 接口说明：
	- msgqid：MSG对象的ID，由msgget()获取
	- msgp：一个指向等待被发送的消息的指针，由于MSG中的消息最大的特点是必须有一个整数标识，用以区分MSG中的不同的消息，因此MSG的消息会使用一个特别的结构体来表达，具体如下所示：
	```
	struct msgbuf
	{
	// 消息类型（固定）
	long mtype;
	
	// 消息正文（可变）
	// ...
	};
	```
	- msgsz：消息正文的长度，不含类型长度
	- msgflg：发送选项：
		-  0：默认发送模式，在MSG缓冲区已满的情况下阻塞，直到缓冲区可用
		- IPC_NOWAIT：非阻塞发送模式，在MSG缓冲区已满的情形下直接退出函数并设置错误码为EAGAIN.
#### 从MSG对象接收消息
- ssize_t msgrcv(int msqid, void \*msgp, size_t msgsz, long msgtyp, int msgflg);
- 接口说明：
	- msgqid：MSG对象的ID，由msgget()获取。
	- msgp：存放消息的内存入口。
	- msgsz：存放消息的内存大小。
	- msgtyp：欲接收消息的类型：
	    - 0：不区分类型，直接读取MSG中的第一个消息。
	    - 大于0：读取类型为指定msgtyp的第一个消息（若msgflg被配置了MSG_EXCEPT则读取除了类型为msgtyp的第一个消息）。
	    - 小于0：读取类型小于等于msgtyp绝对值的第一个具有最小类型的消息。例如当MSG对象中有类型为3、1、5类型消息若干条，当msgtyp为-3时，类型为1的第一个消息将被读取。
	- msgflg：接收选项：
	    - 0：默认接收模式，在MSG中无指定类型消息时阻塞。
	    - IPC_NOWAIT：非阻塞接收模式，在MSG中无指定类型消息时直接退出函数并设置错误码为ENOMSG.
	    - MSG_EXCEPT：读取除msgtyp之外的第一个消息。
	    - MSG_NOERROR：如果待读取的消息尺寸比msgsz大，直接切割消息并返回msgsz部分，读不下的部分直接丢弃。若没有设置该项，则函数将出错返回并设置错误码为E2BIG。
#### 其它操作
- PC对象是一种持久性资源，如果没有明确的删除掉他们，他们是不会自动从内存中消失的，除了可以使用命令的方式删除，可以使用函数来删除
- int msgctl(int msqid, int cmd, struct msqid_ds \*buf);
- 接口说明：
	- msqid：MSG对象ID
- cmd：控制命令字
    - IPC_STAT：获取该MSG的信息，储存在结构体msqid_ds中
    - IPC_SET：设置该MSG的信息，储存在结构体msqid_ds
    - IPC_RMID：立即删除该MSG，并且唤醒所有阻塞在该MSG上的进程，同时忽略第三个参数
