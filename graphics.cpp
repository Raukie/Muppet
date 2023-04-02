#include "graphics.h"
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

#pragma region Initialising Variables
    Camera* Graphics::m_camera = nullptr;
    glm::mat4 Graphics::m_projectionMatrix = glm::mat4(1);
    std::vector<std::shared_ptr<Object>> Graphics::m_objects;
    unsigned int Graphics::m_defaultMatrix = 0;
    unsigned int Graphics::m_defaultShaderProgram = 0;
    std::vector<int> Input::m_pressed(400,0);
    double Input::m_mouseX = 0;
    double Input::m_mouseY = 0;
    double Input::m_mouseOldX = 0;
    double Input::m_mouseOldY = 0;
#pragma endregion

    /*CAMERA*/
    Camera::Camera(glm::vec3 position, glm::vec3 up,
        float yaw, float pitch) :
        position(position), worldUp(up), yaw(yaw), pitch(pitch)
    {
        updateCameraVectors();
    }

    void Camera::SetPosition(glm::vec3 p_position)
    {
        Camera::position = p_position;
        Camera::updateCameraVectors();
    }

    void Camera::LookAt(glm::vec3 p_position)
    {
        glm::vec3 direction = glm::normalize(p_position - Camera::position);

        Camera::yaw = glm::degrees(std::atan2(direction.z, direction.x));
        Camera::pitch = glm::degrees(std::asin(direction.y));
        Camera::updateCameraVectors();
    }

    glm::mat4 Camera::getViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    void Camera::processKeyboard(int direction, float deltaTime)
    {
        float velocity = movementSpeed * deltaTime;
        if (direction == 0) position += front * velocity;
        if (direction == 1) position -= front * velocity;
        if (direction == 2) position -= right * velocity;
        if (direction == 3) position += right * velocity;
    }

    void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch)
        {
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void Camera::updateCameraVectors()
    {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

    /*OBJECT*/
    void Object::Draw(glm::mat4 p_mvp, unsigned int p_matrix)
    {
        Object::UpdateMatrix();
        p_mvp = p_mvp * Object::m_matrix;
        glUniformMatrix4fv(p_matrix, 1, GL_FALSE, &p_mvp[0][0]);

        glDrawElements(GL_TRIANGLES, Object::m_indices.size(), GL_UNSIGNED_INT, Object::m_indices.data());
    }

    void Object::UpdateMatrix()
    {
        glm::mat4 Model(1.0f);
        Model = glm::translate(Model, Object::m_position);
        Model = glm::rotate(Model, glm::radians(Object::m_rotation.x), glm::vec3(1, 0, 0));
        Model = glm::rotate(Model, glm::radians(Object::m_rotation.y), glm::vec3(0, 1, 0));
        Model = glm::rotate(Model, glm::radians(Object::m_rotation.z), glm::vec3(0, 0, 1));
        Model = glm::scale(Model, Object::m_scale);
        Object::m_matrix = Model;
    }

    void Object::SetPosition(glm::vec3 p_position)
    {
        Object::m_position = p_position;
        Object::UpdateMatrix();
    }

    void Object::SetRotation(glm::vec3 p_rotation)
    {
        Object::m_rotation = p_rotation;
        Object::UpdateMatrix();
    }

    void Object::SetScale(glm::vec3 p_scale)
    {
        Object::m_scale = p_scale;
        Object::UpdateMatrix();
    }

    void Object::GenBuffers()
    {
        Object::GenVertexBuffer();
        Object::GenColorBuffer();
        Object::UpdateVertexBuffer();
        Object::UpdateColorBuffer();
    }

    void Object::GenVertexBuffer()
    {
        glGenBuffers(1, &m_vertexBuffer);
    }

    void Object::GenColorBuffer()
    {
        glGenBuffers(1, &m_colorBuffer);
    }

    void Object::UpdateVertexBuffer()
    {
        glBindBuffer(GL_ARRAY_BUFFER, Object::m_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, Object::m_vertices.size() * sizeof(float), Object::m_vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Object::UpdateColorBuffer()
    {
        glBindBuffer(GL_ARRAY_BUFFER, Object::m_colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, Object::m_colors.size() * sizeof(float), Object::m_colors.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Object::RandomColors()
    {
        Object::m_colors.clear();
        for (int i = 0; i < Object::m_vertices.size(); i++)
        {
            Object::m_colors.push_back(rand() % 100 / 100.0f);
        }
    }

    /*GRAPHICS*/
    void Graphics::InitCamera(glm::vec3 position, glm::vec3 up,
        glm::vec3 p_target)
    {
        Graphics::m_camera = new Camera(position, up);
        Graphics::m_camera->LookAt(p_target);
    }

    /*
    SHADERS
    */
    void Graphics::InitProjection(const int p_width, const int p_height)
    {
        Graphics::m_projectionMatrix = glm::perspective(
            45.0f,
            (float)p_width / (float)p_height,
            0.1f,
            100.0f
        );

    }

    int Graphics::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        unsigned int program = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    unsigned int Graphics::ParseShader(const std::string& filepath)
    {
        std::string shaderFile = readFileIntoString2(filepath);
        unsigned vertexFound = shaderFile.find("#VERTEX", 0);
        unsigned vertexEnd = shaderFile.find("#VERTEXEND", 0);

        unsigned fragmentFound = shaderFile.find("#FRAGMENT", 0);
        unsigned fragmentEnd = shaderFile.find("#FRAGMENTEND", 0);

        std::string vertexShader = shaderFile.substr(vertexFound + 7, vertexEnd - vertexFound - 7);
        std::string fragmentShader = shaderFile.substr(fragmentFound + 10, fragmentEnd - fragmentFound - 10);

        return CreateShader(vertexShader, fragmentShader);
    }

    std::string Graphics::readFileIntoString2(const std::string& path)
    {
        auto ss = std::ostringstream{};
        std::ifstream input_file(path);
        if (!input_file.is_open())
        {
            std::cerr << "Could not open the file - '"
                << path << "'" << std::endl;
            exit(EXIT_FAILURE);
        }
        ss << input_file.rdbuf();
        return ss.str();
    }

    unsigned int Graphics::CompileShader(unsigned int type, const std::string& source)
    {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile shaders, Shader: " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << std::endl;
            std::cout << message << std::endl;
            glDeleteShader(id);
            return -1;
        }

        return id;
    }

    void Graphics::Init(int p_width, int p_height)
    {
        Graphics::InitCamera();
        Graphics::InitProjection(p_width, p_height);
        Graphics::m_defaultShaderProgram = Graphics::ParseShader("Basic.shader");
        glUseProgram(Graphics::m_defaultShaderProgram);
        Graphics::m_defaultMatrix = glGetUniformLocation(Graphics::m_defaultShaderProgram, "MVP");
    }

    void Graphics::Draw()
    {
        glm::mat4 matrix = Graphics::m_projectionMatrix * Graphics::m_camera->getViewMatrix();
        for (int i = 0; i < Graphics::m_objects.size(); i++)
        {
            Graphics::m_objects[i]->Draw(matrix, Graphics::m_defaultMatrix);
        }
    }

    int Graphics::LoadObject(std::string p_filepath, glm::vec3 p_position, glm::vec3 p_rotation, glm::vec3 p_scale)
    {
        Object object;
        objl::Loader loader;
        loader.LoadFile(p_filepath);
        for (int i = 0; i < loader.LoadedVertices.size(); i++)
        {
            object.m_vertices.push_back(loader.LoadedVertices[i].Position.X);
            object.m_vertices.push_back(loader.LoadedVertices[i].Position.Y);
            object.m_vertices.push_back(loader.LoadedVertices[i].Position.Z);
        }

        for (int i = 0; i < loader.LoadedIndices.size(); i++)
        {
            object.m_indices.push_back(loader.LoadedIndices[i]);
        }
        loader.~Loader();

        object.RandomColors();
        object.m_position = p_position;
        object.m_rotation = p_rotation;
        object.m_drawMethod = GL_TRIANGLES;
        object.m_scale = p_scale;
        object.UpdateMatrix();
        object.GenBuffers();
        object.UpdateColorBuffer();
        object.UpdateVertexBuffer();

        Graphics::m_objects.push_back(std::make_shared<Object>(object));

        /*TODO MUST RETURN WEAK POINTER INSTEAD OF INDEX*/
        return Graphics::m_objects.size() - 1;
    }

    void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Input::m_pressed[key] = action;
    }

    bool Input::IsPressed(int key)
    {
        return (bool)Input::m_pressed[key];
    }

}
