/* Shaders.cpp
 * Author : Kozhukharov Nikita
 */

//#include <..\glad\glad.h>
//#include <..\GLFW\glfw3.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Renderer/Shaders.h>


/***
 * Shader subclass methods definitions
 ***/

ShaderProgram::Shader::Shader(int shaderType, const char* filepath = "")
	: type(shaderType), valid(false), id(0) {
	if (filepath != "") {
		reload(filepath);
	}
}

void ShaderProgram::Shader::reload(const char* filename) {
	valid = false;
	std::ifstream file(filename);
	if (!file.is_open()) {
		// error, ivalid shader source
		valid = false;
		return;
	}
	std::stringstream text;
	text << file.rdbuf();
	file.close();

	std::string 	texts = text.str();
	const char* text_fmt = texts.c_str();
	id = glCreateShader(type);
	glShaderSource(id, 1, &text_fmt, NULL);
	glCompileShader(id);

	int shaderParam;
	std::string infoLog(512, 0);
	glGetShaderiv(id, GL_COMPILE_STATUS, &shaderParam);
	if (!shaderParam) {
		glGetShaderInfoLog(id, infoLog.size(), NULL, &infoLog[0]);
		std::cout << "Error compiling shader: \n" << infoLog << "\n";
		valid = false;
	}
	else
		valid = true;
}

bool ShaderProgram::Shader::isValid() {
	return valid;
}

unsigned int ShaderProgram::Shader::getId() {
	return id;
}

ShaderProgram::Shader::~Shader() {
	glDeleteShader(id);
}


/***
 * Program methods definitions
 ***/

ShaderProgram::ShaderProgram(const char* vertShaderFilename, const char* fragShaderFilename)
 : valid(false), programId(0) {
	Shader vertShader(GL_VERTEX_SHADER, vertShaderFilename);
	Shader fragShader(GL_FRAGMENT_SHADER, fragShaderFilename);

	if (!vertShader.isValid() || !fragShader.isValid()) {
		// some shader is invalid, add errors processing
		return;
	}
	programId = glCreateProgram();
	glAttachShader(programId, vertShader.getId());
	glAttachShader(programId, fragShader.getId());
	glLinkProgram(programId);

	int params;
	std::string infoLog(512, 0);
	glGetProgramiv(programId, GL_LINK_STATUS, &params);
	if (!params) {
		glGetProgramInfoLog(programId, 512, NULL, &infoLog[0]);
		std::cout << "Error linking program: \n" << infoLog << "\n";
		// error while linking program occured
	}
	else
		valid = true;
}

bool ShaderProgram::isValid() const {
	return valid;
}

void ShaderProgram::use() const {
	glUseProgram(programId);
}
