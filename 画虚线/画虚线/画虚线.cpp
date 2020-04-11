#include <GL/glut.h> 
#include <math.h>
const GLfloat Pi = 3.1415926536f;
const GLfloat factor = 0.1f;
void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_LINE_STIPPLE);/*启动虚线模式，用glDisable(GL_LINE_STIPPLE)可以关闭*/
	glLineStipple(2, 0x0FF0F);	/*设置虚线*/
	glLineWidth(10.0f);/*设置直线的宽度*/
	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.5f, 0.0f);
	glEnd();
	glFlush();
}


int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("绘制点线面");
	glutDisplayFunc(&myDisplay);
	glutMainLoop();
	return 0;
}
