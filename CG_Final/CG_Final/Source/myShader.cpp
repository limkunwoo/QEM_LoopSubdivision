#include "Header/myShader.h"

GLuint myShader::GetProgram()
{
    return program;
}

myShader::myShader()
{
    program = 0;
    vertexShader = 0;
    fragShader = 0;
}

void myShader::CreateShader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
    char* vs = NULL, * fs = NULL, * fs2 = NULL;
    GLuint v, f;
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    vs = textFileRead(vertexShaderFilePath);
    fs = textFileRead(fragmentShaderFilePath);

    const char* vv = vs;
    const char* ff = fs;

    glShaderSource(v, 1, &vv, NULL);
    glShaderSource(f, 1, &ff, NULL);

    free(vs); free(fs);

    glCompileShader(v);
    glCompileShader(f);
    printShaderInfoLog(v);
    printShaderInfoLog(f);

    program = glCreateProgram();
    glAttachShader(program, v);
    glAttachShader(program, f);

    glLinkProgram(program);
    printProgramInfoLog(program);
}

void myShader::Begin()
{
    glUseProgram(program);
}

void myShader::end()
{
    glUseProgram(0);
}

void myShader::SetUniformMatrix4(const char* name, glm::mat4& matrix)
{
    GLuint uniformMatrix = glGetUniformLocation(program, name);
    glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, value_ptr(matrix));
}

void myShader::SetUniformVec3(const char* name, glm::vec3& vec)
{
    GLuint uniformVec = glGetUniformLocation(program, name);
    glUniform3fv(uniformVec, 1, value_ptr(vec));
}

void myShader::SetUniformVec4(const char* name, glm::vec4& vec)
{
    GLuint uniformVec = glGetUniformLocation(program, name);
    glUniform4fv(uniformVec, 1, value_ptr(vec));
}

int myShader::printOglError(char* file, int line)
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

void myShader::printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten = 0;
    char* infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 0)
    {
        infoLog = (char*)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n", infoLog);
        free(infoLog);
    }
}

void myShader::printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten = 0;
    char* infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 0)
    {
        infoLog = (char*)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n", infoLog);
        free(infoLog);
    }
}
