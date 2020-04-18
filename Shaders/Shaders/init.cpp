#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>

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
		// λ��              // ��ɫ
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
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
	//ͨ��glGenBuffers����,����һ������ID��һ�����㻺�����VBO
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
	////��VBO���ƣ��Ȱ��������ڰ��������Ƶ������С�
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray���Զ�������λ��ֵ��Ϊ���������ö������ԣ�
	glEnableVertexAttribArray(0);
	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//�Ѱ������ζ���������󣬿��Խ�󶥵��������VAO
	glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		ourShader.use();
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		/*
			����ϣ�����Ƶ���һ�������Σ����ﴫ��GL_TRIANGLES������
			�ڶ�������ָ���˶����������ʼ����������������0��
			���һ������ָ�����Ǵ�����ƶ��ٸ����㣬������3
		*/
		glDrawArrays(GL_TRIANGLES, 0, 3);
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