#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//�༭��ɫ��
//-------------------------------
//������������ɫ��vertex shader
	/*
		#version 330 core	//��ɫ������GLSL��OpenGL�汾��ƥ��

		ʹ��in�ؼ��֣��ڶ�����ɫ�����������е����붥������(Input Vertex Attribute)
		��������ֻ����λ��(Position)���ݣ���������ֻ��Ҫһ���������ԡ�
		ͨ��vecx()����λ��������
		ͨ��layout (location = 0)�趨�����������λ��ֵ(Location)��

		���ö�����ɫ������������Ǳ����λ�����ݸ�ֵ��Ԥ�����gl_Position����������Ļ����vec4���͵ġ�

	*/
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//���岽Ƭ����ɫ��fragment shader
	/*
		Ƭ����ɫ��ֻ��Ҫһ��������������������һ��4��������������ʾ�������յ������ɫ��
		���ǿ�����out�ؼ����������������������������ΪFragColor��
		���ǽ�һ��alphaֵΪ1.0(1.0������ȫ��͸��)���ٻ�ɫ��vec4��ֵ����ɫ�����
	*/
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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
	// vertex shader
	//���Ĳ� ��̬���붥����ɫ��Դ��
	//�ȴ���������ɫ����ʹ��glCreateShader()���ݲ���GL_VERTEX_SHADER
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	/*
		��һ�����ǰ������ɫ��Դ�븽�ӵ���ɫ�������ϣ�Ȼ�������
		glShaderSource������Ҫ�������ɫ��������Ϊ��һ���������ڶ�����ָ���˴��ݵ�Դ���ַ�������������ֻ��һ����
		�����������Ƕ�����ɫ��������Դ�룬���ĸ���������������ΪNULL��
	*/
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);//compile����
	// check for shader compile errors
	/*
		�������Ƕ���һ�����ͱ�������ʾ�Ƿ�ɹ����룬��������һ�����������Ϣ������еĻ�����������
		Ȼ��������glGetShaderiv����Ƿ����ɹ���
		�������ʧ�ܣ����ǻ���glGetShaderInfoLog��ȡ������Ϣ��Ȼ���ӡ����
	*/
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	//����������Ƭ����ɫ��
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	//���߲���ɫ��������������ɫ�����������Ӷ�����Ƭ����ɫ��������Ƿ����ӳɹ������ӳɹ�ɾ����ɫ������
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//��һ����������
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//���������ζ������ݣ�һ�����У�x,y,���z��
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	//�ڶ������ö��㻺�����VBO�������������VAO�������������EBO
	unsigned int VBO, VAO,EBO;
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray���Զ�������λ��ֵ��Ϊ���������ö������ԣ�
	glEnableVertexAttribArray(0);

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
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		/*
			����ϣ�����Ƶ���һ�������Σ����ﴫ��GL_TRIANGLES������
			�ڶ�������ָ���˶����������ʼ����������������0��
			���һ������ָ�����Ǵ�����ƶ��ٸ����㣬������3
		*/
		//glDrawArrays(GL_TRIANGLES, 0, 3);
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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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