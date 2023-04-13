#pragma once
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "loader.h"
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>"

namespace Muppet
{

    class Camera
    {
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = -90.0f, float pitch = 0.0f);

        glm::mat4 getViewMatrix();
        void LookAt(glm::vec3 p_position);
        void processKeyboard(int direction, float deltaTime);
        void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        void SetPosition(glm::vec3 p_position);
        float movementSpeed = 2.5f;
        glm::vec3 position;
    private:
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;

        float yaw;
        float pitch;

      
        float mouseSensitivity = 0.5f;

        void updateCameraVectors();
    };

    class Transform
    {
    public:
        void UpdateMatrix();
        void SetPosition(glm::vec3 p_position);
        void SetRotation(glm::vec3 p_rotation);
        void SetScale(glm::vec3 p_scale);
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
        glm::mat4 m_matrix;
    };

    class Object
    {
    public:
        std::vector<float> m_vertices;
        std::vector<float> m_normals;
        std::vector<float> m_colors;
        std::vector<float> m_instanceMatrices;
        std::vector<unsigned int> m_indices;
        GLuint m_drawMethod = GL_TRIANGLES;
        std::vector<std::shared_ptr<Transform>> m_copies;
        Transform m_transform;


        std::weak_ptr<Transform> Clone(glm::vec3 p_pos, glm::vec3 p_rot, glm::vec3 p_scale);
        void Draw(unsigned int p_matrix);
        void UpdateMatrix();
        void SetPosition(glm::vec3 p_position);
        void SetRotation(glm::vec3 p_rotation);
        void SetScale(glm::vec3 p_scale);
        void RandomColors();
        void FillColor(glm::vec3 color);
        void GenBuffers();
        void GenVertexBuffer();
        void GenColorBuffer();
        void UpdateVertexBuffer();
        void UpdateColorBuffer();
        void GenInstanceBuffer();
        void UpdateInstanceBuffer();
        void GenIndexBuffer();
        void UpdateIndexBuffer();
        void AddVertex(glm::vec3 pos);
    private:
        unsigned int m_vertexBuffer;
        unsigned int m_colorBuffer;
        unsigned int m_normalBuffer;
        unsigned int m_vao;
        unsigned int m_instanceBuffer;
        unsigned int m_indexBuffer;
    };

    class Graphics
    {
    public:
        static glm::mat4 m_projectionMatrix;
        static Camera* m_camera;
        static std::vector<std::shared_ptr<Object>> m_objects;
        static unsigned int m_instancedShaderProgram;
        static unsigned int m_defaultShaderProgram;
        static unsigned int m_defaultMatrix;
        static unsigned int m_modelMatrix;
        static unsigned int m_cameraMatrix;
        static unsigned int m_worldMatrixInstance;
        static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
        static unsigned int CompileShader(unsigned int type, const std::string& source);
        static std::string readFileIntoString2(const std::string& path);
        static unsigned int ParseShader(const std::string& filepath);
        static void Init(int p_width, int p_height);
        static std::weak_ptr<Object> CopyObject(std::weak_ptr<Object> p_object, glm::vec3 p_pos, glm::vec3 p_rot, glm::vec3 p_scale);
        static void InitCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3 lookat = glm::vec3(1.0f, 1.0f, 1.0f));
        static void SetGlobalDrawMethod(unsigned int p_drawMethod);

        static void InitProjection(const int p_width, const int p_height);
       static void Draw();
        static std::weak_ptr<Object> LoadObject(std::string p_filepath, glm::vec3 p_position = glm::vec3(0,0,0), glm::vec3 p_rotation = glm::vec3(0, 0, 0), glm::vec3 p_scale = glm::vec3(1, 1, 1));
        static std::weak_ptr<Object> CreateEmptyObject();
        bool DeleteObject(std::weak_ptr<Object> p_ptr1);
    };

    class Input
    {
    public:
        static std::vector<int> m_pressed;
        static double m_mouseX, m_mouseY;
        static double m_mouseOldX, m_mouseOldY;
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static bool IsPressed(int key);
       
    };

    /*MATH*/

    float interpolate(float a0, float a1, float w);

    glm::vec2 randomGradient(int ix, int iy);


    float dotGridGradient(int ix, int iy, float x, float y);


    float perlin(float x, float y);
  
}
