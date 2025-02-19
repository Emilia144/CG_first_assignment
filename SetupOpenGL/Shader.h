#pragma once
#include "glad/glad.h"
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	void setVertexAttribPointer(const char* param, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const;
	void use() const;
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void set3Float(const std::string& name, float v0, float v1, float v2) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
private:
	GLuint compileShader(GLenum type, const char* source);
	void checkCompileErrors(GLuint shader, const std::string& type) const;
private:
	GLuint id;
};