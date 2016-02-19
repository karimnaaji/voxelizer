#include "oglw.h"
#define VOXELIZER_IMPLEMENTATION
#include "../voxelizer.h"

using namespace OGLW;
using namespace std;

struct MeshVertex {
    glm::vec3 position;
    glm::vec3 normal;
};

std::vector<unique_ptr<Mesh<MeshVertex>>> meshes;
float voxelsizex = 0.03, voxelsizey = 0.03, voxelsizez = 0.03;
float precision = 0.01;

class Viewer : public App {
    public:
        Viewer() : App({"Viewer", false, false, 960, 720}) {}
        void update(float _dt) override;
        void render(float _dt) override;
        void init() override;
    private:
        unique_ptr<Shader> m_shader;
        unique_ptr<Texture> m_texture;
        float m_xrot = 0.f, m_yrot = 0.f;
};
OGLWMain(Viewer);

void dropCallback(GLFWwindow* window, int count, const char** paths) {
    INFO("%s\n", paths[0]);

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    tinyobj::LoadObj(shapes, materials, paths[0]);

    meshes.clear();
    meshes.resize(shapes.size());

    for (size_t i = 0; i < shapes.size(); i++) {
        vx_mesh_t* mesh;
        vx_mesh_t* result;

        mesh = vx_mesh_alloc(shapes[i].mesh.positions.size(), shapes[i].mesh.indices.size());

        for (size_t f = 0; f < shapes[i].mesh.indices.size(); f++) {
            mesh->indices[f] = shapes[i].mesh.indices[f];
        }

        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            mesh->vertices[v].x = shapes[i].mesh.positions[3*v+0];
            mesh->vertices[v].y = shapes[i].mesh.positions[3*v+1];
            mesh->vertices[v].z = shapes[i].mesh.positions[3*v+2];
        }

        result = vx_voxelize(mesh, voxelsizex, voxelsizey, voxelsizez, precision);

        INFO("Number of vertices: %ld\n", result->nvertices);
        INFO("Number of indices: %ld\n", result->nindices);

        static auto layout = std::shared_ptr<VertexLayout>(new VertexLayout({
            {"position", 3, GL_FLOAT, false, 0, AttributeLocation::position},
            {"normal", 3, GL_FLOAT, false, 0, AttributeLocation::normal},
        }));

        meshes[i] = std::make_unique<Mesh<MeshVertex>>(layout, GL_TRIANGLES);

        std::vector<MeshVertex> vertices;
        std::vector<int> indices(result->indices, result->indices + result->nindices);

        vertices.reserve(result->nvertices);

        for (size_t i = 0; i < result->nindices; ++i) {
            vertices.push_back({
                {result->vertices[result->indices[i]].x,
                 result->vertices[result->indices[i]].y,
                 result->vertices[result->indices[i]].z},
                {result->normals[result->normalindices[i]].x,
                 result->normals[result->normalindices[i]].y,
                 result->normals[result->normalindices[i]].z},
            });
        }

        // FIXME: use indices somehow

        meshes[i]->addVertices(std::move(vertices), {});

        vx_mesh_free(result);
        vx_mesh_free(mesh);
    }
}

void Viewer::init() {

    App::guiMode = true;

    glClearColor(1.0, 1.0, 1.0, 1.0);

    m_camera.setPosition({0.0, -0.5, 10.0});
    m_camera.setNear(5.0);
    m_camera.setFar(15.0);

    m_shader = make_unique<Shader>("default.glsl");

    glfwSetDropCallback(m_window, dropCallback);

}

void Viewer::update(float _dt) {

    if (!App::guiMode) {
        m_xrot += m_cursorX;
        m_yrot += m_cursorY;
    }

}

void Viewer::render(float _dt) {

    if (meshes.size() > 0) {
        RenderState::depthWrite(GL_TRUE);
        RenderState::depthTest(GL_TRUE);
        RenderState::culling(GL_TRUE);
        RenderState::cullFace(GL_BACK);

        glm::mat4 model, view = m_camera.getViewMatrix();

        model = glm::rotate(model, m_xrot * 1e-2f, glm::vec3(0.0, 1.0, 0.0));
        model = glm::rotate(model, m_yrot * 1e-2f, glm::vec3(1.0, 0.0, 0.0));

        glm::mat4 mvp = m_camera.getProjectionMatrix() * view * model;

        m_shader->setUniform("mvp", mvp);

        for (auto& m : meshes) {
            m->draw(*m_shader);
        }
    }

    oglwImGuiBegin();

    {
        ImGui::Text("Press ESC to switch between GUI/Viewer mode");
        ImGui::SliderFloat("voxel size x", &voxelsizex, 0.0f, 1.0f);
        ImGui::SliderFloat("voxel size y", &voxelsizey, 0.0f, 1.0f);
        ImGui::SliderFloat("voxel size z", &voxelsizez, 0.0f, 1.0f);
        ImGui::SliderFloat("precision", &precision, 0.0f, 1.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    oglwImGuiFlush();

}

