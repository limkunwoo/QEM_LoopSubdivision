#pragma once

#include "Header/textfile.h"
#include "GL\glew.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class myShader
{
	GLuint program;
	GLuint vertexShader;
	GLuint fragShader;
public:
	myShader();
	GLuint GetProgram();
	void CreateShader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
	void Begin();
	void end();

	void SetUniformMatrix4(const char* name, glm::mat4& matrix);
	void SetUniformVec3(const char* name, glm::vec3& vec);
	void SetUniformVec4(const char* name, glm::vec4& vec);

    int printOglError(char* file, int line);

    void printShaderInfoLog(GLuint obj);

	void printProgramInfoLog(GLuint obj);
};

