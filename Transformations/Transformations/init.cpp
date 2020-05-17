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
	Shader ourShader("vShaderFile.txt","fshaderFile.txt");
	
	//第一步顶点输入
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//输入三角形顶点数据，一个点有（x,y,深度z）
	
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // 左上
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	//第二步设置顶点缓冲对象VBO、顶点数组对象VAO、索引缓冲对象EBO
	unsigned int VBO, VAO, EBO;
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
	//通过glGenBuffers函数,生成一个缓冲ID、一个顶点缓冲对象VBO，通过glGenTextures()生成纹理对象
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
	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray，以顶点属性位置值作为参数，启用顶点属性；
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//纹理属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//已绑定三角形顶点数组对象，可以解绑顶点数组对象VAO
	glBindVertexArray(0);

	stbi_set_flip_vertically_on_load(true);
	// load and create a texture 
	// -------------------------
	unsigned int texture1,texture2;
	// 加载并生成纹理texture1
	//glGenTextures函数首先需要输入生成纹理的数量，然后把它们储存在第二个参数
	glGenTextures(1, &texture1);
	//配置当前绑定的纹理
	glBindTexture(GL_TEXTURE_2D, texture1);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	/*
	使用glTexParameter*函数第一个参数指定了纹理目标；
	我们使用的是2D纹理，因此纹理目标是GL_TEXTURE_2D。
	第二个参数需要我们指定设置的选项与应用的纹理轴。我们打算配置的是WRAP选项，并且指定S和T轴。
	最后一个参数需要我们传递一个环绕方式(Wrapping)

		环绕方式	描述
		GL_REPEAT	对纹理的默认行为。重复纹理图像。
		GL_MIRRORED_REPEAT	和GL_REPEAT一样，但每次重复图片是镜像放置的。
		GL_CLAMP_TO_EDGE	纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
		GL_CLAMP_TO_BORDER	超出的坐标为用户指定的边缘颜色。
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/*
	当进行放大(Magnify)和缩小(Minify)操作的时候可以设置纹理过滤的选项，比如你可以在纹理被缩小的时候使用邻近过滤，被放大时使用线性过滤。
	我们需要使用glTexParameter*函数为放大和缩小指定过滤方式。
		GL_NEAREST（也叫邻近过滤，Nearest Neighbor Filtering）是OpenGL默认的纹理过滤方式。
		当设置为GL_NEAREST的时候，OpenGL会选择中心点最接近纹理坐标的那个像素。
		GL_NEAREST产生了颗粒状的图案，我们能够清晰看到组成纹理的像素，
		GL_LINEAR（也叫线性过滤，(Bi)linear Filtering）它会基于纹理坐标附近的纹理像素，计算出一个插值，近似出这些纹理像素之间的颜色。
		一个纹理像素的中心距离纹理坐标越近，那么这个纹理像素的颜色对最终的样本颜色的贡献越大。
		而GL_LINEAR能够产生更平滑的图案，很难看出单个的纹理像素。GL_LINEAR可以产生更真实的输出
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	//stb_image.h将会用图像的宽度、高度和颜色通道的个数填充这三个变量
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		/*
		glTexImage2D()
		第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
		第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
		第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
		第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
		下个参数应该总是被设为0（历史遗留的问题）。
		第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
		最后一个参数是真正的图像数据。
		当调用glTexImage2D时，当前绑定的纹理对象就会被附加上纹理图像。然而，目前只有基本级别(Base-level)的纹理图像被加载了，如果要使用多级渐远纹理，我们必须手动设置所有不同的图像（不断递增第二个参数）。或者，直接在生成纹理之后调用glGenerateMipmap。
		这会为当前绑定的纹理自动生成所有需要的多级渐远纹理。
		*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture1" << std::endl;
	}
	//生成了纹理和相应的多级渐远纹理后，释放图像的内存
	stbi_image_free(data);
	// 加载并生成纹理texture2
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
		//激活着色器
		//glUseProgram(shaderProgram);

		// 更新uniform颜色
		/*
			我们通过glfwGetTime()获取运行的秒数。
			然后我们使用sin函数让颜色在0.0到1.0之间改变，最后将结果储存到greenValue里。
		*/
		/*float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;*/
		/*
			我们用glGetUniformLocation查询uniform ourColor的位置值，第一个参数是查询的着色器程序和uniform的名字。
			如果glGetUniformLocation返回-1就代表没有找到这个位置值。
		*/
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		////最后，我们可以通过glUniform4f函数设置uniform值。
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

	/*
		你可能会奇怪为什么sampler2D变量是个uniform，我们却不用glUniform给它赋值。
		使用glUniform1i，我们可以给纹理采样器分配一个位置值，这样的话我们能够在一个片段着色器中设置多个纹理。
		一个纹理的位置值通常称为一个纹理单元(Texture Unit)。
		一个纹理的默认纹理单元是0，它是默认的激活纹理单元。

		纹理单元的主要目的是让我们在着色器中可以使用多于一个的纹理。
		通过把纹理单元赋值给采样器，我们可以一次绑定多个纹理，只要我们首先激活对应的纹理单元。
		就像glBindTexture一样，我们可以使用glActiveTexture激活纹理单元，传入我们需要使用的纹理单元：
		glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture);
		激活纹理单元之后，接下来的glBindTexture函数调用会绑定这个纹理到当前激活的纹理单元，
		纹理单元GL_TEXTURE0默认总是被激活，所以我们在前面的例子里当我们使用glBindTexture的时候，
		无需激活任何纹理单元。
	*/
		glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
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
			查询uniform变量的地址，然后用有Matrix4fv后缀的glUniform函数把矩阵数据发送给着色器。
			第一个参数你现在应该很熟悉了，它是uniform的位置值。
			第二个参数告诉OpenGL我们将要发送多少个矩阵，这里是1。
			第三个参数询问我们我们是否希望对我们的矩阵进行置换(Transpose)，也就是说交换我们矩阵的行和列。
			OpenGL开发者通常使用一种内部矩阵布局，叫做列主序(Column-major Ordering)布局。
			GLM的默认布局就是列主序，所以并不需要置换矩阵，我们填GL_FALSE。
			最后一个参数是真正的矩阵数据，但是GLM并不是把它们的矩阵储存为OpenGL所希望接受的那种，因此我们要先用GLM的自带的函数value_ptr来变换这些数据。
		*/
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		/*
			我们希望绘制的是一个三角形，这里传递GL_TRIANGLES给它。
			第二个参数指定了顶点数组的起始索引，我们这里填0。
			最后一个参数指定我们打算绘制多少个顶点，这里是3
		*/
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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