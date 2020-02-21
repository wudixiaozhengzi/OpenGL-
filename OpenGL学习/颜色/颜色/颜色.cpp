// 颜色.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <GL/glut.h> 
#include <iostream>
const GLdouble Pi = 3.1415926536;

void myDisplay(void) {
	int i;
	//glShadeModel(GL_FLAT);//单色方式
	glShadeModel(GL_SMOOTH);//平滑方式，默认方式
	glClear(GL_COLOR_BUFFER_BIT);//glClear，清除。GL_COLOR_BUFFER_BIT 表示清除颜色
	glBegin(GL_TRIANGLE_FAN);		//GL_TRIANLE_FAN绘制扇形
	glColor3f(0.0f, 1.0f, 1.0f);//设置颜色
	glVertex2f(0.0f, 0.0f);	//指定顶点
	for (i = 0; i < 8; ++i)
	{
		glColor3f(i & 0x04, i & 0x02, i & 0x01);
		glVertex2f(cos(i*Pi / 4), sin(i*Pi / 4));
	}
	glEnd();
	glFlush();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);    /*、glutInit，对 GLUT 进行初始化，这个函数必须在其它的 GLUT 使用之前调用一次。
							  其格式比较死板，一般照抄这句 glutInit(&argc, argv)就可以了。 */
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);     /*设置显示方式，其中 GLUT_RGB 表示使用 RGB 颜色，
													 与之对应的还有 GLUT_INDEX（表示 使用索引颜色）。
													 GLUT_SINGLE 表示使用单缓冲，
													 与之对应的还有 GLUT_DOUBLE（使用双缓冲）。*/
	glutInitWindowPosition(100, 100);     /*设置窗口在屏幕中的位置*/
	glutInitWindowSize(400, 400);     /*设置窗口的大小*/
	glutCreateWindow("颜色");     /*根据前面设置的信息创建窗口。参数将被作为窗口的标题。
										   注意：窗口被创建后，并不立即显示到屏幕上。需要调用 glutMainLoop 才能看到窗口。*/
	glutDisplayFunc(&myDisplay);
	glutMainLoop();     /*进行一个消息循环*/
	return 0;
}