// 抓取BMP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include "pch.h"
#include <gl/glut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define BMP_Header_Length 54
#define WindowWidth 400
#define WindowHeight 400

int main(int argc, char *argv[])
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutInit(&argc, argv);		/*、glutInit，对 GLUT 进行初始化，这个函数必须在其它的 GLUT 使用之前调用一次。 */
	//	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	
	/*设置显示方式，GLUT_RGB 表示使用 RGB 颜色，与之对应的还有 GLUT_INDEX（表示 使用索引颜色）。
	   GLUT_SINGLE 表示使用与之对应的还有 GLUT_DOUBLE（使用双缓冲）。*/
	
	FILE* pDummyFile;
	FILE* pWritingFile;
	GLubyte* pPixelData;
	GLubyte BMP_Header[BMP_Header_Length];
	GLint i, j;
	GLint PixelDataLength;
	errno_t err1,err2;

	/***********************计算pixel像素数据的实际长度*****************************/
	i = WindowWidth * 3; //得到每一行的像素数据长度
	while (i % 4 != 0) //补充数据，直到i是4的倍数
		++i; 
	PixelDataLength = i * WindowHeight;

	/***********************分配内存和打开文件*****************************/
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);
	err1=fopen_s(&pDummyFile,"C:\\Users\\15081\\Desktop\\dummy.bmp","rb");
	//pDummyFile=fopen("dummy.bmp", "rb");
	if (pDummyFile == 0)	exit(0);

	err2 = fopen_s(&pWritingFile,"C:\\Users\\15081\\Desktop\\grab.bmp","wb");
	//pWritingFile=fopen("grab.bmp", "wb");
	if (pWritingFile == 0)	exit(0);

	/*******************************读取并写入像素************************/
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, WindowWidth, WindowHeight,GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

	// 读取 dummy.bmp 的文件头54个字节到数组
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
	
	//把数组内容写入到新的BMP文件
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET);

	//修改其中大小信息
	i = WindowWidth;
	j = WindowHeight;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);

	// 移动到文件末尾，写入像素数据
	fseek(pWritingFile, 0, SEEK_END);
	fwrite(pPixelData, PixelDataLength,1,pWritingFile);
	
	// 释放内存和关闭文件
	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
	glutMainLoop();		 /*进行一个消息循环*/
	return 0;
}
//int main(int argc, char *argv[])
//{
//	glutInit(&argc, argv);		/*、glutInit，对 GLUT 进行初始化，这个函数必须在其它的 GLUT 使用之前调用一次。 */
//	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	/*设置显示方式，GLUT_RGB 表示使用 RGB 颜色，
//													 与之对应的还有 GLUT_INDEX（表示 使用索引颜色）。
//													 GLUT_SINGLE 表示使用单缓冲，
//													 与之对应的还有 GLUT_DOUBLE（使用双缓冲）。*/
//	glutInitWindowPosition(100, 100);	/*设置窗口在屏幕中的位置*/
//	glutInitWindowSize(400, 400);		/*设置窗口的大小*/
//	glutCreateWindow("混合");			/*根据前面设置的信息创建窗口。参数将被作为窗口的标题。
//					注意：窗口被创建后，并不立即显示到屏幕上。需要调用 glutMainLoop 才能看到窗口。*/
//	glutDisplayFunc(&myDisplay);
//	glutMainLoop();		 /*进行一个消息循环*/
//	return 0;
//}
