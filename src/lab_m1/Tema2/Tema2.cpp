#include "components/text_renderer.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "lab_m1/Tema2/DrawUtils.h"
#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/WorldGen/BlockWorld.h"
#include "utils/gl_utils.h"
#include "utils/math_utils.h"
#include <iostream>
#include <random>

using namespace std;
using namespace m1;

Tema2::Tema2() {}
Tema2::~Tema2() {}

void Tema2::Init() {
    b = new BlockWorld();

    camera = new implemented::MyCamera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    miniMap = new implemented::MyCamera();

    miniMap->position = glm::vec3(0, 150, 0);
    miniMap->forward = glm::vec3(0, -1, 0);
    miniMap->right = glm::vec3(1, 0, 0);
    miniMap->up = glm::vec3(0, 0, -1);
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea =
        ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    DrawUtils d;
    p = d.InitTrainCar(glm::vec3(0, 0, 0));
    CreateMesh("car1", p.vertices, p.indices, GL_TRIANGLES);

    p = d.InitTrain(glm::vec3(0, 0, 0));
    CreateMesh("train", p.vertices, p.indices, GL_TRIANGLES);

    p = d.InitTrainCar(glm::vec3(0, 0, 0));
    CreateMesh("car2", p.vertices, p.indices, GL_TRIANGLES);

    p = d.InitStation1(glm::vec3(0, 0, 0));
    CreateMesh("st1", p.vertices, p.indices, GL_TRIANGLES);

    p = d.InitStation2(glm::vec3(0, 0, 0));
    CreateMesh("st2", p.vertices, p.indices, GL_TRIANGLES);

    p = d.InitStation3(glm::vec3(0, 0, 0));
    CreateMesh("st3", p.vertices, p.indices, GL_TRIANGLES);

    p = d.InitStation4(glm::vec3(0, 0, 0));
    CreateMesh("st4", p.vertices, p.indices, GL_TRIANGLES);

    p = d.InitWood(glm::vec3(0, 0, 0));
    CreateMesh("wood", p.vertices, p.indices, GL_TRIANGLES);

    p = d.InitPlant(glm::vec3(0, 0, 0));
    CreateMesh("plant", p.vertices, p.indices, GL_TRIANGLES);

    p = d.InitCoal(glm::vec3(0, 0, 0));
    CreateMesh("coal", p.vertices, p.indices, GL_TRIANGLES);

    float fov = RADIANS(60);
    float aspect = window->props.aspectRatio;
    float zNear = 0.01f;
    float zFar = 200.0f;

    projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);

    b->GenerateTerrain();
    b->GenerateRiver();
    b->GenerateMesh();
    AddMeshToList(b->terrainMesh);

    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x,
                                          resolution.y);

    textRenderer->Load(
        PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"),
        150);

    Shader *shader = new Shader("My");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2",
                                "shaders", "VertexShader.glsl"),
                      GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2",
                                "shaders", "FragmentShader.glsl"),
                      GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    shader = new Shader("Deform");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2",
                                "shaders", "DeformationVS.glsl"),
                      GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2",
                                "shaders", "DeformationFS.glsl"),
                      GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    trainPos = glm::vec3(53, b->heights[53][60], 60);
    car1Pos = glm::vec3(52, b->heights[52][60], 60);
    car2Pos = glm::vec3(51, b->heights[51][60], 60);
    carStates["car1"].pos = car1Pos;
    carStates["car2"].pos = car2Pos;
    carStates["car1"].rotating = false;
    carStates["car2"].rotating = false;
    carStates["car1"].dirx = 1;
    carStates["car2"].dirx = 1;
    carStates["car1"].dirz = 0;
    carStates["car2"].dirz = 0;
    carStates["car1"].angle = 0.0f;
    carStates["car2"].angle = 0.0f;
    trainFic = trainPos;

    deltaPos = glm::vec3(0.5 - WORLD_WIDTH / 2.0, 0.3, 0.5 - WORLD_DEPTH / 2.0);
    dirx = 1;
    keyPressed = 0;

    delivery = generateCommand();
    special_coords.push_back({105, 50});
    special_coords.push_back({105, 109});
    special_coords.push_back({15, 130});
    special_coords.push_back({55, 69});

    time_start.resize(5);
    for (int i = 0; i < 5; i++) {
        time_start[i] = 0.0f;
    }
    time_elapsed.resize(5);
    for (int i = 0; i < 5; i++) {
        time_elapsed[i] = 0.0f;
    }

    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x,
                                          resolution.y);

    textRenderer->Load(
        PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"),
        150);
    camera->position = trainPos + deltaPos;
}

