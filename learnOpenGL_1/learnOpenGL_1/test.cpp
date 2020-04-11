#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
	//程序窗口大小发生 变化时，调整渲染窗口大小
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

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

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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