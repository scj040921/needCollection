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
 - 上述代码只能写在头文件中：这种写法主要依靠**预处理的文本替换**，如果写在.c源文件中，DATATYPE的类型会在编译时就被写死，用户无法更改。
 - 用户使用该容器的时候，定义DATATYPE为其需要的类型
```
// 注意以下内容必须放在头文件 list.h 中

// 初始化空链表，与用户实际数据无关
static node * initList()
{
    node * head = (node *)malloc(sizeof(node));

    if(head != 0)
    {
        head->prev = head;
        head->next = head;
    }

    return head;
}

// 判断链表是否为空，与用户实际数据无关
static bool isEmpty(node *head)
{
    return head->next == head;
}
```
- 通用型算法都只能写在头文件中，因为编译时datatype必须结合用户提供的.c源文件才能确定确切的数据类型
- 如果用户需要使用链表容器处理特定数据，就在包含头文件前自定义宏
- 为防止头文件被多个c文件包含而造成函数冲突，头文件中的函数必须定义为静态函数。
### 2. 增删操作
增删操作都是需要用户具体数据的操作
```
// 根据用户提供的数据，产生一个新节点
static linklist __newNode(datatype *newData)
{
    linklist new = malloc(sizeof(listnode));
    if(new != NULL)
    {
        new->data = *newData;
        new->prev = new;
        new->next = new;
    }

    return new;
}

// 将新节点new插入到链表的首部
void listAdd(linklist head, datatype *newdata)
{
    linklist new = __newNode(newdata);

    new->prev = head;
    new->next = head->next;

    head->next->prev = new;
    head->next = new;
}

// 将新节点new插入到链表的尾部
void listAddTail(linklist head, datatype *newdata)
{
    linklist new = __newNode(newdata);

    new->prev = head->prev;
    new->next = head;

    head->prev->next = new;
    head->prev = new;
}

// 将指定节点从链表中剔除出去
bool listDel(linklist p)
{
    if(p==NULL || isEmpty(p))
        return false;

    // 将原链表首节点剔除出链表
    p->prev->next = p->next;
    p->next->prev = p->prev;
    p->prev = p;
    p->next = p;

    return true;
}
```
  - 不需要对外提供函数接口
### 3. 查找节点
- 查找节点需要对节点间进行比对，比对过程与数据本身密切相关
- 查找节点时节点的判定接口必须由用户提供，链表只提供回调接口
```
// 查找指定的节点，并使用用户提供的钩子函数 equal 来判定节点是否相同
linklist find(linklist head, datatype data,
                bool (*equal)(datatype, datatype))
{
    for(linklist tmp=head->next; tmp!=head; tmp=tmp->next)
    {
        if(equal(tmp->data, data))
            return tmp;
    }
    return NULL;
}
```
### 4. 遍历链表
```
void listForEach(linklist head, void (*handle)(datatype *))
{
    if(isEmpty(head))
        return;

    for(linklist tmp=head->next; tmp!=head; tmp=tmp->next)
        handle(&tmp->data);
}
```
# 2. 内核链表
## 普通链表的弊端
- 不具有通用性，更换数据要全部重写
- 操作链表的函数只能操作指定的参数类型
- 链表的指针指向的是整个节点，而这些节点都是各异的，因此这些指针也都是各异的
### 解决思路：
1. 把链表指针和数据分开
2. 将链表逻辑单独抽出来，去掉节点内的具体数据，让节点只包含双向指针
3. 把不含任何数据的链表镶嵌在具体要串起来的数据节点之中
## 内核链表
- 解决通用性问题就两点：
	1. 设计标准节点
	2. 针对标准节点，设计由标准节点构成的标准链表的所有操作
- 内核链表的标准节点及其所有操作都被封装在内核源码中，包含两部分内容内核链表和哈希链表
### 节点设计
- 标准节点就是不包含任何数据的双向链表节点
- 一般将用户定义的数据节点称为大结构体，标准节点称为小结构体
- 标准节点的用法就是向前在用户定义的大结构体中
### 初始化
- 内核链表的初始化与普通的双向链表无异，就是简单的让前后指针指向自身
- 实际应用中采用标准节点的内核链表都是带头节点的
### 大小结构体转换
- 内核链表的操作都只针对标准的小结构体，与包裹他的大结构体无关，所以用户想要操作数据就需要进行大小结构体的转换
- 通过计算小结构体相对大结构体的地址偏移量，再用指向小结构体的指针减去偏移量就可以的到指向大结构体的指针
```
#define list_entry(ptr, type, member) \
((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
```
### 插入节点
- 内核链表的标准操作中提供了头插法和尾插法
```
// 内部函数
// 将节点new插入到prev与next之间
// 注意，所有的指针都是标准节点指针，与用户数据无关
static inline void __list_add(struct list_head *new,
				struct list_head *prev,
				struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

// 将新节点new插入到链表head的首部
// 即:插入到head的后面
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

// 将新节点new插入到链表head的尾部
// 即:插入到head的前面
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}
```
- 实际应用中，用户针对大结构体中的标准节点进行操作
### 遍历链表
- **向后遍历链表每一个节点**(遍历过程不可删除节点)：
```
#define list_for_each(pos, head) \
for (pos = (head)->next; pos != (head); \
pos = pos->next)
```
- **安全向后遍历每一个节点**（支持边遍历边删除节点）：
```
#define list_for_each_safe(pos, n, head) \
for (pos = (head)->next, n = pos->next; pos != (head); \
pos = n, n = pos->next)
```