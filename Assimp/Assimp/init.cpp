
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "camera.h"
#include "model.h"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
//���Ǳ������ڳ����д�����һ֡�����λ�ã����ǰ����ĳ�ʼֵ����Ϊ��Ļ�����ģ���Ļ�ĳߴ���800x600��
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

float fov = 45.0f;

// timing
/*
ͼ�γ������Ϸͨ�������һ��ʱ���(Deltatime)����������������Ⱦ��һ֡���õ�ʱ�䡣
���ǰ������ٶȶ�ȥ����deltaTimeֵ��������ǣ�������ǵ�deltaTime�ܴ󣬾���ζ����һ֡����Ⱦ�����˸���ʱ�䣬
������һ֡���ٶ���Ҫ��ø�����ƽ����Ⱦ����ȥ��ʱ�䡣
*/
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main()
{
	// glfw: initialize and configure
	/*
		���ȣ�������main�����е���glfwInit��������ʼ��GLFW��
		Ȼ�����ǿ���ʹ��glfwWindowHint����������GLFW��
		��������ڱ������cpp�ļ���õ������� undefined reference (δ���������)����
		Ҳ����δ˳��������GLFW�⡣
	*/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	/*
		����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile)�����Ҳ������û��������ڵĴ�С��
		����ȷ����GLFWʹ�ú���ģʽ������£�ʹ�þɰ溯�����ᵼ��invalid operation(��Ч����)�Ĵ���
	*/
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//���ú��������򴰿ڴ�С���� �仯ʱ��������Ⱦ���ڴ�С
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	// build and compile shaders
		// -------------------------
	Shader ourShader("model_loadingV.txt", "model_loadingF.txt");

	// load models
   // -----------
	Model ourModel("nanosuit/nanosuit.obj");

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per - frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		/*
		OpenGL�洢�������������Ϣ��һ��Z����(Z-buffer)�У�Ҳ����Ϊ��Ȼ���(Depth Buffer)��
		GLFW���Զ�Ϊ����������һ�����壨������Ҳ��һ����ɫ�������洢���ͼ�����ɫ����
		���ֵ�洢��ÿ��Ƭ�����棨��ΪƬ�ε�zֵ������Ƭ����Ҫ���������ɫʱ��OpenGL�Ὣ�������ֵ��z������бȽϣ������ǰ��Ƭ��������Ƭ��֮�������ᱻ���������򽫻Ḳ�ǡ�
		������̳�Ϊ��Ȳ���(Depth Testing)��������OpenGL�Զ���ɵġ�
		*/
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
/*
	�����ǣ��´����ģ�key_callback�����У����Ǽ���˼����Ƿ�����Escape����
	�������ȷ������(���ͷ�)��ʹ��glfwSetwindowShouldClose�����趨WindowShouldClose����Ϊtrue�Ӷ��ر�GLFW��
*/
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 0.05f; // adjust accordingly

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	//�������ϣ���������ƶ�������ʹ�ò��������һ��������(Right Vector)������������Ӧ�ƶ��Ϳ����ˡ�
	//�����ʹ�����ʹ�������ʱ��Ϥ�ĺ���(Strafe)Ч����
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
//���ڸı��Сʱ���޸��ӿ�
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//ʹ��һ��bool�������������Ƿ��ǵ�һ�λ�ȡ������룬����ǣ���ô�����Ȱ����ĳ�ʼλ�ø���Ϊxpos��yposֵ
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
/*���ǻ�����ʵ��һ������(Zoom)�ӿڡ�
��֮ǰ�Ľ̳�������˵��Ұ(Field of View)��fov���������ǿ��Կ��������ж��ķ�Χ��
����Ұ��Сʱ������ͶӰ�����Ŀռ�ͻ��С�������Ŵ�(Zoom In)�˵ĸо���
���ǻ�ʹ�����Ĺ������Ŵ�*/
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

