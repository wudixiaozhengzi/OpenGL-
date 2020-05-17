#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>; // ����glad����ȡ���еı���OpenGLͷ�ļ�

#include <string>
#include <fstream>//�ļ�IO��ͷ�ļ�
#include <sstream>//stringIO��ͷ�ļ�
#include <iostream>

class Shader
{
public:
	// ����ID
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	// ��������ȡ��������ɫ��
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	// ʹ��/�������
	void use();
	
	// uniform���ߺ���
	void setBool(const std::string &name, bool value) const;
	
	void setInt(const std::string &name, int value) const;
	
	void setFloat(const std::string &name, float value) const;
	

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(GLuint shader, std::string type);

};

#endif
