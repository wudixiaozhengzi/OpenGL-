#include <GL/glut.h> 
#include <math.h>
const GLfloat Pi = 3.1415926536f;
const GLfloat factor = 0.1f;
void myDisplay(void) {
	GLfloat x;
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINE);
	glVertex2f(-1.0f, 0.0f);
	glVertex2f(1.0f,0.0f);
	glVertex2f(0.0f, -1.0f);
	glVertex2f(0.0f, 1.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
	for (x = -1.0f / factor; x < 1.0f / factor; x += 0.01f)
	{
		glVertex2f(x*factor, sin(x)*factor);
	}
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
