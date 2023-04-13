#include "graphics.h"
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "loader.h"
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>"


#define MATRICES_LOCATION 7

namespace Muppet
{

#pragma region Initialising Variables
    Camera* Graphics::m_camera = nullptr;
    glm::mat4 Graphics::m_projectionMatrix = glm::mat4(1);
    std::vector<std::shared_ptr<Object>> Graphics::m_objects;
    unsigned int Graphics::m_defaultMatrix = 0;
    unsigned int Graphics::m_defaultShaderProgram = 0;
    unsigned int Graphics::m_cameraMatrix = 0;
    unsigned int Graphics::m_modelMatrix = 0;
    unsigned int Graphics::m_instancedShaderProgram = 0;
    unsigned int Graphics::m_worldMatrixInstance = 0;    
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

    /*TRANSFORM*/

    void Transform::UpdateMatrix()
    {
        glm::mat4 Model(1.0f);
        Model = glm::translate(Model, Transform::m_position);
        Model = glm::rotate(Model, glm::radians(Transform::m_rotation.x), glm::vec3(1, 0, 0));
        Model = glm::rotate(Model, glm::radians(Transform::m_rotation.y), glm::vec3(0, 1, 0));
        Model = glm::rotate(Model, glm::radians(Transform::m_rotation.z), glm::vec3(0, 0, 1));
        Model = glm::scale(Model, Transform::m_scale);
        Transform::m_matrix = Model;
    }

    void Transform::SetPosition(glm::vec3 p_position)
    {
        Transform::m_position = p_position;
        Transform::UpdateMatrix();
    }

    void Transform::SetRotation(glm::vec3 p_rotation)
    {
        Transform::m_rotation = p_rotation;
        Transform::UpdateMatrix();
    }

    void Transform::SetScale(glm::vec3 p_scale)
    {
        Transform::m_scale = p_scale;
        Transform::UpdateMatrix();
    }

    /*OBJECT*/
    void Object::Draw(unsigned int p_matrix)
    {
        glBindVertexArray(Object::m_vao);
       
        if (Object::m_copies.size() > 0)
        {
            glUseProgram(Graphics::m_instancedShaderProgram);
            glDrawElementsInstanced(Object::m_drawMethod, Object::m_indices.size(), GL_UNSIGNED_INT, (void*)0, (GLsizei)Object::m_copies.size());
        }
        else
        {
            glUseProgram(Graphics::m_defaultShaderProgram);
            glUniformMatrix4fv(p_matrix, 1, GL_FALSE, &Object::m_transform.m_matrix[0][0]);
            glDrawElements(Object::m_drawMethod, Object::m_indices.size(), GL_UNSIGNED_INT, (void*)0);
        }
       
        glBindVertexArray(0);
    }

    std::weak_ptr<Transform> Object::Clone(glm::vec3 p_pos, glm::vec3 p_rot, glm::vec3 p_scale)
    {
        Transform transform;
        transform.m_position = p_pos;
        transform.m_rotation = p_rot;
        transform.m_scale = p_scale;
        transform.UpdateMatrix();
        Object::m_copies.push_back(std::make_shared<Transform>(transform));
        return Object::m_copies[Object::m_copies.size() - 1];
    }

    void Object::UpdateMatrix()
    {
        Object::m_transform.UpdateMatrix();
    }

    void Object::SetPosition(glm::vec3 p_position)
    {
        Object::m_transform.m_position = p_position;
        Object::m_transform.UpdateMatrix();
    }

    void Object::SetRotation(glm::vec3 p_rotation)
    {
        Object::m_transform.m_rotation = p_rotation;
        Object::m_transform.UpdateMatrix();
    }

    void Object::SetScale(glm::vec3 p_scale)
    {
        Object::m_transform.m_scale = p_scale;
        Object::UpdateMatrix();
    }

    void Object::GenBuffers()
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(Object::m_vao);
        Object::GenVertexBuffer();
        Object::GenColorBuffer();
        Object::GenIndexBuffer();
        Object::UpdateVertexBuffer();
        Object::UpdateColorBuffer();
        Object::UpdateIndexBuffer();
        glBindVertexArray(0);
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

