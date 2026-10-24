#include "ShaderLoader.h"

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// constructor reads and builds the shader - without geometry shader
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// getting shader's source code
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;
	// ensure ifstream objects can throw exceptions
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);
		std::stringstream vertexShaderStream, fragmentShaderStream;
		// read file's buffer contents into streams (basically entire file)
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		// close file handelers (files)
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// convering streams into strings
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();

	}
	catch (std::ifstream::failure exception)
	{
		std::cout << "Shader error: file not successfully read: " << vertexPath <<  ", " << fragmentPath << '\n';
		std::cout << "Details:\n" << exception.what() << '\n';
	}
	// changing strings into const char*
	const char* vertexShaderCode{ vertexCode.c_str() };
	const char* fragmentShaderCode{ fragmentCode.c_str() };

	// compiling shaders
	unsigned int vertexShader, fragmentShader;
	// vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	// check for errors
	checkCompileErrors(vertexShader, "VERTEX");
	// fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	// check for errors
	checkCompileErrors(fragmentShader, "FRAGMENT");

	// shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	// check for errors
	checkCompileErrors(ID, "PROGRAM");

	// delete the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// constructor reads and builds the shader -  this one also with the geometry shader
Shader::Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
	// getting shader's source code
	std::string vertexCode;
	std::string geometryCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream geometryShaderFile;
	std::ifstream fragmentShaderFile;
	// ensure ifstream objects can throw exceptions
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geometryShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vertexShaderFile.open(vertexPath);
		geometryShaderFile.open(geometryPath);
		fragmentShaderFile.open(fragmentPath);
		std::stringstream vertexShaderStream, geometryShaderStream, fragmentShaderStream;
		// read file's buffer contents into streams (basically entire file)
		vertexShaderStream << vertexShaderFile.rdbuf();
		geometryShaderStream << geometryShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		// close file handelers (files)
		vertexShaderFile.close();
		geometryShaderFile.close();
		fragmentShaderFile.close();
		// convering streams into strings
		vertexCode = vertexShaderStream.str();
		geometryCode = geometryShaderStream.str();
		fragmentCode = fragmentShaderStream.str();

	}
	catch (std::ifstream::failure exception)
	{
		std::cout << "Shader error: file not successfully read: " << vertexPath << ", " << geometryPath << ", " << fragmentPath << '\n';
		std::cout << "Details:\n" << exception.what() << '\n';
	}
	// changing strings into const char*
	const char* vertexShaderCode{ vertexCode.c_str() };
	const char* geometryShaderCode{ geometryCode.c_str() };
	const char* fragmentShaderCode{ fragmentCode.c_str() };

	// compiling shaders
	unsigned int vertexShader, fragmentShader, geometryShader;
	// vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	// check for errors
	checkCompileErrors(vertexShader, "VERTEX");
	// geometry shader
	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader, 1, &geometryShaderCode, NULL);
	glCompileShader(geometryShader);
	// check for errors
	checkCompileErrors(geometryShader, "GEOMETRY");
	// fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	// check for errors
	checkCompileErrors(fragmentShader, "FRAGMENT");

	// shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, geometryShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	// check for errors
	checkCompileErrors(ID, "PROGRAM");

	// delete the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
}

// check if the shader compiled succesfully
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Shader compilation error (" << type << "):\n"
				<< infoLog << "\n -- --------------------------------------------------- -- \n" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Shader program linking error (" << type << "):\n"
				<< infoLog << "\n -- --------------------------------------------------- -- \n" << std::endl;
		}
	}
}