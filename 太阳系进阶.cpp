#include "pch.h"
#include <GL/glut.h> 
#include <iostream>
#include <stdio.h> 
#include <time.h> 
const GLdouble Pi = 3.1415926536;

/*
	太阳半径69600km
	地球半径6378km
	月球半径1739km
	地球到太阳的距离约为1.5亿km=150000000km
	月亮到地球的距离约为		380000km
*/

/*修改不至于比例差距太大
	太阳半径69600000km（放大100倍）
	地球半径15945000km（放大 2500 倍）
	月球半径4345000km （放大 2500 倍）
	地球到太阳的距离约为1.5亿km=150000000km（保持不变）
	月亮到地球的距离约为	    38000000km（放大 100 倍）
*/

/*
	假设三个天体都是球体，都处于同一平面
	假设每个月都是30天，一年12个月，共有360天
	太阳中心为原点，天体轨迹所在的平面表示了 X 轴与 Y 轴决定的平面，
	且每年第一天，地球在 X 轴正方向上，月亮在地球的正 X 轴方向。
*/

static int day = 200;
#define WIDTH 400
#define HEIGHT 400
static GLfloat angle = 0.0f;

double CalFrequency() {
	static int count;
	static double save;
	static clock_t last, current;
	double timegap;
	++count;
	if (count <= 50)
		return save;
	count = 0;
	last = current;
	current = clock();
	timegap = (current - last) / (double)CLK_TCK;
	save = 50.0 / timegap;
	return save;
}

void myDisplay(void)
{
	double FPS = CalFrequency();
	printf("FPS = %f\n", FPS);
	 
	//glClear清除   GL_COLOR_BUFFER_BIT 表示清除颜色，GL_DEPTH_BUFFER_BIT表示清空深度缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/**************第一步确立可视空间*****************/
	//glMatrixMode(GL_PROJECTION);//操作投影矩阵
	//glLoadIdentity();	//将当前矩阵设置为单位矩阵
	////为了透视效果，使用gluPerspective设置可视空间，设定视角为75(可修改)
	////高宽比为1.0。最近可视距离为1.0。最远可视距离为200000000*2=400000000
	//gluPerspective(75, 1, 1, 400000000);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	///*
	//	gluLookAt();前三个参数表示了观察点的位置，中间表示了观察目标的参数，
	//	最后三个参数表示从（0,0,0）到（x,y,z）的直线，他表示观察者认为上的方向
	//	观察目标设置为太阳中心，选择Z轴正方向为上方向，将地球半径取整200000000
	//*/
	//gluLookAt(0, -200000000, 200000000, 0, 0, 0, 0, 0, 1);

	gluPerspective(90.0f, 1.0f, 1.0f, 20.0f);     
	glMatrixMode(GL_MODELVIEW);     
	glLoadIdentity();    
	gluLookAt(0.0, 5.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	/*************绘制红色的太阳*****************/
	
	//定义太阳光源，一种白色的光源
	{
		GLfloat sun_light_position[]={ 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		
		glLightfv(GL_LIGTH0, GL_POSITION, sun_light_position);
		glLightfv(GL_LIGTH0, GL_AMBIENT, sun_light_ambient);
		glLightfv(GL_LIGTH0, GL_DIFFUSE, sun_light_diffuse);
		glLightfv(GL_LIGTH0, GL_SPECULAR, sun_light_specular);

		glEnable(GL_LIGHT0);	//开启第 0 号光源
		glEnable(GL_LIGHTING);	//打开灯光处理
		glEnable(GL_DEPTH_TEST);
	}
	//定义太阳材质并绘制太阳
	{
		GLfloat sun_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
		GLfloat sun_mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };         
		GLfloat sun_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };         
		GLfloat sun_mat_emission[] = { 0.5f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_mat_shininess = 0.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
		glMaterial(GL_FRONT, GL_SHININESS, sun_mat_shininess);

	}
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(69600000, 20, 20);//球体半径，薄片数值，堆数值（后面两个都是描述球体精确度的）
	
									  
	/*************绘制蓝色的地球*****************/
	 // 定义地球的材质并绘制地球     
	{         
		GLfloat earth_mat_ambient[]  = {0.0f, 0.0f, 0.5f, 1.0f};         
		GLfloat earth_mat_diffuse[]  = {0.0f, 0.0f, 0.5f, 1.0f};        
		GLfloat earth_mat_specular[] = {0.0f, 0.0f, 1.0f, 1.0f}; 
		GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };         
		GLfloat earth_mat_shininess = 30.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);         
		glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);         
		glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);         
		glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);         
		glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);

		glColor3f(0.0f, 0.0f, 1.0f);
	//今年已经经过的天数已知为 day，则地球转过的角度为 day/一年的天数*360 度。 前面已经假定每年都是 360 天
	// 注意地球公转是“自西向东”的，因此是饶着 Z 轴负方向进行逆时针旋转 
	glRotatef(day / 360.0*360.0, 0.0f, 0.0f, -1.0f);
	//glTranslatef(地球轨道半径, 0, 0); 
	glTranslatef(150000000, 0.0f, 0.0f);
	glutSolidSphere(15945000, 20, 20);

	//绘制黄色的月亮
	glColor3f(1.0f, 1.0f, 0.0f);
	/*
		在绘制地球是，坐标已经旋转过的，现在的旋转是在以前的基础上进行旋转的，所以要处理差值
		可以在绘制地球前用glPushMatrix保存矩阵，绘制完地球后用glPopMatrix恢复矩阵。
		再设计一个跟地球位置无关的月亮位置公式绘制月亮
	*/
	glRotatef(day / 30.0*360.0 - day / 360.0*360.0, 0.0f, 0.0f, -1.0f);
	glTranslatef(38000000, 0.0f, 0.0f);
	glutSolidSphere(4345000, 20, 20);

	/* OpenGL 把三维坐标中的物体绘制到二维屏幕，绘制的顺序是按照代码的顺序来进行的。
		因此 后绘制的物体会遮住先绘制的物体，即使后绘制的物体在先绘制的物体的“后面”也是如此。*/
	glFlush();
	glutSwapBuffers();
}

void myIdle(void) {     /* 新的函数，在空闲时调用，作用是把日期往后移动一天并重新绘制，达到动画效果 */
	++day;
	if (day >= 360)
		day = 0;
	myDisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // 修改了参数为 GLUT_DOUBLE 双缓冲    
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("太阳，地球和月光照演示");   // 改了窗口标题     
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myIdle);               // 新加入了这句     
	glutMainLoop();
	return 0;
}