    void Object::FillColor(glm::vec3 color)
    {
        for (int i = 0; i < Object::m_vertices.size(); i+=3)
        {
            Object::m_colors.push_back(color.x / 255.0f);
            Object::m_colors.push_back(color.y / 255.0f);
            Object::m_colors.push_back(color.z / 255.0f);
        }
    }

    void Object::RandomColors()
    {
        Object::m_colors.clear();

        for (int i = 0; i < Object::m_vertices.size(); i+=9)
        {
            float a = rand() % 100 / 100.0f;
            float b = rand() % 100 / 100.0f;
            float c = rand() % 100 / 100.0f;
            Object::m_colors.push_back(a);
            Object::m_colors.push_back(b);
            Object::m_colors.push_back(c);
            Object::m_colors.push_back(a);
            Object::m_colors.push_back(b);
            Object::m_colors.push_back(c);
            Object::m_colors.push_back(a);
            Object::m_colors.push_back(b);
            Object::m_colors.push_back(c);

        }
        
    }

    void Object::GenInstanceBuffer()
    {
        glBindVertexArray(Object::m_vao);
        glGenBuffers(1, &m_instanceBuffer);
        UpdateInstanceBuffer();
        glBindVertexArray(0);
    }

    void Object::UpdateInstanceBuffer()
    {
        glBindVertexArray(Object::m_vao);
        Object::m_instanceMatrices.clear();
        for (int i = 0; i < Object::m_copies.size(); i++)
        {
            const float* pSource = (const float*)glm::value_ptr(Object::m_copies[i]->m_matrix);
            for (int j = 0; j < 16; j++) 
            {
                Object::m_instanceMatrices.push_back(pSource[j]);
            }
              
        }
      
        glBindBuffer(GL_ARRAY_BUFFER, Object::m_instanceBuffer);
        glBufferData(GL_ARRAY_BUFFER, Object::m_instanceMatrices.size() * sizeof(float),
            Object::m_instanceMatrices.data(), GL_STATIC_DRAW);
        for (unsigned int i = 0; i < 4; i++)
        {
            glEnableVertexAttribArray(MATRICES_LOCATION + i);
            glVertexAttribPointer(MATRICES_LOCATION + i, 4, GL_FLOAT, GL_FALSE,
                16 * sizeof(float),
                (const GLvoid*)(sizeof(GLfloat) * i * 4));
            glVertexAttribDivisor(MATRICES_LOCATION + i, 1);
        }
        glBindVertexArray(0);
    }

    void Object::GenIndexBuffer()
    {
        glGenBuffers(1, &m_indexBuffer);
    }

