# 1.标准IO函数
## 文件的打开与关闭
- 系统IO打开文件得到的是一个整数，成为文件描述符，标准IO打开文件得到的是一个指针，称为文件指针
- 文件指针指向结构体FILE,该结构体包含了文件描述符
### 标准IO打开文件
- **功能**：获取指定文件的文件指针
- **原型**：FILE \*fopen(const char \*path,const \*mode)
- **参数**：
	- path:即将要打开的文件
	- mode：
		- "r"：以只读方式打开文件，要求文件必须存在
		- "r+"：以读写方式打开文件，要求文件必须存在
		- "w"：以只写方式打开文件，文件如果不存在就创建新文件，如果存在将会清空文件内容
		- "w+"：以读写方式打开文件，文件如果不存在将会创建新文件，如果存在将会将其内容清空
		- "a"：以只写方式打开文件，文件如果不存在将会创建新文件，且文件位置偏移量被自动定位到文件末尾，以追加方式写数据
		- "a+"：以读写方式打开文件，文件如果不存在将会创建新文件，且文件位置偏移量被自动定位到文件末尾，以追加的方式写数据
- **返回值**：
	- 成功：文件指针
	- 失败：NULL
### 标准IO关闭文件
- **功能**：关闭指定的文件并释放其资源
- **原型**：int fclose(FILE \*fp);
- **参数**：fp：即将要关闭的文件
- **返回值**：
	- 成功：0
	- 失败：EOF
- **注意**：fclose涉及内存释放，不可对同一个文件多次关闭
## 文件内容的读写操作
### 按字节读写文本文件
- **按字节读**
	- **功能**：获取指定文件的第一个字符
	- **原型**
		- int fgetc(FILE \*stream);
		- int getc(FILE \*stream);
		- int getxhar(void);
	- **参数**：stream：为文件指针
	- **返回值**：
		- 成功：读取到的字符
		- 失败：EOF
	- 当返回EOF时，文件stream可能已到达末尾，或者遇到错误
- **按字节写**
	- **功能**：将一个字符写入一个指定的文件
	- **原型**：
		- int fputc(int c,FILE \*stream);
		- int putc(int c,FILE \*stream);
		- int putchar(int c);
	- **参数**：
		- c：要写入的字符
		- stream：写入的文件指针
	- **返回值**：
		- 成功：写入的字符
		- EOF
- **关键点**：
	- fgetc与getc功能完全一样，区别是fgetc是函数getc是宏
	- fputc和putc的功能也完全一样，fputc是函数putc是宏
	- getchar和putchar只能针对键盘输入和屏幕输出，不能指定别的文件
### 按行读取文本文件
- **读取**
	- **功能**：从指定文件读取最多一行数据
	- **原型**：
		- char \*fgets(char \*s,int size,FILE \*stream);
		- char \*gets(char \*s);
	- **参数**：
		- s：自定义缓冲区指针
		- size：自定义缓冲区大小
		- stream：即将被读取数据的数据的文件指针
	- **返回值**：
		- 成功：自定义缓冲区指针s
		- 失败：NULL
	- gets()缺省从文件stdin读入数据
	- 当返回NULL时，文件stream可能已达末尾，或者遇到错误
- **写入**：
	- **功能**：将数据写入指定的文件
	- **原型**：
		- int fputs(const char \*s,FILE \*stream)；
		- int puts(const char \*s);
	- **参数**：
		- s：自定义缓冲区指针
		- stream：即将被写入数据的文件指针
	- **返回值**：
		- 成功：非负整数
		- 失败：EOF
	- puts缺省将数据写入文件stdout
- **关键点**：
	- 对于读操作，返回EOF意味着读操作失败，有两种情况：
		1. 如果feof(fp)为真，意味着读到了文件末尾，没有数据可读
		2. 如果ferror(fp)为真，意味着遇到了错误
	- fgets()和gets()的区别：
		- fgets()可以读取任意指定的文件，而gets()只能从键盘读取
		- fgets()内存在边界判断，而gets()没有，因此后者是不安全的，不建议使用
		- fgets()任何情况下都按原样读取数据，gets()会自动去除数据末尾的‘\n’
	- fputs()与puts()的区别：
		- fputs()可以将数据写入任意指定文件，而puts()只能将数据输出到屏幕
		- fputs()在任何情形下都按原样写入数据，puts()会给数据末尾加上'\n'
### 按指定格式读写文本文件
- **写文件**
	- **功能**：将格式化数据写入指定的文件或内存
	- **原型**：
		- int fprintf(FILE \*retrict stream,const char \*restrict format,...);
		- int printf(const char \*restrict format,...);
		- int snprintf(char \*restrict s,size_t n,const char \*restrict format,...);
		- int sprintf(char \*restrict s,const char \*restrict format,...);
	- **参数**
		- stream：写入数据的文件指针
		- format：格式控制串
		- s：写入数据的自定义缓冲
		- n：自定义缓冲区的大小
	- **返回值**
		- 成功：成功写入的字节数
		- 失败：-1
- **读文件**
	- **功能**：从指定的文件或内存中读取格式化数据
	- **原型**：
		- int fscanf(FILE \*restrict stream,const char \*restrict format,...);
		- int scanf(const char \*restrict format,...);
		- int sscanf(const char \*restrict s,const char \*restrict format,...);
	- **参数**：
		- stream：读出数据的文件指针
		- format：格式控制串
		- s：读出数据的自定义缓冲区
	- **返回值**
		- 成功：正确匹配且赋值的数据个数
		- 失败：EOF
- **关键点**：
	1. fprintf()不仅可以像printf()一样输出设备输出信息，也可以向有strea指定的任何有相应权限的文件写入数据
	2. sprintf()和snprintf()都是向一块自定义缓冲区写入数据，不同的是后者第二个参数提供了这块缓冲区的大小，避免缓冲区溢出，因此应尽量使用后者，放弃使用前者
	3. fscanf()不仅可以像scanf()一样从标准输入设备读入信息，也可以从任何由stream指定的有相应权限的文件读入数据
	4. sscanf()从一块由s指定的自定义缓冲区中读入数据
	5. 这些函数的读写都是带格式的
### 按数据块读写文件
- **读取文件**
	- **功能**：从指定文件中读取若干个数据块
	- **原型**：size_t fread(void \*ptr,size_t size,size_t nmemb,FILE \*stream);
	- **参数**：
		- ptr：自定义缓冲区指针
		- size：数据块大小
		- nmemb：数据块个数
		- stream：即将被读取数据的文件指针
	- **返回值**
		- 成功：读取的数据块个数
		- 失败：的读取的数据块个数小于nmnmb或等于0
- **写入文件**
	- **功能**：将若干数据块写入指定的文件
	- **原型**：size_t fread(void \*ptr,size_t size,size_t nmemb,FILE \*stream);
	- **参数**：
		- ptr：自定义缓冲区指针
		- size：数据块大小
		- nmemb：数据块个数
		- stream：即将被写入数据的文件指针
	- **返回值**：
		- 成功：写入的数据块个数
		- 失败：写入的数据块个数，小于nmemb或等于0
- **关键点**：
	- 这两个函数既能处理文本也能处理二进制文件
	- 返回值是块数而非字节数
## 文件位置的获取与设定
### 设置文件位置
- **功能**：设置指定文件的当前位置偏移量
- **原型**：int fseek(FILE \*stream,long offset,int whence);
- **参数**：
	- 