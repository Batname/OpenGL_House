#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// MACROS
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

using namespace std;
using namespace glm;

// variables declaration
GLFWwindow* window;
GLuint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
int viewportWidth, viewportHeight;
const char* title = "Dream house";

vec3 cameraPos(0.0f, 0.0f, 3.0f);
vec3 cameraFront(0.0f, 0.0f, -1.0f);
vec3 cameraUp(0.0f, 1.0f, 0.0f);
bool keys[1024];
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
GLfloat lastX = WINDOW_WIDTH / 2.0f;
GLfloat lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


typedef struct {
    vec3 vertices;
    vec3 color;
} GeometryData;


// function declaration
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
GLuint LoadShaders(const char* vertexFilePath, const char* fragmentFilePath);
void doMovement();

// geometry defenitional
GeometryData geometryData[] = {
    // front
    vec3( 0.5f,  0.5f, 0.5f), vec3( 1.0f,  0.0f, 0.0f), // 0
    vec3( 0.5f, -0.5f, 0.5f), vec3( 1.0f,  0.0f, 0.0f), // 1
    vec3(-0.5f, -0.5f, 0.5f), vec3( 1.0f,  0.0f, 0.0f), // 2
    vec3(-0.5f,  0.5f, 0.5f), vec3( 1.0f,  0.0f, 0.0f), // 3
    vec3( 0.0f,  0.8f, 0.5f), vec3( 1.0f,  0.0f, 0.0f), // 4
    
    // side right
    vec3( 0.0f,  0.8f, -0.5f), vec3( 0.0f,  1.0f, 0.0f), // 5
    vec3( 0.5f,  0.5f, -0.5f), vec3( 0.0f,  1.0f, 0.0f), // 6
    vec3( 0.5f,  0.5f,  0.5f), vec3( 0.0f,  1.0f, 0.0f), // 7
    vec3( 0.0f,  0.8f,  0.5f), vec3( 0.0f,  1.0f, 0.0f), // 8
    vec3( 0.5f, -0.5f, -0.5f), vec3( 0.0f,  1.0f, 0.0f), // 9
    vec3( 0.5f, -0.5f,  0.5f), vec3( 0.0f,  1.0f, 0.0f), // 10
    
    // side left
    vec3( 0.0f,  0.8f, -0.5f), vec3( 0.0f,  1.0f, 0.0f), // 11
    vec3(-0.5f,  0.5f, -0.5f), vec3( 0.0f,  1.0f, 0.0f), // 12
    vec3(-0.5f,  0.5f,  0.5f), vec3( 0.0f,  1.0f, 0.0f), // 13
    vec3( 0.0f,  0.8f,  0.5f), vec3( 0.0f,  1.0f, 0.0f), // 14
    vec3(-0.5f, -0.5f, -0.5f), vec3( 0.0f,  1.0f, 0.0f), // 15
    vec3(-0.5f, -0.5f,  0.5f), vec3( 0.0f,  1.0f, 0.0f), // 16
    
    // top
    vec3( 0.5f,  0.5f, -0.5f), vec3( 0.0f,  0.0f, 0.0f), // 17
    vec3( 0.5f,  0.5f,  0.5f), vec3( 0.0f,  0.0f, 0.0f), // 18
    vec3( 0.0f,  0.8f,  0.5f), vec3( 0.0f,  0.0f, 0.0f), // 19
    vec3( 0.0f,  0.8f, -0.5f), vec3( 0.0f,  0.0f, 0.0f), // 20
    vec3( -0.5f, 0.5f,  0.5f), vec3( 0.0f,  0.0f, 0.0f), // 21
    vec3( -0.5f, 0.5f, -0.5f), vec3( 0.0f,  0.0f, 0.0f), // 22
    
    // bottom
    vec3( 0.5f, -0.5f, -0.5f), vec3( 0.0f,  0.0f, 0.0f), // 23
    vec3( 0.5f, -0.5f,  0.5f), vec3( 0.0f,  0.0f, 0.0f), // 24
    vec3(-0.5f, -0.5f,  0.5f), vec3( 0.0f,  0.0f, 0.0f), // 25
    vec3(-0.5f, -0.5f, -0.5f), vec3( 0.0f,  0.0f, 0.0f), // 26
    
    // back
    vec3( 0.5f,  0.5f, -0.5f), vec3( 0.0f,  0.0f, 1.0f), // 27
    vec3( 0.5f, -0.5f, -0.5f), vec3( 0.0f,  0.0f, 1.0f), // 28
    vec3(-0.5f, -0.5f, -0.5f), vec3( 0.0f,  0.0f, 1.0f), // 29
    vec3(-0.5f,  0.5f, -0.5f), vec3( 0.0f,  0.0f, 1.0f), // 30
    vec3( 0.0f,  0.8f, -0.5f), vec3( 0.0f,  0.0f, 1.0f), // 31

};

GLuint indices[] = {
    // front
    0, 1, 3,
    1, 2, 3,
    0, 3, 4,
    
    // side right
    5, 6, 7,
    7, 8, 5,
    
    6, 9, 10,
    10, 7, 6,
    
    // side left
    11, 12, 13,
    13, 14, 11,
    
    12, 15, 16,
    16, 13, 12,
    
    // top
    17, 18, 19,
    19, 20, 17,
    
    19, 20, 21,
    21, 22, 20,
    
    // bottom
    23, 24, 25,
    25, 26, 23,
    
    // back
    27, 28, 29,
    29, 30, 27,
    27, 30, 31,
};