    void Object::UpdateIndexBuffer()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Object::m_indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Object::m_indices.size() * sizeof(unsigned int), &Object::m_indices[0], GL_STATIC_DRAW);
    }
    
    void Object::AddVertex(glm::vec3 pos)
    {
        Object::m_vertices.push_back(pos.x);
        Object::m_vertices.push_back(pos.y);
        Object::m_vertices.push_back(pos.z);
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
            10000.0f
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
        Graphics::m_modelMatrix = glGetUniformLocation(Graphics::m_defaultShaderProgram, "MODEL");
        Graphics::m_cameraMatrix = glGetUniformLocation(Graphics::m_defaultShaderProgram, "PROJECTION");
        
        Graphics::m_instancedShaderProgram = Graphics::ParseShader("Instance.shader");
        glUseProgram(Graphics::m_instancedShaderProgram);
        Graphics::m_worldMatrixInstance = glGetUniformLocation(Graphics::m_instancedShaderProgram, "PROJECTION");

        glUniformMatrix4fv(Graphics::m_defaultMatrix, 1, GL_FALSE, &Graphics::m_projectionMatrix[0][0]);
    }

    void Graphics::Draw()
    {
        glm::mat4 matrix = Graphics::m_projectionMatrix * Graphics::m_camera->getViewMatrix();
        
        glUseProgram(Graphics::m_defaultShaderProgram);
        glUniformMatrix4fv(Graphics::m_cameraMatrix, 1, GL_FALSE, &matrix[0][0]);
        glUseProgram(Graphics::m_instancedShaderProgram);
        glUniformMatrix4fv(Graphics::m_worldMatrixInstance, 1, GL_FALSE, &matrix[0][0]);

        for (int i = 0; i < Graphics::m_objects.size(); i++)
        {
            Graphics::m_objects[i]->Draw(Graphics::m_modelMatrix);
        }
    }

    std::weak_ptr<Object> Graphics::LoadObject(std::string p_filepath, glm::vec3 p_position, glm::vec3 p_rotation, glm::vec3 p_scale)
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
        object.m_transform.m_position = p_position;
        object.m_transform.m_rotation = p_rotation;
        object.m_transform.m_scale = p_scale;
        object.UpdateMatrix();
        object.GenBuffers();
        object.UpdateColorBuffer();
        object.UpdateVertexBuffer();

        Graphics::m_objects.push_back(std::make_shared<Object>(object));

        return Graphics::m_objects[Graphics::m_objects.size()-1];
    }

    std::weak_ptr<Object> Graphics::CreateEmptyObject()
    {
        Object obj;
        Graphics::m_objects.push_back(std::make_shared<Object>(obj));
        return Graphics::m_objects[Graphics::m_objects.size() - 1];
    }

    std::weak_ptr<Object> Graphics::CopyObject(std::weak_ptr<Object> p_object, glm::vec3 p_pos, glm::vec3 p_rot, glm::vec3 p_scale)
    {
        std::shared_ptr<Object> object = p_object.lock();
        Object copy = *object;
        copy.GenBuffers();
        copy.m_transform.m_position = p_pos;
        copy.m_transform.m_rotation = p_rot;
        copy.m_transform.m_scale = p_scale;
        copy.UpdateMatrix();
        Graphics::m_objects.push_back(std::make_shared<Object>(copy));
        return Graphics::m_objects[Graphics::m_objects.size() - 1];
    }
    
    bool Graphics::DeleteObject(std::weak_ptr<Object> p_ptr1)
    {
        for (int i = 0; i < Graphics::m_objects.size(); i++)
            if (Graphics::m_objects[i] == p_ptr1.lock())
                return true;
        return false;
    }

    void Graphics::SetGlobalDrawMethod(unsigned int p_drawMethod)
    {
        for (int i = 0; i < Graphics::m_objects.size(); i++)
        {
            std::shared_ptr<Object> obj = Graphics::m_objects[i];
            obj->m_drawMethod = p_drawMethod;
        }
    }

    /*INPUT*/

    void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Input::m_pressed[key] = action;
    }

    bool Input::IsPressed(int key)
    {
        return (bool)Input::m_pressed[key];
    }

    /*MATH*/

    float interpolate(float a0, float a1, float w)
    {

        return (a1 - a0) * w + a0;
  
    }

    glm::vec2 randomGradient(int ix, int iy)
    {
        const unsigned w = 8 * sizeof(unsigned);
        const unsigned s = w / 2; // rotation width
        unsigned a = ix, b = iy;
        a *= 3284157443; b ^= a << s | a >> w - s;
        b *= 1911520717; a ^= b << s | b >> w - s;
        a *= 2048419325;
        float random = a * (3.14159265 / ~(~0u >> 1)); 
        glm::vec2 v;
        v.x = cos(random); v.y = sin(random);
        return v;
    }


    float dotGridGradient(int ix, int iy, float x, float y)
    {

        glm::vec2 gradient = randomGradient(ix, iy);


        float dx = x - (float)ix;
        float dy = y - (float)iy;

 
        return (dx * gradient.x + dy * gradient.y);
    }


    float perlin(float x, float y)
    {

        int x0 = (int)floor(x);
        int x1 = x0 + 1;
        int y0 = (int)floor(y);
        int y1 = y0 + 1;

        float sx = x - (float)x0;
        float sy = y - (float)y0;

        float n0, n1, ix0, ix1, value;

        n0 = dotGridGradient(x0, y0, x, y);
        n1 = dotGridGradient(x1, y0, x, y);
        ix0 = interpolate(n0, n1, sx);

        n0 = dotGridGradient(x0, y1, x, y);
        n1 = dotGridGradient(x1, y1, x, y);
        ix1 = interpolate(n0, n1, sx);

        value = interpolate(ix0, ix1, sy);
        return value;
    }
}
