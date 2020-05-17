#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
	Shader ourShader("vShaderFile.txt","fshaderFile.txt");
	
	//��һ����������
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//���������ζ������ݣ�һ�����У�x,y,���z��
	
	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // ����
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // ����
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
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
	glGenBuffers(1, &EBO);

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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray���Զ�������λ��ֵ��Ϊ���������ö������ԣ�
	glEnableVertexAttribArray(0);
	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//��������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
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

	// render loop
	// -----------
	/*
		glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳���
		����ǵĻ��ú�������trueȻ����Ϸѭ��������ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó����ˡ�
	*/
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		//������ɫ��
		//glUseProgram(shaderProgram);

		// ����uniform��ɫ
		/*
			����ͨ��glfwGetTime()��ȡ���е�������
			Ȼ������ʹ��sin��������ɫ��0.0��1.0֮��ı䣬��󽫽�����浽greenValue�
		*/
		/*float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;*/
		/*
			������glGetUniformLocation��ѯuniform ourColor��λ��ֵ����һ�������ǲ�ѯ����ɫ�������uniform�����֡�
			���glGetUniformLocation����-1�ʹ���û���ҵ����λ��ֵ��
		*/
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		////������ǿ���ͨ��glUniform4f��������uniformֵ��
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

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

		// create transformations
		glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		//transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		ourShader.use();
		/*
			��ѯuniform�����ĵ�ַ��Ȼ������Matrix4fv��׺��glUniform�����Ѿ������ݷ��͸���ɫ����
			��һ������������Ӧ�ú���Ϥ�ˣ�����uniform��λ��ֵ��
			�ڶ�����������OpenGL���ǽ�Ҫ���Ͷ��ٸ�����������1��
			����������ѯ�����������Ƿ�ϣ�������ǵľ�������û�(Transpose)��Ҳ����˵�������Ǿ�����к��С�
			OpenGL������ͨ��ʹ��һ���ڲ����󲼾֣�����������(Column-major Ordering)���֡�
			GLM��Ĭ�ϲ��־������������Բ�����Ҫ�û�����������GL_FALSE��
			���һ�������������ľ������ݣ�����GLM�����ǰ����ǵľ��󴢴�ΪOpenGL��ϣ�����ܵ����֣��������Ҫ����GLM���Դ��ĺ���value_ptr���任��Щ���ݡ�
		*/
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		/*
			����ϣ�����Ƶ���һ�������Σ����ﴫ��GL_TRIANGLES������
			�ڶ�������ָ���˶����������ʼ����������������0��
			���һ������ָ�����Ǵ�����ƶ��ٸ����㣬������3
		*/
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
	glDeleteBuffers(1, &EBO);

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