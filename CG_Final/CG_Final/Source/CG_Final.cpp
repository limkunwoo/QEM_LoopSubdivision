// CG_FinalProject.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Header/Mesh.h"
#include "Header/textfile.h"
#include "Header/myShader.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Windows.h>

using namespace glm;

float _zoom = 15.0f;
float _rotate_x = 0;
float _rotate_y = 0.001f;
float _translate_x = 0.0f;
float _translate_y = 0.0f;
int last_x = 0;
int last_y = 0;
float _viewportX = 640.f;
float _viewportY = 480.f;

Mesh* _mesh;
myShader _faceShader;
myShader _structureShader;
vec3 cameraPosition = vec3(0,0,10);

bool _displayPoint = false;
bool _displayWire = false;
bool _displaySurface = true;

unsigned char _btnStates[3] = { 0 };
void Init(void)
{
    glEnable(GL_DEPTH_TEST);
}
void draw(void)
{
    mat4 translateMat = mat4(1.0f);
    translateMat = translate(translateMat, vec3(_translate_x, _translate_y, -_zoom));
    mat4 rotateMat = mat4(1.0f);
    rotateMat = rotate(rotateMat, _rotate_x, vec3(0, 1, 0));
    rotateMat = rotateMat * rotate(rotateMat, _rotate_y, vec3(1, 0, 0));
    mat4 model = translateMat * rotateMat;

    mat4 projectionMat = mat4(1.0f);
    projectionMat = perspective(45.0f, _viewportX / _viewportY, 0.1f, 100.0f);
    
    mat4 viewMat = mat4(1.0f);
    viewMat = inverse(translate(viewMat, cameraPosition));

    _faceShader.Begin();
    _faceShader.SetUniformMatrix4("projection", projectionMat);
    _faceShader.SetUniformMatrix4("model", model);
    _faceShader.SetUniformMatrix4("view", viewMat);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    if (_displaySurface)
        _mesh->DrawFace();
    _faceShader.end();

    _structureShader.Begin();
    _structureShader.SetUniformMatrix4("projection", projectionMat);
    _structureShader.SetUniformMatrix4("model", model);
    _structureShader.SetUniformMatrix4("view", viewMat);

    if (_displayPoint)
        _mesh->DrawPoint();
    if (_displayWire)
        _mesh->DrawEdge();
    _structureShader.end();
    glDisable(GL_LIGHTING);
    
}

void GL_Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(_translate_x, _translate_y, -_zoom);
    glRotatef(_rotate_x, 0, 1, 0);
    glRotatef(_rotate_y, 1, 0, 0);

    draw();
    glutSwapBuffers();
}
void GL_Reshape(int w, int h)
{
    if (w == 0) {
        h = 1;
    }
    _viewportX = w;
    _viewportY = h;
    glViewport(0, 0, _viewportX, _viewportY);
    
    //glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(projectionMat));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)_viewportX / (float)_viewportY, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}
void GL_Motion(int x, int y)
{
    int diff_x = x - last_x;
    int diff_y = y - last_y;

    last_x = x;
    last_y = y;

    if (_btnStates[2]) {
        _zoom -= (float)0.05f * diff_x;
    }
    else if (_btnStates[0]) {
        _rotate_x += (float)0.005f * diff_x;
        _rotate_y += (float)0.005f * diff_y;
    }
    else if (_btnStates[1]) {
        _translate_x += (float)0.05f * diff_x;
        _translate_y -= (float)0.05f * diff_y;
    }
    glutPostRedisplay();
}

void GL_Mouse(int button, int state, int x, int y)
{
    last_x = x;
    last_y = y;

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        _btnStates[0] = ((GLUT_DOWN == state) ? 1 : 0);
        break;
    case GLUT_MIDDLE_BUTTON:
        _btnStates[1] = ((GLUT_DOWN == state) ? 1 : 0);
        break;
    case GLUT_RIGHT_BUTTON:
        _btnStates[2] = ((GLUT_DOWN == state) ? 1 : 0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void GL_Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'Q':
    case 'q':
        exit(0);
        break;
    case 'L':
    case 'l':
        _mesh->LoopSubdivision(1);
        break;
    case '1':
        _mesh->LoadObj("obj\\obj_1.obj");
        break;
    case '2':
        _mesh->LoadObj("obj\\obj_2.obj");
        break;
    case '3':
        _mesh->LoadObj("obj\\obj_3.obj");
        break;
    case '4':
        _mesh->LoadObj("obj\\obj_4.obj");
        break;
    case '5':
        _mesh->LoadObj("obj\\obj_5.obj");
        break;
    case '6':
        _mesh->LoadObj("obj\\obj_6.obj");
        break;
    case '7':
        _mesh->LoadObj("obj\\obj_7.obj");
        break;
    case '8':
        _mesh->LoadObj("obj\\obj_8.obj");
        break;
    case '9':
        _mesh->LoadObj("obj\\dragon.obj");
        break;
    case '0':
        _mesh->LoadObj("obj\\cow.obj");
        break;
    case 'Z':
    case 'z':
        _displayPoint = !_displayPoint;
        break;
    case 'x':
    case 'X':
        _displayWire = !_displayWire;
        break;
    case 'C':
    case 'c':
        _displaySurface = !_displaySurface;
        break;
    case 'E':
    case 'e':
        _mesh->Simplification(0.8);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void DoMenu(int value)  // 순서대로 1,2,3 메인팝업, 4,5,6 서브팝업
{
    switch (value) {
    case 1:
        glClearColor(1.0, 1.0, 1.0, 1.0);
        break;
    case 2:
        glClearColor(0.0, 0.0, 0.0, 1.0);
        break;
    case 3:
        glClearColor(0.5, 0.5, 0.5, 1.0);
        break;
    case 4:
        glColor3f(1.0, 0.0, 0.0);
        break;
    case 5:
        glColor3f(0.0, 1.0, 0.0);
        break;
    case 6:
        glColor3f(0.0, 0.0, 1.0);
        break;
    }
    glutPostRedisplay();
}

void initGLEW()
{
    //Initialize GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(0);
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    //Check Shader
    //ARB
    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
        printf("Ready for GLSL (ARB)\n");
    else
    {
        printf("No GLSL support\n");
        exit(0);
    }

    //OpenGL 4.6
    if (glewIsSupported("GL_VERSION_4_6"))
        printf("Ready for OpenGL 4.6\n");
    else {
        printf("OpenGL 4.6 not supported\n");
        exit(0);
    }
}
int main()
{
    _mesh = new Mesh("obj\\obj_1.obj");

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Camera Nevigation");
    glutDisplayFunc(GL_Display);
    glutReshapeFunc(GL_Reshape);
    glutMouseFunc(GL_Mouse);
    glutMotionFunc(GL_Motion);
    glutKeyboardFunc(GL_Keyboard);
    initGLEW();
    //setShaders("Shader/minimal.vert", "Shader/minimal.frag");
    _faceShader.CreateShader("Shader/minimal.vert", "Shader/minimal.frag");
    _structureShader.CreateShader("Shader/structVert.vert", "Shader/minimal.frag");
    Init();

    char* vs = NULL, * fs = NULL, * fs2 = NULL;
    vs = textFileRead("Shader/minimal.vert");
    fs = textFileRead("Shader/minimal.frag");

    std::cout << vs << std::endl;
    std::cout << fs << std::endl;

    glutMainLoop();
}