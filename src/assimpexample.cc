#include <cstdio>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <boost/range/irange.hpp>

static GLuint LoadShaders(const char *vertexFilePath, const char *fragFilePath)
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream(vertexFilePath, std::ios::in);
    if (vertexShaderStream.is_open())
    {
        std::string line = "";

        while (getline(vertexShaderStream, line))
        {
            vertexShaderCode += "\n" + line;
        }

        vertexShaderStream.close();
    }
    else
    {
        printf("Impossible to open %s. Are you in the right directory?\n", vertexFilePath);
        getchar();
        return 0;
    }

    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(fragFilePath, std::ios::in);
    if (fragmentShaderStream.is_open())
    {
        std::string line = "";
        while (getline(fragmentShaderStream, line))
        {
            fragmentShaderCode += "\n" + line;
        }
        fragmentShaderStream.close();
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    printf("Compiling shader : %s\n", vertexFilePath);
    const char *vertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexSourcePointer, nullptr);
    glCompileShader(vertexShaderID);

    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertexShaderID, infoLogLength, nullptr, &vertexShaderErrorMessage[0]);
        printf("%s\n", &vertexShaderErrorMessage[0]);
    }

    printf("Compiling shader : %s\n", fragFilePath);
    const char *fragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, nullptr);
    glCompileShader(fragmentShaderID);

    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, nullptr, &fragmentShaderErrorMessage[0]);
        printf("%s\n", &fragmentShaderErrorMessage[0]);
    }

    printf("Linking program\n");
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, nullptr, &programErrorMessage[0]);
        printf("%s\n", &programErrorMessage[0]);
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

int main()
{
    GLFWwindow *window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifdef WIN32
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using OpenGL %s\n", glGetString(GL_VERSION));
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    GLuint programID = LoadShaders("resources/shaders/basic.vert", "resources/shaders/basic.frag");

    // Transforms
    GLuint worldMatrixID, cameraMatrixID;
    glm::mat4 world, camera;

    worldMatrixID = glGetUniformLocation(programID, "world");
    cameraMatrixID = glGetUniformLocation(programID, "camera");

    {
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 model = glm::mat4(1.0f);

        world = model;
        camera = proj * view;
    }

    glClearColor(0, 0, 0.3f, 0.0f);

    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile("resources/meshes/sphere.fbx",
        aiProcess_CalcTangentSpace      |
        aiProcess_Triangulate           |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);
    if (!scene)
    {
        fprintf(stderr, "%s\n", importer.GetErrorString());
        return 1;
    }

    GLuint vertexBufferID;
    GLuint indexBufferID;
    GLsizei numIndices = 0;
    
    if (scene->HasMeshes())
    {
        for (auto mi : boost::irange(0U, scene->mNumMeshes))
        {
            auto mesh = scene->mMeshes[mi];

            if (mesh->HasPositions())
            {
                glGenBuffers(1, &vertexBufferID);
                glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
                glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
            }
            
            if (mesh->HasFaces())
            {
                std::vector<uint16_t> indices;

                for (auto fi : boost::irange(0U, mesh->mNumFaces))
                {
                    indices.insert(indices.end(),
                        mesh->mFaces[fi].mIndices,
                        mesh->mFaces[fi].mIndices + mesh->mFaces[fi].mNumIndices);
                    numIndices += mesh->mFaces[fi].mNumIndices;
                }

                glGenBuffers(1, &indexBufferID);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
            }
        }
    }

    glUseProgram(programID);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDisableVertexAttribArray(0);
    glUseProgram(0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        glUniformMatrix4fv(worldMatrixID, 1, GL_FALSE, &world[0][0]);
        glUniformMatrix4fv(cameraMatrixID, 1, GL_FALSE, &camera[0][0]);

        glBindVertexArray(vertexArrayID);

        glEnableVertexAttribArray(0);

        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, nullptr);

        glDisableVertexAttribArray(0);

        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteBuffers(1, &indexBufferID);

    glDeleteProgram(programID);

    glfwTerminate();

    return 0;
}