void Tema2::FrameStart() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds) {
    timer += deltaTimeSeconds;
    if (timer >= max_time) {
        timer = max_time;
        float left = -10.0f;
        float right = 10.0f;
        float bottom = -10.0f;
        float top = 10.0f;
        float near = 0.1f;
        float far = 100.0f;
        projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
        glm::vec2 coordinates = glm::vec2(220, 220);
        textRenderer->RenderText("Game Over", coordinates.x, coordinates.y, 1);
        return;
    }
    DrawScene1(deltaTimeSeconds);

    glViewport(40, 40, 300, 300);
    glClear(GL_DEPTH_BUFFER_BIT);

    DrawScene2(deltaTimeSeconds);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::DrawScene1(float deltaTimeSeconds) {
    glm::ivec2 resolution = window->GetResolution();
    float fov = glm::radians(60.0f);
    float aspect = (float)resolution.x / (float)resolution.y;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(fov, aspect, nearPlane, farPlane);

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(
        modelMatrix, glm::vec3(105 - WORLD_WIDTH / 2.0, b->heights[100][50],
                               55 - WORLD_DEPTH / 2.0));
    RenderMesh(meshes["st1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(
        modelMatrix, glm::vec3(105 - WORLD_WIDTH / 2.0, b->heights[100][100],
                               105 - WORLD_DEPTH / 2.0));
    modelMatrix =
        glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0, 1, 0));
    RenderMesh(meshes["st2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(
        modelMatrix, glm::vec3(15 - WORLD_WIDTH / 2.0, b->heights[10][130],
                               135 - WORLD_DEPTH / 2.0));
    RenderMesh(meshes["st3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(
        modelMatrix, glm::vec3(15 - WORLD_WIDTH / 2.0, b->heights[10][130],
                               135 - WORLD_DEPTH / 2.0));
    RenderMesh(meshes["w"], shaders["VertexColor"], modelMatrix);

    Shader *shader = shaders["Deform"];
    shader->Use();

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));

    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(view));

    int loc_projection_matrix =
        glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE,
                       glm::value_ptr(projection));

    int t = glGetUniformLocation(shader->program, "uTime");
    glUniform1f(t, timer);

    t = glGetUniformLocation(shader->program, "uMaxTime");
    glUniform1f(t, max_time);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(
        modelMatrix, glm::vec3(55 - WORLD_WIDTH / 2.0, b->heights[50][60],
                               65 - WORLD_DEPTH / 2.0));
    modelMatrix =
        glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0, 1, 0));
    RenderMesh(meshes["st4"], shader, modelMatrix);

    for (int i = 0; i < 5; i++) {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix =
            glm::translate(modelMatrix, glm::vec3(53 + i - WORLD_WIDTH / 2.0,
                                                  b->heights[50][60] + 5,
                                                  65 - WORLD_DEPTH / 2.0));
        if (delivery[i] == 1) {
            RenderMesh(meshes["wood"], shaders["VertexColor"], modelMatrix);
        } else if (delivery[i] == 2) {
            RenderMesh(meshes["coal"], shaders["VertexColor"], modelMatrix);
        } else if (delivery[i] == 3) {
            RenderMesh(meshes["plant"], shaders["VertexColor"], modelMatrix);
        }
    }

    for (int i = 0; i < 3; i++) {
        time_elapsed[i] += deltaTimeSeconds;
    }

    glm::ivec2 cell = {(int)round(trainPos.x), (int)round(trainPos.z)};
    for (int i = 0; i < 3; i++) {
        if (cell.x == special_coords[i].first &&
            cell.y == special_coords[i].second && delivery[idx] == i + 1 &&
            (time_start[i] == 0 ||
             abs(time_start[i] - time_elapsed[i]) >= 10.0f)) {

            delivery[idx] = 0;
            idx++;
            time_start[i] = deltaTimeSeconds;
            time_elapsed[i] = deltaTimeSeconds;
        }
    }

    if (idx == 5 && cell.x == special_coords[3].first &&
        cell.y == special_coords[3].second) {
        delivery = generateCommand();
        idx = 0;
        timer = 0.0f;
    }

    UpdateTrain(deltaTimeSeconds);
    UpdateCar("car1", deltaTimeSeconds);
    UpdateCar("car2", deltaTimeSeconds);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(
        modelMatrix, glm::vec3(-WORLD_WIDTH / 2.0f, 0, -WORLD_DEPTH / 2.0f));

    RenderMesh(b->terrainMesh, shaders["My"], modelMatrix);
}

