#include "oglw.h"

using namespace OGLW;
using namespace std;

class Viewer : public App {
    public:
        Viewer() : App({"Viewer", false, false, 960, 720}) {}
        void update(float _dt) override;
        void render(float _dt) override;
        void init() override;
    private:
        unique_ptr<Shader> m_shader;
        unique_ptr<Texture> m_texture;
        unique_ptr<RawMesh> m_mesh;
        float m_xrot = 0.f, m_yrot = 0.f;
};
OGLWMain(Viewer);

void dropCallback(GLFWwindow* window, int count, const char** paths) {
    INFO("%s\n", paths[0]);
}

void Viewer::init() {
    // Toggle gui mode by default
    App::guiMode = true;
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //m_camera.setPosition({0.0, 0.0, 10.0});
    //m_camera.setNear(5.0);
    //m_camera.setFar(15.0);
    //m_shader = make_unique<Shader>("default.glsl");
    //m_mesh = loadOBJ(s_tileOBJ);
    //m_texture = make_unique<Texture>(s_atlasAO);
    //RenderState::depthWrite(GL_TRUE);
    //RenderState::depthTest(GL_TRUE);
    //RenderState::culling(GL_TRUE);
    //RenderState::cullFace(GL_BACK);

    glfwSetDropCallback(m_window, dropCallback);
}

void Viewer::update(float _dt) {
    //m_xrot += m_cursorX;
    //m_yrot += m_cursorY;
}

void Viewer::render(float _dt) {
    //glm::mat4 model, view = m_camera.getViewMatrix();
    //model = glm::rotate(model, m_xrot * 1e-2f, glm::vec3(0.0, 1.0, 0.0));
    //model = glm::rotate(model, m_yrot * 1e-2f, glm::vec3(1.0, 0.0, 0.0));
    //glm::mat4 mvp = m_camera.getProjectionMatrix() * view * model;
    //m_texture->bind(0);
    //m_shader->setUniform("ao", 0);
    //m_shader->setUniform("mvp", mvp);
    //m_mesh->draw(*m_shader);

    oglwImGuiBegin();

    ImVec4 clear_color = ImColor(114, 144, 154);
    {
        static float f = 0.0f;
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }


    oglwImGuiFlush();
}

