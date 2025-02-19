#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;		// Vertex shader code
	std::string fragmentCode;	// Fragment shader code
	
	std::ifstream vShaderFile;	// Vertex shader file
	std::ifstream fShaderFile;	// Fragment shader file

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Read vertex shader file
		vShaderFile.open(vertexPath);
		std::stringstream vShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		vShaderFile.close();
		
		// Fill vertex shader code
		vertexCode = vShaderStream.str();
		
		
		// Read fragment shader file
		fShaderFile.open(fragmentPath);
		std::stringstream fShaderStream;
		fShaderStream << fShaderFile.rdbuf();
		fShaderFile.close();
				
		// Fill fragment shader code
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	vertex = compileShader(GL_VERTEX_SHADER, vShaderCode);
	fragment = compileShader(GL_FRAGMENT_SHADER, fShaderCode);

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	checkCompileErrors(id, "PROGRAM");


	glDeleteShader(vertex);
	glDeleteShader(fragment);
}



Shader::~Shader()
{
	glDeleteProgram(id);
}

void Shader::setVertexAttribPointer(const char* param, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const
{
	GLint posAttrib = glGetAttribLocation(id, param);
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, size, type, normalized, stride, pointer);
}

void Shader::use() const
{
	glUseProgram(id);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set3Float(const std::string& name, float v0, float v1, float v2) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), v0, v1, v2);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

GLuint Shader::compileShader(GLenum type, const char* source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	checkCompileErrors(shader, "SHADER");
	return shader;
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) const
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cerr << "ERROR::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			std::cerr << "ERROR::PROGRAM_LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
}