void Tema2::DrawScene2(float deltaTimeSeconds) {
    glm::mat4 view = miniMap->GetViewMatrix();
    glm::mat4 prj = glm::ortho(-75.f, 75.f, -75.f, 75.f, 0.1f, 300.f);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(
        modelMatrix, glm::vec3(-WORLD_WIDTH / 2.0f, 0, -WORLD_DEPTH / 2.0f));

    Shader *shader = shaders["My"];
    shader->Use();

    int loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));

    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(view));

    int loc_projection_matrix =
        glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(prj));

    b->terrainMesh->Render();

    shader = shaders["VertexColor"];
    shader->Use();

    loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    loc_view_matrix = glGetUniformLocation(shader->program, "View");
    loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");

    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(prj));

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(
        modelMatrix, glm::vec3(105 - WORLD_WIDTH / 2.0, b->heights[100][50],
                               55 - WORLD_DEPTH / 2.0));
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));
    meshes["st1"]->Render();

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(
        modelMatrix, glm::vec3(105 - WORLD_WIDTH / 2.0, b->heights[100][100],
                               105 - WORLD_DEPTH / 2.0));
    modelMatrix =
        glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0, 1, 0));
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));
    meshes["st2"]->Render();

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(
        modelMatrix, glm::vec3(15 - WORLD_WIDTH / 2.0, b->heights[10][130],
                               135 - WORLD_DEPTH / 2.0));
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));
    meshes["st3"]->Render();

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(
        modelMatrix, glm::vec3(55 - WORLD_WIDTH / 2.0, b->heights[50][60],
                               65 - WORLD_DEPTH / 2.0));
    modelMatrix =
        glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0, 1, 0));
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));
    meshes["st4"]->Render();

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, trainPos + deltaPos);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7, 0.7, 0.7));
    if (rotate) {
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angle),
                                  glm::vec3(0.0, 1.0, 0.0));
    }
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));
    meshes["train"]->Render();
}

void Tema2::FrameEnd() {}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float cameraSpeed = 20.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            camera->TranslateForward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->TranslateForward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            camera->TranslateUpward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            camera->TranslateUpward(cameraSpeed * deltaTime);
        }
    }
}

void Tema2::OnKeyPress(int key, int mods) {
    float trainSpeed = 2.0f;
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        return;
    }

    if (key == GLFW_KEY_W) {
        autoMove = true;
        if (waiting) {
            waiting = false;
            keyPressed = GLFW_KEY_W;
            carStates["car1"].keyPressed = GLFW_KEY_W;
            carStates["car2"].keyPressed = GLFW_KEY_W;
        }
    }

    if (key == GLFW_KEY_A) {
        autoMove = true;
        if (waiting) {
            waiting = false;
            keyPressed = GLFW_KEY_A;
            carStates["car1"].keyPressed = GLFW_KEY_A;
            carStates["car2"].keyPressed = GLFW_KEY_A;
        }
    }

    if (key == GLFW_KEY_D) {
        autoMove = true;
        if (waiting) {
            waiting = false;
            keyPressed = GLFW_KEY_D;
            carStates["car1"].keyPressed = GLFW_KEY_D;
            carStates["car2"].keyPressed = GLFW_KEY_D;
        }
    }
}

