# 1.通用性分析
## 容器
- 所有的数据结构本质上都是一种容器
- 容器只关心数据之间的逻辑关系，并提供与之相对应的操作的集合
- 容器不关心数据本身的类型，存储的数据都被称为数据节点
## 通用性
- 容器提供的是数据处理的通用解决方案，不管处理什么数据对他们的操作都是统一的
- 用来获得通用性的方法：
	- 创建容器时由用户提供数据类型
	- 将数据从容器中剥离出去，容器只提供逻辑（Llinux内核链表）
## 通过第一种方法获得通用性
### 1. 节点的设计
```
// list.h

#ifndef DATATYPE
#define DATATYPE int
#endif

typedef DATATYPE datatype;

// 此处的节点是通用的
// 原理是将具体数据的类型让渡给用户自己去定义
typedef struct node
{
    datatype data;

    struct node *prev;
    struct node *next;
}listnode, *linklist;
```
 - 上述代码只能写在头文件中