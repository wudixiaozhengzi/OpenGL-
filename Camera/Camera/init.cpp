#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "camera.h"
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

	/*
		glfwWindowHint�����ĵ�һ����������ѡ������ƣ����ǿ��ԴӺܶ���GLFW_��ͷ��ö��ֵ��ѡ��
		�ڶ�����������һ�����Σ������������ѡ���ֵ��
		��Ҫ����GLFW����Ҫʹ�õ�OpenGL�汾��3.3������GLFW���ڴ���OpenGL������ʱ�����ʵ��ĵ�����
		��Ҳ����ȷ���û���û���ʵ���OpenGL�汾֧�ֵ�������޷����С�
		���ǽ����汾��(Major)�ʹΰ汾��(Minor)����Ϊ3��
	*/
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

	// build and compile our shader program
	// ------------------------------------
	Shader ourShader("vShaderFile.txt","fshaderFile.txt");
	
	//��һ����������
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		  glm::vec3(0.0f,  0.0f,  0.0f),
		  glm::vec3(2.0f,  5.0f, -15.0f),
		  glm::vec3(-1.5f, -2.2f, -2.5f),
		  glm::vec3(-3.8f, -2.0f, -12.3f),
		  glm::vec3(2.4f, -0.4f, -3.5f),
		  glm::vec3(-1.7f,  3.0f, -7.5f),
		  glm::vec3(1.3f, -2.0f, -2.5f),
		  glm::vec3(1.5f,  2.0f, -2.5f),
		  glm::vec3(1.5f,  0.2f, -1.5f),
		  glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	//�ڶ������ö��㻺�����VBO�������������VAO�������������EBO
	unsigned int VBO, VAO, EBO;
	/*
		//ѧϰ��https://www.cnblogs.com/keguniang/p/9859057.html
		ÿ�����ǻ���һ��������ʱ��������Ҫ�ظ�ͬ���Ĺ��������Ȱ󶨻�������Ȼ�����ö������ԣ���
		����Ҫ���Ƶ�����ܶ�ʱ��������̾��Ե���Щ��ʱ����ô������û��һ�ַ�ʽ������һ�����أ�
		�����VAO�������飬�������ж�����ƹ����е���Щ���úͰ󶨹��̼��д洢��һ��
		��������Ҫʱ��ֻ��Ҫʹ����Ӧ��VAO���ɡ�
		VAO�����ַ�ʽ�е���һ���н飬�����з����İ󶨺Ͷ������ù�����������������
		������Ҫ����ʱ��ֱ��������н�ͺ��ˡ�

		����ԡ����㡱���ԣ�Ҳ����˵����������Ļ��ơ�ϢϢ��غ� ������û���κι�ϵ��VAO�������Ϊһ��״̬��������¼VBO��״̬��
		ͨ��VAO���ܿ��ٷ��ʵ�VBO��״̬��VBO�����ݡ�
		VAO��¼����һ�λ���������Ҫ����Ϣ�������������������glBindBuffer����
		�����ݵĸ�ʽ����ô����glVertexAttribPointer����

		VAO��һ�����������ж����������Ե�״̬��ϣ����洢�˶������ݵĸ�ʽ�Լ��������������VBO��������á�
		VAO����û�д洢���������������ݣ���Щ��Ϣ�Ǵ洢��VBO�еģ�
		VAO�൱���ǶԺܶ��VBO�����ã���һЩVBO�����һ����Ϊһ������ͳһ����

		�ڴ���VAO֮����Ҫʹ��glBindVertexArray������Ϊ��ǰ������VAO,
		֮�����ǹ��ڶ������ݵ����ã���������ʹ�õ�VBO���󣬶�����������õ���Ϣ���ᱻ�洢��VAO֮�У���
		����˵VBO�ǶԶ���������Ϣ�İ󶨣�VAO�ǶԺܶ��VBO�İ󶨡�
	*/
	//ͨ��glGenBuffers����,����һ������ID��һ�����㻺�����VBO��ͨ��glGenTextures()�����������
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	//��ʼ�������������ͨ��glGenVertexArrays()���䶥���������
	glGenVertexArrays(1, &VAO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	//���Ȱ󶨶����������Ȼ��󶨺����ö��㻺������Ȼ�����ö������ԡ�
	//���õ������������֮�����󶨵�OpenGL�����Ա�ʹ�á�
	glBindVertexArray(VAO);
	//ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	//֮������ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/*
		glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ�����
		���ĵ�һ��������Ŀ�껺������ͣ����㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ���ϡ�
		�ڶ�������ָ���������ݵĴ�С(���ֽ�Ϊ��λ)����һ���򵥵�sizeof������������ݴ�С���С�
		����������������ϣ�����͵�ʵ�����ݡ�
		���ĸ�����ָ��������ϣ���Կ���ι�����������ݡ�����������ʽ��
			GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
			GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
			GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//��VBO���ƣ��Ȱ��������ڰ��������Ƶ������С�
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//�ڰ˲������Ӷ�������
	//���ö�������ָ��
	/*
		������ɫ����������ָ���κ��Զ�������Ϊ��ʽ�����롣
		��ʹ����к�ǿ������Ե�ͬʱ��
		������ȷ��ζ�����Ǳ����ֶ�ָ���������ݵ���һ�����ֶ�Ӧ������ɫ������һ���������ԡ�
		****ʹ��glVertexAttribPointer()��������OpenGLֱ�Ӵ��ڴ��л�ȡ����

		glVertexAttribPointer�����Ĳ���
			��һ������ָ������Ҫ���õĶ������ԡ�
				���ǵ������ڶ�����ɫ����ʹ��layout(location = 0)������position�������Ե�λ��ֵ(Location)�������԰Ѷ������Ե�λ��ֵ����Ϊ0����Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��
			�ڶ�������ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
			����������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)��
			���ĸ��������������Ƿ�ϣ�����ݱ���׼��(Normalize)��
				�����������ΪGL_TRUE���������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣���ǰ�������ΪGL_FALSE��
			�����������������(Stride)�������������������Ķ���������֮��ļ����
				�����¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)��
				Ҫע�������������֪����������ǽ������еģ���������������֮��û�п�϶������Ҳ��������Ϊ0����OpenGL�������岽���Ƕ��٣�ֻ�е���ֵ�ǽ�������ʱ�ſ��ã���
				һ�������и���Ķ������ԣ����Ǿͱ����С�ĵض���ÿ����������֮��ļ���������ں���ῴ�����������
				�����������˼��˵���Ǵ�������Եڶ��γ��ֵĵط�����������0λ��֮���ж����ֽڣ���
			���һ��������������void*��������Ҫ���ǽ��������ֵ�ǿ������ת��������ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)��
				����λ������������Ŀ�ͷ������������0��
	*/
	//λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray���Զ�������λ��ֵ��Ϊ���������ö������ԣ�
	glEnableVertexAttribArray(0);
	//// ��ɫ����
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//��������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//�Ѱ������ζ���������󣬿��Խ�󶥵��������VAO
	glBindVertexArray(0);

	stbi_set_flip_vertically_on_load(true);
	// load and create a texture 
	// -------------------------
	unsigned int texture1,texture2;
	// ���ز���������texture1
	//glGenTextures����������Ҫ�������������������Ȼ������Ǵ����ڵڶ�������
	glGenTextures(1, &texture1);
	//���õ�ǰ�󶨵�����
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	/*
	ʹ��glTexParameter*������һ������ָ��������Ŀ�ꣻ
	����ʹ�õ���2D�����������Ŀ����GL_TEXTURE_2D��
	�ڶ���������Ҫ����ָ�����õ�ѡ����Ӧ�õ������ᡣ���Ǵ������õ���WRAPѡ�����ָ��S��T�ᡣ
	���һ��������Ҫ���Ǵ���һ�����Ʒ�ʽ(Wrapping)

		���Ʒ�ʽ	����
		GL_REPEAT	�������Ĭ����Ϊ���ظ�����ͼ��
		GL_MIRRORED_REPEAT	��GL_REPEATһ������ÿ���ظ�ͼƬ�Ǿ�����õġ�
		GL_CLAMP_TO_EDGE	��������ᱻԼ����0��1֮�䣬�����Ĳ��ֻ��ظ���������ı�Ե������һ�ֱ�Ե�������Ч����
		GL_CLAMP_TO_BORDER	����������Ϊ�û�ָ���ı�Ե��ɫ��
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/*
	�����зŴ�(Magnify)����С(Minify)������ʱ���������������˵�ѡ������������������С��ʱ��ʹ���ڽ����ˣ����Ŵ�ʱʹ�����Թ��ˡ�
	������Ҫʹ��glTexParameter*����Ϊ�Ŵ����Сָ�����˷�ʽ��
		GL_NEAREST��Ҳ���ڽ����ˣ�Nearest Neighbor Filtering����OpenGLĬ�ϵ�������˷�ʽ��
		������ΪGL_NEAREST��ʱ��OpenGL��ѡ�����ĵ���ӽ�����������Ǹ����ء�
		GL_NEAREST�����˿���״��ͼ���������ܹ��������������������أ�
		GL_LINEAR��Ҳ�����Թ��ˣ�(Bi)linear Filtering����������������긽�����������أ������һ����ֵ�����Ƴ���Щ��������֮�����ɫ��
		һ���������ص����ľ�����������Խ������ô����������ص���ɫ�����յ�������ɫ�Ĺ���Խ��
		��GL_LINEAR�ܹ�������ƽ����ͼ�������ѿ����������������ء�GL_LINEAR���Բ�������ʵ�����
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	//stb_image.h������ͼ��Ŀ�ȡ��߶Ⱥ���ɫͨ���ĸ����������������
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		/*
		glTexImage2D()
		��һ������ָ��������Ŀ��(Target)������ΪGL_TEXTURE_2D��ζ�Ż������뵱ǰ�󶨵����������ͬһ��Ŀ���ϵ������κΰ󶨵�GL_TEXTURE_1D��GL_TEXTURE_3D���������ܵ�Ӱ�죩��
		�ڶ�������Ϊ����ָ���༶��Զ����ļ��������ϣ�������ֶ�����ÿ���༶��Զ����ļ���Ļ�������������0��Ҳ���ǻ�������
		��������������OpenGL����ϣ����������Ϊ���ָ�ʽ�����ǵ�ͼ��ֻ��RGBֵ���������Ҳ��������ΪRGBֵ��
		���ĸ��͵���������������յ�����Ŀ�Ⱥ͸߶ȡ�����֮ǰ����ͼ���ʱ�򴢴������ǣ���������ʹ�ö�Ӧ�ı�����
		�¸�����Ӧ�����Ǳ���Ϊ0����ʷ���������⣩��
		���ߵڰ˸�����������Դͼ�ĸ�ʽ���������͡�����ʹ��RGBֵ�������ͼ�񣬲������Ǵ���Ϊchar(byte)���飬���ǽ��ᴫ���Ӧֵ��
		���һ��������������ͼ�����ݡ�
		������glTexImage2Dʱ����ǰ�󶨵��������ͻᱻ����������ͼ��Ȼ����Ŀǰֻ�л�������(Base-level)������ͼ�񱻼����ˣ����Ҫʹ�ö༶��Զ�������Ǳ����ֶ��������в�ͬ��ͼ�񣨲��ϵ����ڶ��������������ߣ�ֱ������������֮�����glGenerateMipmap��
		���Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
		*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture1" << std::endl;
	}
	//�������������Ӧ�Ķ༶��Զ������ͷ�ͼ����ڴ�
	stbi_image_free(data);
	// ���ز���������texture2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture2" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
   // -------------------------------------------------------------------------------------------
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
	// either set it manually like so:
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	// or set it via the texture class
	ourShader.setInt("texture2", 1);

	// pass projection matrix to shader(as projection matrix rarely changes there's no need to do this per frame)
	// -----------------------------------------------------------------------------------------------------------
	

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

	/*
		����ܻ����Ϊʲôsampler2D�����Ǹ�uniform������ȴ����glUniform������ֵ��
		ʹ��glUniform1i�����ǿ��Ը��������������һ��λ��ֵ�������Ļ������ܹ���һ��Ƭ����ɫ�������ö������
		һ�������λ��ֵͨ����Ϊһ������Ԫ(Texture Unit)��
		һ�������Ĭ������Ԫ��0������Ĭ�ϵļ�������Ԫ��

		����Ԫ����ҪĿ��������������ɫ���п���ʹ�ö���һ��������
		ͨ��������Ԫ��ֵ�������������ǿ���һ�ΰ󶨶������ֻҪ�������ȼ����Ӧ������Ԫ��
		����glBindTextureһ�������ǿ���ʹ��glActiveTexture��������Ԫ������������Ҫʹ�õ�����Ԫ��
		glActiveTexture(GL_TEXTURE0); // �ڰ�����֮ǰ�ȼ�������Ԫ
		glBindTexture(GL_TEXTURE_2D, texture);
		��������Ԫ֮�󣬽�������glBindTexture�������û�����������ǰ���������Ԫ��
		����ԪGL_TEXTURE0Ĭ�����Ǳ��������������ǰ��������ﵱ����ʹ��glBindTexture��ʱ��
		���輤���κ�����Ԫ��
	*/
		glActiveTexture(GL_TEXTURE0); // �ڰ�����֮ǰ�ȼ�������Ԫ
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		ourShader.use();
	
		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);
		
		////͸�Ӿ���͸��ͶӰ
		//glm::mat4 projection = glm::mat4(1.0f);
		//projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//// ע�⣬���ǽ�����������Ҫ�����ƶ������ķ������ƶ���
		//int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
		//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, &model[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//---------------------------
		/*
			������������Ⱦ
			glDrawElements()
				��һ������ָ�������ǻ��Ƶ�ģʽ�������glDrawArrays��һ����
				�ڶ������������Ǵ�����ƶ���ĸ�����������6��Ҳ����˵����һ����Ҫ����6�����㡣
				���������������������ͣ�������GL_UNSIGNED_INT��
				���һ�����������ǿ���ָ��EBO�е�ƫ���������ߴ���һ���������飬�������ǵ��㲻��ʹ��������������ʱ�򣩣��������ǻ���������д0��
			glDrawElements�����ӵ�ǰ�󶨵�GL_ELEMENT_ARRAY_BUFFERĿ���EBO�л�ȡ����,
			������ÿ��Ҫ��������Ⱦһ������ʱ��Ҫ����Ӧ��EBO��glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);�⻹���е��鷳��
			���������������ͬ�����Ա��������������İ�״̬��
			VAO��ʱ���ڰ󶨵������������ᱻ����ΪVAO��Ԫ�ػ������
			��VAO��ͬʱҲ���Զ���EBO��
		*/
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	/*
		����Ϸѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ��
		���ǿ�����main������������glfwTerminate�������ͷ�GLFW������ڴ档
	*/
	glfwTerminate();
	return 0;
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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	//�������ϣ���������ƶ�������ʹ�ò��������һ��������(Right Vector)������������Ӧ�ƶ��Ϳ����ˡ�
	//�����ʹ�����ʹ�������ʱ��Ϥ�ĺ���(Strafe)Ч����
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
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