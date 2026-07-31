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