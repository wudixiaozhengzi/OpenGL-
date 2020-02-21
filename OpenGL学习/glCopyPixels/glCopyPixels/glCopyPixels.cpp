// glCopyPixels.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h> 

#define WindowWidth 400
#define WindowHeight 400
#define FileName "dummy.bmp"
static GLint PixelLength;
static GLubyte* PixelData;

/* 函数 grab
* 抓取窗口中的像素
* 假设窗口宽度为 WindowWidth，高度为 WindowHeight
*/
#define BMP_Header_Length 54
void grab(void)
{
	FILE* pDummyFile;
	FILE* pWritingFile;
	GLubyte* pPixelData;
	GLubyte BMP_Header[BMP_Header_Length];
	GLint i, j;
	GLint PixelDataLength;
	errno_t err1, err2;

	// 计算像素数据的实际长度
	i = WindowWidth * 3; // 得到每一行的像素数据长度
	while (i % 4 != 0) // 补充数据，直到 i 是的倍数
		++i;

	PixelDataLength = i * WindowHeight;
	// 分配内存和打开文件
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)	exit(0);
	 
	err1= fopen_s(&pDummyFile, FileName, "rb");
	if (pDummyFile == 0)	exit(0);
	err2 = fopen_s(&pWritingFile,"C:\\Users\\15081\\Desktop\\grab.bmp", "wb");
	if (pWritingFile == 0) 	exit(0);

	// 读取像素
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, WindowWidth, WindowHeight,GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

	// 把 dummy.bmp 的文件头复制为新文件的文件头
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET);
	i = WindowWidth;
	j = WindowHeight;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);

	// 写入像素数据
	fseek(pWritingFile, 0, SEEK_END);
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

	 //释放内存和关闭文件
	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
}
void display(void)
{
	// 清除屏幕
	glClear(GL_COLOR_BUFFER_BIT);
	// 绘制三角形
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(0.5f, 1.0f);
	glEnd();
	glPixelZoom(-0.5f, -0.5f); //并同时进行水平和垂直方向的翻转，然后缩小为原来的一半
	glRasterPos2i(1, 1);
	glCopyPixels(WindowWidth / 2, WindowHeight / 2,WindowWidth / 2, WindowHeight / 2, GL_COLOR);
	// 完成绘制，并抓取图象保存为 BMP 文件
	glutSwapBuffers();
	grab();
}

int main(int argc, char* argv[])
{
	// 初始化 GLUT 并运行
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WindowWidth,WindowHeight);
	glutCreateWindow(FileName);
	glutDisplayFunc(&display);
	glutMainLoop();
	// 释放内存
	// 实际上，glutMainLoop 函数永远不会返回，这里也永远不会到达
	// 这里写释放内存只是出于一种个人习惯
	// 不用担心内存无法释放。在程序结束时操作系统会自动回收所有内存
	free(PixelData);
	return 0;
}
