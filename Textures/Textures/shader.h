#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>//文件IO类头文件
#include <sstream>//stringIO类头文件
#include <iostream>

class Shader
{
public:
	// 程序ID
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	// 构造器读取并构建着色器
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	// 使用/激活程序
	void use();
	
	// uniform工具函数
	void setBool(const std::string &name, bool value) const;
	
	void setInt(const std::string &name, int value) const;
	
	void setFloat(const std::string &name, float value) const;
	

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(GLuint shader, std::string type);

};

#endif
