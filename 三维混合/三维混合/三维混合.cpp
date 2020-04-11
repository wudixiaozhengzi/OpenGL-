// 三维混合.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <GL/glut.h>
#include <iostream>

GLfloat angle = 0.0f;

//设置白色光源
void setLight(void)
{
	static const GLfloat light_position[] = { 1.0f,1.0f,-1.0f,1.0f };
	static const GLfloat light_ambient[] = { 0.2f,0.2f,0.2f,1.0f };
	static const GLfloat light_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
	static const GLfloat light_specular[] = { 1.0f,1.0f,1.0f,1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION,light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}
//每个球体颜色不同，所以他们的材质也不同；用函数设置材质
void setMartiral(const GLfloat mat_diffuse[4], GLfloat mat_shininess)
{
	static const GLfloat mat_specular[] = { 0.0f,0.0f,0.0f,1.0f };
	static const GLfloat mat_emission[] = { 0.0f,0.0f,0.0f,1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
}

void myDisplay(void)
{
	//定义三个球的材质颜色
	const static GLfloat red_color[] = { 1.0f,0.0f,0.0f,1.0f };
	const static GLfloat green_color[] = { 0.0f,1.0f,0.0f,0.333f };
	const static GLfloat blue_color[] = { 0.0f,0.0f,1.0f,0.5f };

	//清除屏幕
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//启动混合并设置混合因子
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);

	//设置光源
	setLight();

	//以(0,0,0.5)为中心，绘制一个半径为3的不透明红色球体（里离观察者最远）
	setMartiral(red_color, 30.0);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.5f);
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();

	//下面绘制半透明物体，因此将深度缓冲设置为只读
	glDepthMask(GL_FALSE);

	//以(0.2,0,-0.5)为中心，绘制一个半径为2 的半透明蓝色球体（离观察者近）
	setMartiral(blue_color, 30.0);
	glPushMatrix();
	glTranslated(0.2f, 0.0f, -0.5f);
	glutSolidSphere(0.2, 30, 30);
	glPopMatrix();

	//完成半透明物体的绘制，将深度缓冲区会服务可读可写的形式
	glDepthMask(GL_TRUE);

	glutSwapBuffers();
	glFlush();/*，保证前面的 OpenGL 命令立即执行（而不是让它们在缓冲区中等待）。其作用跟 fflush(stdout)类似。 */
}

void myIdle(void)
{
	++angle;
	if (angle >= 360.0f)
		angle = 0.0f;
	myDisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);		/*glutInit，对 GLUT 进行初始化，这个函数必须在其它的 GLUT 使用之前调用一次。 */
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	/*设置显示方式，GLUT_RGB 表示使用 RGB 颜色，
													 与之对应的还有 GLUT_INDEX（表示 使用索引颜色）。
													 GLUT_SINGLE 表示使用单缓冲，
													 与之对应的还有 GLUT_DOUBLE（使用双缓冲）。*/
	glutInitWindowPosition(100, 100);	/*设置窗口在屏幕中的位置*/
	glutInitWindowSize(400, 400);		/*设置窗口的大小*/
	glutCreateWindow("混合");			/*根据前面设置的信息创建窗口。参数将被作为窗口的标题。
					注意：窗口被创建后，并不立即显示到屏幕上。需要调用 glutMainLoop 才能看到窗口。*/
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myIdle);
	glutMainLoop();		 /*进行一个消息循环*/
	return 0;
}