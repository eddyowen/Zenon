#include "Renderer.h"

#include "VertexArray.h"

#include "Resource/ResourceManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace zn
{
    bool Renderer::Init()
    {
        // TEMPORAL ///////////////////////////////////////
        if (auto shader = ResourceManager::LoadShader("Basic Shader", "Content/Shaders/vertex.glsl", "Content/Shaders/fragment.glsl"))
        {
            m_basicShader = shader.value();
        }

        if (auto wallTexture = ResourceManager::LoadTexture("Wall", "Content/Textures/wall.jpg"))
        {
            m_wallTexture = wallTexture.value();
        }

        if (auto georgeTexture = ResourceManager::LoadTexture("George", "Content/Textures/george.jpg"))
        {
            m_georgeTexture = georgeTexture.value();
        }

        m_vertexArray = CreateUnique<zn::VertexArray>();
        m_vertexArray->Bind();

        VertexBuffer vertexBuffer{};
        vertexBuffer.Bind();
        vertexBuffer.SetData(vertices.data(), vertices.size() * sizeof(float));
		
        VertexBufferLayout vertexBufferLayout;
        vertexBufferLayout.PushElement<float>(3); // positions
        vertexBufferLayout.PushElement<float>(2); // texture coords

        m_vertexArray->AddVertexBuffer(vertexBuffer, vertexBufferLayout);

        //IndexBuffer indexBuffer{};
        //indexBuffer.Bind();
        //indexBuffer.SetData(indices, 6);
		
        vertexBuffer.Unbind();
		
        m_vertexArray->Unbind();
        
        //// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
       // indexBuffer.Unbind();
        
        return true;
    }

    void Renderer::Shutdown()
    {
        
    }

    void Renderer::ClearScreen(float r, float g, float b, float a) const
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Render() const
    {
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 1200.0f / 1200.0f, 0.1f, 100.0f);

        m_wallTexture->Bind();
        m_basicShader->Bind();
        m_basicShader->SetInt("texture1", 0);
        m_basicShader->SetInt("texture2", 1);
        //m_basicShader->SetMat4("model", model);
        m_basicShader->SetMat4("view", view);
        m_basicShader->SetMat4("projection", projection);

        m_wallTexture->Bind();
        m_georgeTexture->Bind(1);
        m_vertexArray->Bind();

        int counter = 0;
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; 
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            if(i == 0 || counter == 3)
            {
                model = glm::rotate(model, glm::radians(float(glfwGetTime() * 120.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
                counter = 0;
            }

            m_basicShader->SetMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);

            counter++;
        }

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