GLuint numVertices = NUM_ARRAY_ELEMENTS(geometryData);
GLuint numIndices = NUM_ARRAY_ELEMENTS(indices);

// Buffers declaration
GLuint vertexBufferObjectID, vertexArrayObjectID, elementBufferObjectID;
GLuint programID;
GLuint fullTransformMatrixLocation;


// MAIN
int main(int argc, const char * argv[]) {
    assert(glfwInit() && "glfw init error");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);
    assert(window && "Failed to create window");
    glfwMakeContextCurrent(window);
    
    /*--------glew options-----------*/
    glewExperimental = true;
    assert(glewInit() == GLEW_OK && "Failad init GLEW");
    
    glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
    glViewport(0, 0, viewportWidth, viewportHeight);
    glEnable(GL_DEPTH_TEST);
    
    /*--------glfw options-----------*/
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    
    /* ------ GL buffers-------------*/
    glGenVertexArrays(1, &vertexArrayObjectID);
    glGenBuffers(1, &vertexBufferObjectID);
    glGenBuffers(1, &elementBufferObjectID);

    glBindVertexArray(vertexArrayObjectID);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(geometryData), geometryData, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjectID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryData), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryData), (GLvoid*)(sizeof(GeometryData) / 2));
    glEnableVertexAttribArray(1);
    
    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    /* ----- load programID ---------*/
    programID = LoadShaders("shaders/house.vs.glsl", "shaders/house.frag.glsl");
    fullTransformMatrixLocation = glGetUniformLocation(programID, "fullTransformMatrix");
    
    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glfwPollEvents();
        doMovement();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw geometry
        glUseProgram(programID);
    
        mat4 fullTransformMatrix;
        
        mat4 projecitonMatrix = perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
        mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        mat4 modelScaleMatrix = scale(vec3(0.5f));
        mat4 modelRotationMatrix = rotate(mat4(), radians(5.0f), vec3(1.0f, 0.0f, 0.0f));
        mat4 modelTranslateMatrix = translate(mat4(), vec3(0.0f, 0.0f, 1.0f));
        mat4 modelMatrix = modelTranslateMatrix * modelRotationMatrix * modelScaleMatrix;
        
        fullTransformMatrix = projecitonMatrix * viewMatrix * modelMatrix;
        
        glUniformMatrix4fv(fullTransformMatrixLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);

        glBindVertexArray(vertexArrayObjectID);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
    }
    
    
    glDeleteVertexArrays(1, &vertexArrayObjectID);
    glDeleteBuffers(1, &vertexBufferObjectID);
    glDeleteBuffers(1, &elementBufferObjectID);
    glfwTerminate();
    
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

void doMovement() {
    GLfloat cameraSpeed = 2.0f * deltaTime;
    vec3 cross = normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    if (keys[GLFW_KEY_W]) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (keys[GLFW_KEY_S]) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (keys[GLFW_KEY_A]) {
        cameraPos -= cross;
    }
    if (keys[GLFW_KEY_D]) {
        cameraPos += cross;
    }
    
    if (keys[GLFW_KEY_R]) {
        cameraPos += cameraSpeed * cameraUp;
    }
    
    if (keys[GLFW_KEY_F]) {
        cameraPos -= cameraSpeed * cameraUp;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    GLfloat sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw += xoffset;
    pitch += yoffset;
    
    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;
    
    vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

GLuint LoadShaders(const char* vertexFilePath, const char* fragmentFilePath) {

    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint ProgramID = glCreateProgram();
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    string line = "";
    
    // read vertexCode
    string vertexFileCode;
    ifstream vertexFileStream;
    vertexFileStream.open(vertexFilePath);
    assert(!vertexFileStream.fail() && "Can't open vertex file");
    while (getline(vertexFileStream, line)) {
        if(vertexFileStream.fail()) {
            assert("Fail to read line");
            vertexFileStream.close();
        } else {
            vertexFileCode += "\n" + line;
        }
    }
    vertexFileStream.close();
    line = "";
    
    // read fragmentCode
    string fragmentFileCode;
    ifstream fragmentFileStream;
    fragmentFileStream.open(fragmentFilePath);
    assert(!fragmentFileStream.fail() && "Can't open fragment file");
    while (getline(fragmentFileStream, line)) {
        if(fragmentFileStream.fail()) {
            assert("Fail to read line");
            fragmentFileStream.close();
        } else {
            fragmentFileCode += "\n" + line;
        }
    }
    fragmentFileStream.close();
    
    // Compile Vertex Shader
    cout << "Compiling shader: " << vertexFilePath << endl;
    char const * vertexSourcePointer = vertexFileCode.c_str();
    glShaderSource(VertexShaderID, 1, &vertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
    
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        cout  << &VertexShaderErrorMessage[0] << endl;
        exit(EXIT_FAILURE);
    }
    
    // Compile Fragment Shader
    cout << "Compiling shader: " << fragmentFilePath << endl;
    char const * FragmentSourcePointer = fragmentFileCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        cout  << &FragmentShaderErrorMessage[0] << endl;
        exit(EXIT_FAILURE);
    }
    
    // Link the program
    cout <<  "Linking program" << endl;
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        cout  << &ProgramErrorMessage[0] << endl;
        exit(EXIT_FAILURE);
    }
    
    // Free memory
    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return ProgramID;
}

