#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include "lab_m1/Tema2/MeshData.h"
#include "lab_m1/Tema2/MyCamera.h"
#include "lab_m1/Tema2/WorldGen/BlockWorld.h"

struct CarState {
    glm::vec3 pos;
    int dirx, dirz;
    float angle;
    bool rotating;
    int keyPressed;
};

struct SavedCoords {
    int dirx;
    int dirz;
    float angle;
};

namespace m1 {
class Tema2 : public gfxc::SimpleScene {
  public:
    struct ViewportArea {
        ViewportArea() : x(0), y(0), width(1), height(1) {}
        ViewportArea(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}
        int x;
        int y;
        int width;
        int height;
    };

    Tema2();
    ~Tema2();

    void Init() override;

  private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void RenderMesh(Mesh *mesh, Shader *shader,
                    const glm::mat4 &modelMatrix) override;
    void DrawScene1(float deltaTimeSeconds);
    void DrawScene2(float deltaTimeSeconds);

    void CreateMesh(const char *name, const std::vector<VertexFormat> &vertices,
                    const std::vector<unsigned int> &indices,
                    GLenum polygonMode);

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button,
                           int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX,
                       int offsetY) override;
    void OnWindowResize(int width, int height) override;

  private:
    void UpdateTrain(float deltaTime);
    void UpdateCar(string name, float deltaTime);
    vector<int> generateCommand();
    gfxc::TextRenderer *textRenderer;
    implemented::MyCamera *camera;
    implemented::MyCamera *miniMap;
    glm::mat4 projectionMatrix;

    MeshData p;
    BlockWorld *b;

    glm::vec3 trainPos;
    glm::vec3 car1Pos;
    glm::vec3 car2Pos;
    glm::vec3 trainFic;
    glm::vec3 deltaPos;
    map<string, CarState> carStates;
    map<pair<int, int>, SavedCoords> savedCoords;
    int dirx;
    int dirz;
    int keyPressed;
    float angle = 0.0f;
    int idx = 0;
    vector<pair<int, int>> special_coords;
    vector<float> time_start;
    vector<float> time_elapsed;
    float timer = 0.0f;
    float max_time = 240.0f;
    ViewportArea miniViewportArea;

    float trainSpeed = 10.0f;
    bool autoMove = false;
    bool rotate = false;
    bool waiting = false;
    bool possible = false;
    vector<int> delivery;
};
} // namespace m1
