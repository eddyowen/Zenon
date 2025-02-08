#pragma once

#include "Core/Base.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace zn
{
    class Shader;
    class Texture;
    class VertexArray;
    
    class Renderer
    {
    public:
        Renderer() = default;
        ~Renderer() = default;

        bool Init();
        void Shutdown();
        
        void ClearScreen(float r, float g, float b, float a) const;
        void Render() const;
        
    private:
        // TEMPORAL ///////////////////////////////////////
        SharedPtr<Shader> m_basicShader;
        UniquePtr<VertexArray> m_vertexArray;

        SharedPtr<Texture> m_wallTexture;
        SharedPtr<Texture> m_georgeTexture;
		
        glm::mat4 m_transform;
		
        unsigned int VBO, EBO, VAO;

        float vertices[32] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
           -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
           -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
       };

        unsigned int indices[6] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
        };
    };
}