void Tema2::OnKeyRelease(int key, int mods) {}
void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
            camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
        }
    }
}
void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Tema2::OnWindowResize(int width, int height) {}

void Tema2::RenderMesh(Mesh *mesh, Shader *shader,
                       const glm::mat4 &modelMatrix) {
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE,
                       glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE,
                       glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::CreateMesh(const char *name,
                       const std::vector<VertexFormat> &vertices,
                       const std::vector<unsigned int> &indices,
                       GLenum polygonMode) {
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(),
                 &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    glGenBuffers(1, &IBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(),
                 &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
                          (void *)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
                          (void *)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
                          (void *)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    glBindVertexArray(0);

    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION) {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go "
                "away when completing the tasks."
             << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core "
                "spec >=3.1 forbids null VAOs."
             << std::endl;
    }

    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO,
                                 static_cast<unsigned int>(indices.size()));
    meshes[name]->SetDrawMode(polygonMode);
}

void Tema2::UpdateTrain(float deltaTime) {
    glm::ivec2 cell = {(int)round(trainPos.x), (int)round(trainPos.z)};
    vector<pair<int, int>> temp = b->ComputeNumberOfNeighs(cell.x, cell.y);

    if (keyPressed == GLFW_KEY_A) {
        if (dirx > 0) {
            int sw = 0;
            int target = cell.y - 1;
            for (const auto &v : temp) {
                if (v.second == target) {
                    sw = 1;
                }
            }
            if (!sw) {
                possible = false;
                goto jump;
            }
        } else if (dirx < 0) {
            int sw = 0;
            int target = cell.y + 1;
            for (const auto &v : temp) {
                if (v.second == target) {
                    sw = 1;
                }
            }
            if (!sw) {
                possible = false;
                goto jump;
            }
        } else if (dirz > 0) {
            int sw = 0;
            int target = cell.x - 1;
            for (const auto &v : temp) {
                if (v.first == target) {
                    sw = 1;
                }
            }
            if (!sw) {
                possible = false;
                goto jump;
            }
        } else if (dirz < 0) {
            int sw = 0;
            int target = cell.x + 1;
            for (const auto &v : temp) {
                if (v.first == target) {
                    sw = 1;
                }
            }
            if (!sw) {
                possible = false;
                goto jump;
            }
        }
        rotate = true;
        possible = true;
        if (dirx > 0) {
            angle = 90.f;
            dirx = 0;
            dirz = -1;
        } else if (dirz > 0) {
            angle = 0.f;
            dirz = 0;
            dirx = 1;
        } else if (dirx < 0) {
            angle = -90.f;
            dirx = 0;
            dirz = 1;
        } else if (dirz < 0) {
            angle = 180.f;
            dirx = -1;
            dirz = 0;
        }
        savedCoords[{cell.x, cell.y}] = {dirx, dirz, angle};
    } else if (keyPressed == GLFW_KEY_W) {
        if (dirx > 0) {
            int sw = 0;
            int target = cell.x + 1;
            for (const auto &v : temp) {
                if (v.first == target) {
                    sw = 1;
                }
            }
            if (!sw) {
                possible = false;
                goto jump;
            }
        } else if (dirx < 0) {
            int sw = 0;
            int target = cell.x - 1;
            for (const auto &v : temp) {
                if (v.first == target) {
                    sw = 1;
                }
            }
            if (!sw) {
                possible = false;
                goto jump;
            }
        } else if (dirz > 0) {
            int sw = 0;
            int target = cell.y + 1;
            for (const auto &v : temp) {
                if (v.second == target) {
                    sw = 1;
                }
            }
            if (!sw) {
                possible = false;
                goto jump;
            }
        } else if (dirz < 0) {
            int sw = 0;
            int target = cell.y - 1;
            for (const auto &v : temp) {
                if (v.second == target) {
                    sw = 1;
                }
            }
            if (!sw) {
                possible = false;
                goto jump;
            }
        }
        possible = true;
        if (dirx > 0) {
            dirx = 1;
            dirz = 0;
        } else if (dirz > 0) {
            dirz = 1;
            dirx = 0;
        } else if (dirx < 0) {
            dirx = -1;
            dirz = 0;
        } else if (dirz < 0) {
            dirx = 0;
            dirz = -1;
        }
        savedCoords[{cell.x, cell.y}] = {dirx, dirz, angle};
    } else if (keyPressed == GLFW_KEY_D) {
        if (dirx > 0) {
            int sw = 0;
            int target = cell.y + 1;
            for (const auto &v : temp) {
                if (v.second == target) {
                    sw = 1;
                }
            }
            if (!sw) {
                possible = false;
                goto jump;
            }
        } else if (dirx < 0) {
            int sw = 0;
            int target = cell.y - 1;
            for (const auto &v : temp) {
                if (v.second == target) {
                    sw = 1;
                }
            }
            if (!sw) {
                possible = false;
                goto jump;
            }
        } else if (dirz > 0) {
            int sw = 0;
            int target = cell.x + 1;
            for (const auto &v : temp) {
                if (v.first == target) {
                    sw = 1;
                }
            }
            if (!sw) {
                possible = false;
                goto jump;
            }
        } else if (dirz < 0) {
            int sw = 0;
            int target = cell.x - 1;
            for (const auto &v : temp) {
                if (v.first == target) {
                    sw = 1;
                }
            }
            if (!sw) {
                possible = false;
                goto jump;
            }
        }
        rotate = true;
        possible = true;
        if (dirx > 0) {
            angle = -90.f;
            dirx = 0;
            dirz = 1;
        } else if (dirz > 0) {
            angle = 180.f;
            dirz = 0;
            dirx = -1;
        } else if (dirx < 0) {
            angle = 90.f;
            dirx = 0;
            dirz = -1;
        } else if (dirz < 0) {
            angle = 0.f;
            dirx = 1;
            dirz = 0;
        }
        savedCoords[{cell.x, cell.y}] = {dirx, dirz, angle};
    }

jump:
    if (autoMove && !waiting) {
        if (dirx != 0) {
            glm::vec3 next =
                trainPos + glm::vec3(dirx * trainSpeed * deltaTime, 0, 0);
            glm::ivec2 cell = {(int)round(next.x), (int)round(next.z)};
            if (b->train[cell.x][cell.y]) {

                vector<pair<int, int>> temp =
                    b->ComputeNumberOfNeighs(cell.x, cell.y);

                if (temp.size() > 2) {
                    if (!keyPressed) {
                        waiting = true;

                    } else {
                        if (possible) {
                            trainPos.x = (float)cell.x + dirx;
                            trainPos.z = (float)cell.y;
                            trainFic = trainPos;
                            keyPressed = 0;
                        }
                    }
                }

                if (autoMove) {
                    trainPos.x += dirx * trainSpeed * deltaTime;
                    trainFic = trainPos;
                }
            } else {
                trainPos.x = round(trainPos.x);
                trainPos.z = round(trainPos.z);
                trainFic = trainPos;

                dirx = 0;

                glm::ivec2 tmp = {(int)round(trainFic.x),
                                  (int)round(trainFic.z)};
                vector<pair<int, int>> temp =
                    b->ComputeNumberOfNeighs(tmp.x, tmp.y);
                int diff = 0;
                for (const auto &p : temp) {
                    if (tmp.y != p.second) {
                        diff = p.second - tmp.y;
                    }
                }
                dirz = diff;
                angle = diff * (-90.f);
                rotate = true;
            }
        } else if (dirz != 0) {
            glm::vec3 next =
                trainPos + glm::vec3(0, 0, dirz * trainSpeed * deltaTime);
            glm::ivec2 cell = {(int)round(next.x), (int)round(next.z)};

            if (b->train[cell.x][cell.y]) {
                vector<pair<int, int>> temp =
                    b->ComputeNumberOfNeighs(cell.x, cell.y);

                if (temp.size() > 2) {
                    if (!keyPressed) {
                        waiting = true;
                    } else {
                        if (possible) {
                            trainPos.x = (float)cell.x;
                            trainPos.z = (float)cell.y + dirz;
                            trainFic = trainPos;
                            keyPressed = 0;
                        }
                    }
                }

                if (autoMove) {
                    trainPos.z += dirz * trainSpeed * deltaTime;
                    trainFic = trainPos;
                }
            } else {
                trainPos.x = round(trainPos.x);
                trainPos.z = round(trainPos.z);
                trainFic = trainPos;

                dirz = 0;

                glm::ivec2 tmp = {(int)round(trainFic.x),
                                  (int)round(trainFic.z)};
                vector<pair<int, int>> temp =
                    b->ComputeNumberOfNeighs(tmp.x, tmp.y);
                int diff = 0;
                for (const auto &p : temp) {
                    if (tmp.x != p.first) {
                        diff = p.first - tmp.x;
                    }
                }
                dirx = diff;
                if (diff == 1) {
                    angle = 0.f;
                } else {
                    angle = 180.f;
                }
                rotate = true;
            }
        }
    }

    cell = {(int)round(trainPos.x), (int)round(trainPos.z)};
    trainPos.y = b->heights[cell.x][cell.y];

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, trainPos + deltaPos);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
    if (rotate) {
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angle),
                                  glm::vec3(0.0, 1.0, 0.0));
    }
    RenderMesh(meshes["train"], shaders["VertexColor"], modelMatrix);

    // glm::vec3 trainPosition = trainPos + deltaPos;
    //
    // float distanceBehind = 5.0f;
    // float heightAbove = 10.0f;
    //
    // glm::vec3 trainForward;
    //
    // trainForward =
    //     glm::vec3(cos(glm::radians(angle)), 0, sin(glm::radians(angle)));
    //
    // glm::vec3 targetPosition = trainPosition - trainForward * distanceBehind
    // +
    //                            glm::vec3(0, heightAbove, 0);
    //
    // float positionSmoothness = 5.0f;
    // camera->position = glm::mix(camera->position, targetPosition,
    //                             deltaTime * positionSmoothness);
    //
    // glm::vec3 lookAtPoint = trainPosition + trainForward * 5.0f;
    // glm::vec3 targetForward = glm::normalize(lookAtPoint - camera->position);
    //
    // float rotationSmoothness = 3.0f;
    // camera->forward = glm::normalize(glm::mix(camera->forward, targetForward,
    //                                           deltaTime *
    //                                           rotationSmoothness));
    //
    // camera->right =
    //     glm::normalize(glm::cross(camera->forward, glm::vec3(0, 1, 0)));
    // camera->up = glm::normalize(glm::cross(camera->right, camera->forward));
}

