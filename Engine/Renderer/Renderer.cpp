#include "Renderer.hpp"

#include "VertexArray.hpp"
#include "Resource/ResourceManager.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace zn
{
    Renderer::Renderer()
    {
        
    }
    
    Renderer::~Renderer()
    {
        
    }

    b8 Renderer::Init(u32 width, u32 height)
    {
        // TEMPORAL ///////////////////////////////////////
        if (auto shader = ResourceManager::LoadShader("Default Shader", "Content/Shaders/default.vert", "Content/Shaders/default.frag"))
        {
            m_basicShader = shader.value();
        }

        if (auto lightDebugCubeShader = ResourceManager::LoadShader("Light Cube Shader", "Content/Shaders/light_cube.vert", "Content/Shaders/light_cube.frag"))
        {
            m_lightDebugCubeShader = lightDebugCubeShader.value();
        }

        if (auto lightingShader = ResourceManager::LoadShader("Lighting Shader", "Content/Shaders/lighting.vert", "Content/Shaders/lighting.frag"))
        {
            m_lightingShader = lightingShader.value();
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
        vertexBuffer.SetData(vertices.data(), vertices.size() * sizeof(f32));
		
        VertexBufferLayout vertexBufferLayout;
        vertexBufferLayout.PushElement<f32>(3); // positions
        vertexBufferLayout.PushElement<f32>(2); // texture coords

        m_vertexArray->AddVertexBuffer(vertexBuffer, vertexBufferLayout);

        //IndexBuffer indexBuffer{};
        //indexBuffer.Bind();
        //indexBuffer.SetData(indices, 6);
		
        vertexBuffer.Unbind();
        m_vertexArray->Unbind();

        //// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
       // indexBuffer.Unbind();

        // LIGHTING EXAMPLE SETUP
        // ------------------------------------------------
        
        m_lightDegugCubeVA = CreateUnique<VertexArray>();
        m_lightDegugCubeVA->Bind();

        VertexBuffer lightDebugCubeVB{};
        lightDebugCubeVB.Bind();
        lightDebugCubeVB.SetData(lightCubeVerts.data(), lightCubeVerts.size() * sizeof(f32));
		
        VertexBufferLayout lightDebugCubeVBL;
        lightDebugCubeVBL.PushElement<f32>(3); // positions

        m_lightDegugCubeVA->AddVertexBuffer(lightDebugCubeVB, lightDebugCubeVBL);

        lightDebugCubeVB.Unbind();
        m_lightDegugCubeVA->Unbind();

        // --------------------------------
        
        m_lightingCubeVA = CreateUnique<VertexArray>();
        m_lightingCubeVA->Bind();

        VertexBuffer lightingCubeVB{};
        lightingCubeVB.Bind();
        lightingCubeVB.SetData(lightCubeVerts.data(), lightCubeVerts.size() * sizeof(f32));
		
        VertexBufferLayout lightingCubeVBL;
        lightingCubeVBL.PushElement<f32>(3); // positions

        m_lightingCubeVA->AddVertexBuffer(lightingCubeVB, lightingCubeVBL);

        lightingCubeVB.Unbind();
        m_lightingCubeVA->Unbind();
        
        return true;
    }

    void Renderer::Shutdown()
    {
        
    }

    void Renderer::ClearScreen(f32 r, f32 g, f32 b, f32 a) const
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::TexturedCubesExample(const Camera& camera) const
    {
        m_basicShader->Bind();
        m_basicShader->SetInt("texture1", 0);
        m_basicShader->SetInt("texture2", 1);
        m_basicShader->SetMat4("view", camera.GetViewMatrix());
        m_basicShader->SetMat4("projection", camera.GetProjection());
        
        m_wallTexture->Bind();
        m_georgeTexture->Bind(1);
        m_vertexArray->Bind();
        
        int counter = 0;
        for(uSize i = 0; i < 10; i++)
        {
            math::m4 model = math::m4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            f32 angle = 20.0f * i; 
            model = glm::rotate(model, glm::radians(angle), math::v3(1.0f, 0.3f, 0.5f));
        
            if(i == 0 || counter == 3)
            {
                model = glm::rotate(model, glm::radians(f32(glfwGetTime() * 120.0f)), math::v3(1.0f, 1.0f, 1.0f));
                counter = 0;
            }
        
            m_basicShader->SetMat4("model", model);
        
            glDrawArrays(GL_TRIANGLES, 0, 36);
        
            counter++;
        }

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void Renderer::LightingExample(const Camera& camera) const
    {
        math::v3 lightDebugCubePos(0.0f, 0.0f, 0.0f);
        
        math::m4 lightModel = math::m4(1.0f);
        lightModel = glm::translate(lightModel, lightDebugCubePos);
        lightModel = glm::scale(lightModel, math::v3(.6f));

        m_lightDebugCubeShader->Bind();
        m_lightDebugCubeShader->SetMat4("model", lightModel);
        m_lightDebugCubeShader->SetMat4("view", camera.GetViewMatrix());
        m_lightDebugCubeShader->SetMat4("projection", camera.GetProjection());

        m_lightDegugCubeVA->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        m_lightDegugCubeVA->Unbind();

        math::v3 cubePos(3.0f, 0.0f, 0.0f);
        
        math::m4 cubeModel = math::m4(1.0f);
        cubeModel = glm::translate(cubeModel, cubePos);

        m_lightingShader->Bind();
        m_lightingShader->SetMat4("model", cubeModel);
        m_lightingShader->SetMat4("view", camera.GetViewMatrix());
        m_lightingShader->SetMat4("projection", camera.GetProjection());
        m_lightingShader->SetVec3("lightColor",  { 1.0f, 1.0f, 1.0f });
        m_lightingShader->SetVec3("objectColor", { 0.0f, 1.0f, 1.0f });

        m_lightingCubeVA->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        m_lightingCubeVA->Unbind();
    }

    void Renderer::Render(const Camera& camera) const
    {
        ClearScreen(0.3f, 0.3f, 0.3f, 1.0f);

        //TexturedCubesExample(camera, projection);
        LightingExample(camera);
    }
}
