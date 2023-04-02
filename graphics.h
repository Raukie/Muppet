#pragma once
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "loader.h"
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>

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
    private:
        glm::vec3 position;
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;

        float yaw;
        float pitch;

        float movementSpeed = 2.5f;
        float mouseSensitivity = 0.1f;

        void updateCameraVectors();
    };

    class Object
    {
    public:
        std::vector<float> m_vertices;
        std::vector<float> m_normals;
        std::vector<float> m_colors;
        std::vector<unsigned int> m_indices;
        GLuint m_drawMethod;
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
        glm::mat4 m_matrix;

        void Draw(glm::mat4 p_mvp, unsigned int p_matrix);
        void UpdateMatrix();
        void SetPosition(glm::vec3 p_position);
        void SetRotation(glm::vec3 p_rotation);
        void SetScale(glm::vec3 p_scale);
        void RandomColors();
        void GenBuffers();
        void GenVertexBuffer();
        void GenColorBuffer();
        void UpdateVertexBuffer();
        void UpdateColorBuffer();

    private:
        unsigned int m_vertexBuffer;
        unsigned int m_colorBuffer;
        unsigned int m_normalBuffer;

       

    };

    class Graphics
    {
    public:
        static glm::mat4 m_projectionMatrix;
        static Camera* m_camera;
        static std::vector<std::shared_ptr<Object>> m_objects;
        static unsigned int m_defaultShaderProgram;
        static unsigned int m_defaultMatrix;
        static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
        static unsigned int CompileShader(unsigned int type, const std::string& source);
        static std::string readFileIntoString2(const std::string& path);
        static unsigned int ParseShader(const std::string& filepath);
        static void Init(int p_width, int p_height);

        static void InitCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3 lookat = glm::vec3(1.0f, 1.0f, 1.0f));

        static void InitProjection(const int p_width, const int p_height);
       static void Draw();
        static int LoadObject(std::string p_filepath, glm::vec3 p_position = glm::vec3(0,0,0), glm::vec3 p_rotation = glm::vec3(0, 0, 0), glm::vec3 p_scale = glm::vec3(1, 1, 1));

    };

    class Input
    {
    public:
        static std::vector<int> m_pressed;
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static bool IsPressed(int key);
    };

  
}
