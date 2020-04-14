#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//编辑着色器
//-------------------------------
//第三步顶点着色器vertex shader
	/*
		#version 330 core	//着色器语言GLSL与OpenGL版本号匹配

		使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)
		现在我们只关心位置(Position)数据，所以我们只需要一个顶点属性。
		通过vecx()设置位置向量。
		通过layout (location = 0)设定了输入变量的位置值(Location)。

		设置顶点着色器的输出，我们必须把位置数据赋值给预定义的gl_Position变量，它在幕后是vec4类型的。

	*/
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//第五步片段着色器fragment shader
	/*
		片段着色器只需要一个输出变量，这个变量是一个4分量向量，它表示的是最终的输出颜色。
		我们可以用out关键字声明输出变量，这里我们命名为FragColor。
		我们将一个alpha值为1.0(1.0代表完全不透明)的橘黄色的vec4赋值给颜色输出。
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
		首先，我们在main函数中调用glfwInit函数来初始化GLFW，
		然后我们可以使用glfwWindowHint函数来配置GLFW。
		如果你现在编译你的cpp文件会得到大量的 undefined reference (未定义的引用)错误，
		也就是未顺利地链接GLFW库。
	*/
	glfwInit();

	/*
		glfwWindowHint函数的第一个参数代表选项的名称，我们可以从很多以GLFW_开头的枚举值中选择；
		第二个参数接受一个整形，用来设置这个选项的值。
		需要告诉GLFW我们要使用的OpenGL版本是3.3，这样GLFW会在创建OpenGL上下文时做出适当的调整。
		这也可以确保用户在没有适当的OpenGL版本支持的情况下无法运行。
		我们将主版本号(Major)和次版本号(Minor)都设为3。
	*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	/*
		告诉GLFW我们使用的是核心模式(Core-profile)，并且不允许用户调整窗口的大小。
		在明确告诉GLFW使用核心模式的情况下，使用旧版函数将会导致invalid operation(无效操作)的错误
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
	//调用函数，程序窗口大小发生 变化时，调整渲染窗口大小
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
	//第四步 动态编译顶点着色器源码
	//先创建顶点着色器，使用glCreateShader()传递参数GL_VERTEX_SHADER
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	/*
		下一步我们把这个着色器源码附加到着色器对象上，然后编译它
		glShaderSource函数把要编译的着色器对象作为第一个参数。第二参数指定了传递的源码字符串数量，这里只有一个。
		第三个参数是顶点着色器真正的源码，第四个参数我们先设置为NULL。
	*/
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);//compile编译
	// check for shader compile errors
	/*
		首先我们定义一个整型变量来表示是否成功编译，还定义了一个储存错误消息（如果有的话）的容器。
		然后我们用glGetShaderiv检查是否编译成功。
		如果编译失败，我们会用glGetShaderInfoLog获取错误消息，然后打印它。
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
	//第六步编译片段着色器
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
	//第七步着色器程序，先生成着色器对象，再连接顶点与片段着色器，检查是否连接成功，连接成功删除着色器对象
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

	//第一步顶点输入
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//输入三角形顶点数据，一个点有（x,y,深度z）
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

	//第二步设置顶点缓冲对象VBO、顶点数组对象VAO、索引缓冲对象EBO
	unsigned int VBO, VAO,EBO;
	/*
		//学习于https://www.cnblogs.com/keguniang/p/9859057.html
		每当我们绘制一个几何体时，我们需要重复同样的工作（首先绑定缓冲区、然后设置顶点属性）。
		当需要绘制的物体很多时，这个过程就显得有些耗时。那么我们有没有一种方式来简化这一过程呢？
		这就是VAO做的事情，它将所有顶点绘制过程中的这些设置和绑定过程集中存储在一起，
		当我们需要时，只需要使用相应的VAO即可。
		VAO的这种方式有点像一个中介，把所有繁琐的绑定和顶点设置工作都集中起来处理，
		我们需要绘制时，直接找这个中介就好了。

		它针对“顶点”而言，也就是说它跟“顶点的绘制”息息相关和 点数组没有任何关系，VAO可以理解为一个状态容器，记录VBO的状态。
		通过VAO就能快速访问到VBO的状态及VBO的数据。
		VAO记录的是一次绘制中所需要的信息，这包括“数据在哪里glBindBuffer”、
		“数据的格式是怎么样的glVertexAttribPointer”。

		VAO是一个保存了所有顶点数据属性的状态结合，它存储了顶点数据的格式以及顶点数据所需的VBO对象的引用。
		VAO本身并没有存储顶点的相关属性数据，这些信息是存储在VBO中的，
		VAO相当于是对很多个VBO的引用，把一些VBO组合在一起作为一个对象统一管理。

		在创建VAO之后，需要使用glBindVertexArray设置它为当前操作的VAO,
		之后我们关于顶点数据的设置（包括数据使用的VBO对象，顶点的属性设置的信息都会被存储在VAO之中）。
		可以说VBO是对顶点属性信息的绑定，VAO是对很多个VBO的绑定。
	*/
	//通过glGenBuffers函数,生成一个缓冲ID和一个顶点缓冲对象VBO
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//初始化顶点数组对象，通过glGenVertexArrays()分配顶点数组对象，
	glGenVertexArrays(1, &VAO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	//首先绑定顶点数组对象，然后绑定和设置顶点缓冲区，然后配置顶点属性。
	//当得到顶点数组对象之后将它绑定到OpenGL环境以便使用。
	glBindVertexArray(VAO);
	//使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	//之后我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/*
		glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。
		它的第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。
		第二个参数指定传输数据的大小(以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。
		第三个参数是我们希望发送的实际数据。
		第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：
			GL_STATIC_DRAW ：数据不会或几乎不会改变。
			GL_DYNAMIC_DRAW：数据会被改变很多。
			GL_STREAM_DRAW ：数据每次绘制时都会改变。
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//与VBO类似，先绑定索引，在把索引复制到缓冲中。
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//第八步，链接顶点属性
	//设置顶点属性指针
	/*
		顶点着色器允许我们指定任何以顶点属性为形式的输入。
		这使其具有很强的灵活性的同时，
		它还的确意味着我们必须手动指定输入数据的哪一个部分对应顶点着色器的哪一个顶点属性。
		****使用glVertexAttribPointer()函数告诉OpenGL直接从内存中获取数据

		glVertexAttribPointer函数的参数
			第一个参数指定我们要配置的顶点属性。
				还记得我们在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)吗？它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
			第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
			第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
			第四个参数定义我们是否希望数据被标准化(Normalize)。
				如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
			第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。
				由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。
				要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
				一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子
				这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节）。
			最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。
				由于位置数据在数组的开头，所以这里是0。
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray，以顶点属性位置值作为参数，启用顶点属性；
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//已绑定三角形顶点数组对象，可以解绑顶点数组对象VAO
	glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	/*
		glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，
		如果是的话该函数返回true然后游戏循环便结束了，之后为我们就可以关闭应用程序了。
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
			我们希望绘制的是一个三角形，这里传递GL_TRIANGLES给它。
			第二个参数指定了顶点数组的起始索引，我们这里填0。
			最后一个参数指定我们打算绘制多少个顶点，这里是3
		*/
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//---------------------------
		/*
			从索引缓冲渲染
			glDrawElements()
				第一个参数指定了我们绘制的模式，这个和glDrawArrays的一样。
				第二个参数是我们打算绘制顶点的个数，这里填6，也就是说我们一共需要绘制6个顶点。
				第三个参数是索引的类型，这里是GL_UNSIGNED_INT。
				最后一个参数里我们可以指定EBO中的偏移量（或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候），但是我们会在这里填写0。
			glDrawElements函数从当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的EBO中获取索引,
			所以在每次要用索引渲染一个物体时需要绑定相应的EBO，glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);这还是有点麻烦。
			不过顶点数组对象同样可以保存索引缓冲对象的绑定状态。
			VAO绑定时正在绑定的索引缓冲对象会被保存为VAO的元素缓冲对象。
			绑定VAO的同时也会自动绑定EBO。
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
		当游戏循环结束后我们需要正确释放/删除之前的分配的所有资源。
		我们可以在main函数的最后调用glfwTerminate函数来释放GLFW分配的内存。
	*/
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
/*
	在我们（新创建的）key_callback函数中，我们检测了键盘是否按下了Escape键。
	如果键的确按下了(不释放)，使用glfwSetwindowShouldClose函数设定WindowShouldClose属性为true从而关闭GLFW。
*/
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
//窗口改变大小时，修改视口
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}