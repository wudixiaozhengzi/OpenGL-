#include "pch.h"
#include <GL/glut.h>


void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glColor4f(1, 0, 0, 0.5);
	glRectf(-1, -1, 0.5, 0.5);
	glColor4f(0, 1, 0, 0.5);
	glRectf(-0.5, -0.5, 1, 1);

	glutSwapBuffers();
	
	glFlush();/*，保证前面的 OpenGL 命令立即执行（而不是让它们在缓冲区中等待）。其作用跟 fflush(stdout)类似。 */
}

int main(int argc,char *argv[])
{
	glutInit(&argc,argv);		/*、glutInit，对 GLUT 进行初始化，这个函数必须在其它的 GLUT 使用之前调用一次。 */
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);	/*设置显示方式，GLUT_RGB 表示使用 RGB 颜色，
													 与之对应的还有 GLUT_INDEX（表示 使用索引颜色）。
													 GLUT_SINGLE 表示使用单缓冲，
													 与之对应的还有 GLUT_DOUBLE（使用双缓冲）。*/
	glutInitWindowPosition(100, 100);	/*设置窗口在屏幕中的位置*/
	glutInitWindowSize(400, 400);		/*设置窗口的大小*/
	glutCreateWindow("混合");			/*根据前面设置的信息创建窗口。参数将被作为窗口的标题。
					注意：窗口被创建后，并不立即显示到屏幕上。需要调用 glutMainLoop 才能看到窗口。*/
	glutDisplayFunc(&myDisplay);
	glutMainLoop();		 /*进行一个消息循环*/
	return 0;
}

