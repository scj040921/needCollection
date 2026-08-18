# 1.液晶屏的操作
## 基本概念
- **像素**：屏幕上显示颜色的最小单位，英文位pixel
- **分辨率**
	- 宽、高两个维度上的像素点数目
	- 分辨率越高需要的显存越大
- **色深**：
	- 每个像素所对应的内存字节数
	- 32位色深的屏幕一般被称为真彩屏
	- 色深决定了一个像素点所能表达的颜色的丰富程度，色深越大，色彩表现力越强
## 内存映射的基本原理
- 为LCD做内存映射，是将屏幕的每一个像素点跟映射内存一一对应，映射内存是二维数组，可以通过方便的操作二维数组中的元素来操作屏幕中的像素点。映射内存可以称为显存
- **关键点**：
	- LCD上显示的图像色彩，由其对应的内存数据决定
	- 映射内存的大小至少等于LCD真实尺寸的大小
	- 映射内存的大小可以大于LCD的真实尺寸，有利于优化动态画面体验
# 2.BMP格式图片处理
## BMP格式简介
- 无压缩的RGB数据封装格式，数据读出来就可以直接使用，不需要额外的解码
### BMP格式头
- BMP格式头中包含了以下三个结构体：
	- bitmap_header
	- bitmap_info
	- rgb_quad
```
struct bitmap_header
{
	int16_t type;
	int32_t size; // 图像文件大小
	int16_t reserved1;
	int16_t reserved2;
	int32_t offbits; // bmp图像数据偏移量
}__attribute__((packed));

struct bitmap_info
{
	int32_t size;   // 本结构大小	
	int32_t width;  // 图像宽
	int32_t height; // 图像高
	int16_t planes;

	int16_t bit_count; // 色深
	int32_t compression;
	int32_t size_img; // BMP文件中的RGB数据大小，必须是4的整数倍
	int32_t X_pel;
	int32_t Y_pel;
	int32_t clrused;
	int32_t clrImportant;
}__attribute__((packed));

// 以下结构体不一定存在于BMP文件中，除非：
// bitmap_info.compression为真
struct rgb_quad
{
	int8_t blue;
	int8_t green;
	int8_t red;
	int8_t reserved;
}__attribute__((packed));
```
### 特殊规则
- 每行数据的字节数必须是4的倍数，如果不是四的倍数就需要补齐
- 补齐后会导致出现无效字节，这些无效字节需要在处理图片时跳过
- BMP图片中的RGB数据是上下颠倒的，因此处理数据时要从最后一行的第一个字节开始
# 3.JPG格式图片处理