void Tema2::UpdateCar(string name, float deltaTime) {
    if (autoMove && !waiting) {
        if (carStates[name].dirx != 0) {
            glm::vec3 next =
                carStates[name].pos +
                glm::vec3(carStates[name].dirx * trainSpeed * deltaTime, 0, 0);
            glm::ivec2 cell = {(int)round(next.x), (int)round(next.z)};
            if (b->train[cell.x][cell.y]) {

                vector<pair<int, int>> temp =
                    b->ComputeNumberOfNeighs(cell.x, cell.y);

                if (temp.size() > 2) {
                    if (possible) {
                        int x = carStates[name].dirx;
                        int z = carStates[name].dirz;
                        carStates[name].dirx =
                            savedCoords[{cell.x, cell.y}].dirx;
                        carStates[name].dirz =
                            savedCoords[{cell.x, cell.y}].dirz;

                        carStates[name].angle =
                            savedCoords[{cell.x, cell.y}].angle;
                        carStates[name].rotating = true;

                        carStates[name].pos.x =
                            (float)cell.x + carStates[name].dirx;
                        carStates[name].pos.z = (float)cell.y;
                        carStates[name].keyPressed = 0;
                    }
                }

                if (autoMove) {
                    carStates[name].pos.x +=
                        carStates[name].dirx * trainSpeed * deltaTime;
                }
            } else {
                carStates[name].pos.x = round(carStates[name].pos.x);
                carStates[name].pos.z = round(carStates[name].pos.z);

                carStates[name].dirx = 0;

                glm::ivec2 tmp = {(int)round(carStates[name].pos.x),
                                  (int)round(carStates[name].pos.z)};
                vector<pair<int, int>> temp =
                    b->ComputeNumberOfNeighs(tmp.x, tmp.y);
                int diff = 0;
                for (const auto &p : temp) {
                    if (tmp.y != p.second) {
                        diff = p.second - tmp.y;
                    }
                }
                carStates[name].dirz = diff;
                carStates[name].angle = diff * (90.f);
                carStates[name].rotating = true;
            }
        } else if (carStates[name].dirz != 0) {
            glm::vec3 next =
                carStates[name].pos +
                glm::vec3(0, 0, carStates[name].dirz * trainSpeed * deltaTime);
            glm::ivec2 cell = {(int)round(next.x), (int)round(next.z)};

            if (b->train[cell.x][cell.y]) {

                vector<pair<int, int>> temp =
                    b->ComputeNumberOfNeighs(cell.x, cell.y);

                if (temp.size() > 2) {
                    if (possible) {
                        int x = carStates[name].dirx;
                        int z = carStates[name].dirz;
                        carStates[name].dirx =
                            savedCoords[{cell.x, cell.y}].dirx;
                        carStates[name].dirz =
                            savedCoords[{cell.x, cell.y}].dirz;

                        carStates[name].angle =
                            savedCoords[{cell.x, cell.y}].angle;
                        carStates[name].rotating = true;
                        carStates[name].pos.x = (float)cell.x;
                        carStates[name].pos.z =
                            (float)cell.y + carStates[name].dirz;
                        carStates[name].keyPressed = 0;
                    }
                }

                if (autoMove) {
                    carStates[name].pos.z +=
                        carStates[name].dirz * trainSpeed * deltaTime;
                }
            } else {
                carStates[name].pos.x = round(carStates[name].pos.x);
                carStates[name].pos.z = round(carStates[name].pos.z);

                carStates[name].dirz = 0;

                glm::ivec2 tmp = {(int)round(carStates[name].pos.x),
                                  (int)round(carStates[name].pos.z)};
                vector<pair<int, int>> temp =
                    b->ComputeNumberOfNeighs(tmp.x, tmp.y);
                int diff = 0;
                for (const auto &p : temp) {
                    if (tmp.x != p.first) {
                        diff = p.first - tmp.x;
                    }
                }
                carStates[name].dirx = diff;
                if (diff == 1) {
                    carStates[name].angle = 0.f;
                } else {
                    carStates[name].angle = 180.f;
                }
                carStates[name].rotating = true;
            }
        }
    }

    glm::ivec2 cell = {(int)round(carStates[name].pos.x),
                       (int)round(carStates[name].pos.z)};
    carStates[name].pos.y = b->heights[cell.x][cell.y];
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, carStates[name].pos + deltaPos);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
    if (carStates[name].rotating) {
        modelMatrix =
            glm::rotate(modelMatrix, glm::radians(carStates[name].angle),
                        glm::vec3(0.0, 1.0, 0.0));
    }
    RenderMesh(meshes[name.c_str()], shaders["VertexColor"], modelMatrix);
}

vector<int> Tema2::generateCommand() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 3);
    vector<int> res;
    for (int i = 0; i < 5; i++) {
        res.push_back(dist(gen));
    }

    return res;
}
