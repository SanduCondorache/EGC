#include "lab_m1/lab2/lab2.h"

#include <iostream>
#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

class TrainStation {
  public:
    TrainStation();
    ~TrainStation();

    // Generate station geometry at specified location
    void Generate(float startX, float startZ, float baseHeight, int size = 9);

    // Get generated data
    const std::vector<VertexFormat> &GetVertices() const { return vertices; }
    const std::vector<unsigned int> &GetIndices() const { return indices; }

    // Clear generated data
    void Clear();

  private:
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Helper to add a quad
    void AddQuad(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2,
                 const glm::vec3 &p3, const glm::vec3 &normal,
                 const glm::vec3 &color);

    // Component generators
    void GeneratePlatform(float x1, float x2, float z1, float z2, float h,
                          float platformHeight);
    void GeneratePillar(float px, float pz, float h, float platformHeight,
                        float roofHeight, float pillarWidth);
    void GenerateRoof(float x1, float x2, float z1, float z2, float h,
                      float roofHeight, float roofThickness, float overhang);
    void GenerateWall(float x1, float z1, float z2, float h,
                      float platformHeight, float roofHeight);
};

TrainStation::TrainStation() {}

TrainStation::~TrainStation() {}

void TrainStation::Clear() {
    vertices.clear();
    indices.clear();
}

void TrainStation::AddQuad(const glm::vec3 &p0, const glm::vec3 &p1,
                           const glm::vec3 &p2, const glm::vec3 &p3,
                           const glm::vec3 &normal, const glm::vec3 &color) {
    unsigned int baseIdx = vertices.size();

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p1, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));
    vertices.push_back(VertexFormat(p3, normal, color));

    // First triangle
    indices.push_back(baseIdx + 0);
    indices.push_back(baseIdx + 1);
    indices.push_back(baseIdx + 2);

    // Second triangle
    indices.push_back(baseIdx + 0);
    indices.push_back(baseIdx + 2);
    indices.push_back(baseIdx + 3);
}

void TrainStation::GeneratePlatform(float x1, float x2, float z1, float z2,
                                    float h, float platformHeight) {
    glm::vec3 platformColor(0.6f, 0.6f, 0.65f);

    // Top surface
    AddQuad(glm::vec3(x1, h + platformHeight, z1),
            glm::vec3(x2, h + platformHeight, z1),
            glm::vec3(x2, h + platformHeight, z2),
            glm::vec3(x1, h + platformHeight, z2), glm::vec3(0, 1, 0),
            platformColor);

    // Front edge
    AddQuad(glm::vec3(x1, h, z1), glm::vec3(x2, h, z1),
            glm::vec3(x2, h + platformHeight, z1),
            glm::vec3(x1, h + platformHeight, z1), glm::vec3(0, 0, -1),
            platformColor);

    // Back edge
    AddQuad(glm::vec3(x2, h, z2), glm::vec3(x1, h, z2),
            glm::vec3(x1, h + platformHeight, z2),
            glm::vec3(x2, h + platformHeight, z2), glm::vec3(0, 0, 1),
            platformColor);

    // Left edge
    AddQuad(glm::vec3(x1, h, z2), glm::vec3(x1, h, z1),
            glm::vec3(x1, h + platformHeight, z1),
            glm::vec3(x1, h + platformHeight, z2), glm::vec3(-1, 0, 0),
            platformColor);

    // Right edge
    AddQuad(glm::vec3(x2, h, z1), glm::vec3(x2, h, z2),
            glm::vec3(x2, h + platformHeight, z2),
            glm::vec3(x2, h + platformHeight, z1), glm::vec3(1, 0, 0),
            platformColor);
}

void TrainStation::GeneratePillar(float px, float pz, float h,
                                  float platformHeight, float roofHeight,
                                  float pillarWidth) {
    glm::vec3 pillarColor(0.9f, 0.9f, 0.9f);
    float pw = pillarWidth;

    // Front face (+Z)
    AddQuad(glm::vec3(px - pw, h + platformHeight, pz + pw),
            glm::vec3(px + pw, h + platformHeight, pz + pw),
            glm::vec3(px + pw, h + roofHeight, pz + pw),
            glm::vec3(px - pw, h + roofHeight, pz + pw), glm::vec3(0, 0, 1),
            pillarColor);

    // Back face (-Z)
    AddQuad(glm::vec3(px + pw, h + platformHeight, pz - pw),
            glm::vec3(px - pw, h + platformHeight, pz - pw),
            glm::vec3(px - pw, h + roofHeight, pz - pw),
            glm::vec3(px + pw, h + roofHeight, pz - pw), glm::vec3(0, 0, -1),
            pillarColor);

    // Left face (-X)
    AddQuad(glm::vec3(px - pw, h + platformHeight, pz - pw),
            glm::vec3(px - pw, h + platformHeight, pz + pw),
            glm::vec3(px - pw, h + roofHeight, pz + pw),
            glm::vec3(px - pw, h + roofHeight, pz - pw), glm::vec3(-1, 0, 0),
            pillarColor);

    // Right face (+X)
    AddQuad(glm::vec3(px + pw, h + platformHeight, pz + pw),
            glm::vec3(px + pw, h + platformHeight, pz - pw),
            glm::vec3(px + pw, h + roofHeight, pz - pw),
            glm::vec3(px + pw, h + roofHeight, pz + pw), glm::vec3(1, 0, 0),
            pillarColor);
}

void TrainStation::GenerateRoof(float x1, float x2, float z1, float z2, float h,
                                float roofHeight, float roofThickness,
                                float overhang) {
    glm::vec3 roofColor(0.7f, 0.2f, 0.2f);
    glm::vec3 roofBottomColor = roofColor * 0.7f;

    float rx1 = x1 - overhang;
    float rx2 = x2 + overhang;
    float rz1 = z1 - overhang;
    float rz2 = z2 + overhang;

    // Top surface
    AddQuad(glm::vec3(rx1, h + roofHeight + roofThickness, rz1),
            glm::vec3(rx2, h + roofHeight + roofThickness, rz1),
            glm::vec3(rx2, h + roofHeight + roofThickness, rz2),
            glm::vec3(rx1, h + roofHeight + roofThickness, rz2),
            glm::vec3(0, 1, 0), roofColor);

    // Bottom surface
    AddQuad(glm::vec3(rx1, h + roofHeight, rz1),
            glm::vec3(rx1, h + roofHeight, rz2),
            glm::vec3(rx2, h + roofHeight, rz2),
            glm::vec3(rx2, h + roofHeight, rz1), glm::vec3(0, -1, 0),
            roofBottomColor);

    // Front edge
    AddQuad(glm::vec3(rx1, h + roofHeight, rz1),
            glm::vec3(rx2, h + roofHeight, rz1),
            glm::vec3(rx2, h + roofHeight + roofThickness, rz1),
            glm::vec3(rx1, h + roofHeight + roofThickness, rz1),
            glm::vec3(0, 0, -1), roofColor);

    // Back edge
    AddQuad(glm::vec3(rx2, h + roofHeight, rz2),
            glm::vec3(rx1, h + roofHeight, rz2),
            glm::vec3(rx1, h + roofHeight + roofThickness, rz2),
            glm::vec3(rx2, h + roofHeight + roofThickness, rz2),
            glm::vec3(0, 0, 1), roofColor);

    // Left edge
    AddQuad(glm::vec3(rx1, h + roofHeight, rz2),
            glm::vec3(rx1, h + roofHeight, rz1),
            glm::vec3(rx1, h + roofHeight + roofThickness, rz1),
            glm::vec3(rx1, h + roofHeight + roofThickness, rz2),
            glm::vec3(-1, 0, 0), roofColor);

    // Right edge
    AddQuad(glm::vec3(rx2, h + roofHeight, rz1),
            glm::vec3(rx2, h + roofHeight, rz2),
            glm::vec3(rx2, h + roofHeight + roofThickness, rz2),
            glm::vec3(rx2, h + roofHeight + roofThickness, rz1),
            glm::vec3(1, 0, 0), roofColor);
}

void TrainStation::GenerateWall(float x1, float z1, float z2, float h,
                                float platformHeight, float roofHeight) {
    glm::vec3 wallColor(0.8f, 0.75f, 0.65f);

    AddQuad(glm::vec3(x1, h + platformHeight, z1),
            glm::vec3(x1, h + platformHeight, z2),
            glm::vec3(x1, h + roofHeight, z2),
            glm::vec3(x1, h + roofHeight, z1), glm::vec3(-1, 0, 0), wallColor);
}

void TrainStation::Generate(float startX, float startZ, float baseHeight,
                            int size) {
    Clear();

    const float platformHeight = 0.5f;
    const float roofHeight = 4.0f;
    const float roofThickness = 0.3f;
    const float roofOverhang = 0.5f;
    const float pillarWidth = 0.4f;

    float x1 = startX;
    float x2 = startX + size;
    float z1 = startZ;
    float z2 = startZ + size;
    float h = baseHeight;

    // Generate platform
    GeneratePlatform(x1, x2, z1, z2, h, platformHeight);

    // Generate 4 corner pillars
    float pillarInset = 2.0f;
    GeneratePillar(x1 + pillarInset, z1 + pillarInset, h, platformHeight,
                   roofHeight, pillarWidth);
    GeneratePillar(x2 - pillarInset, z1 + pillarInset, h, platformHeight,
                   roofHeight, pillarWidth);
    GeneratePillar(x1 + pillarInset, z2 - pillarInset, h, platformHeight,
                   roofHeight, pillarWidth);
    GeneratePillar(x2 - pillarInset, z2 - pillarInset, h, platformHeight,
                   roofHeight, pillarWidth);

    // Generate roof
    GenerateRoof(x1, x2, z1, z2, h, roofHeight, roofThickness, roofOverhang);

    // Generate back wall
    GenerateWall(x1, z1, z2, h, platformHeight, roofHeight);
}
Lab2::Lab2() {}

Lab2::~Lab2() {}

void Lab2::Init() {
    cullFace = GL_BACK;
    polygonMode = GL_FILL;

    // Load a mesh from file into GPU memory
    {
        Mesh *mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS,
                                 "primitives"),
                       "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a mesh box using custom data
    {
        vector<VertexFormat> vertices{
            VertexFormat(glm::vec3(0, 0, 1)), VertexFormat(glm::vec3(1, 0, 1)),
            VertexFormat(glm::vec3(0, 1, 1)), VertexFormat(glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(0, 0, 0)), VertexFormat(glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(0, 1, 0)), VertexFormat(glm::vec3(1, 1, 0))};

        vector<unsigned int> indices = {0, 1, 2, // indices for first triangle
                                        1, 3, 2, // indices for second triangle
                                        2, 3, 7, 2, 7, 6, 1, 7, 3, 1,
                                        5, 7, 6, 7, 4, 7, 5, 4, 0, 4,
                                        1, 1, 4, 5, 2, 6, 4, 0, 2, 4};

        meshes["cube_A"] = new Mesh("generated cube 1");
        meshes["cube_A"]->InitFromData(vertices, indices);

        CreateMesh("cube_B", vertices, indices);
    }
    {
        TrainStation station;
        station.Generate(100, 50, 7, 9);

        // Get the geometry
        const auto &stationVertices = station.GetVertices();
        const auto &stationIndices = station.GetIndices();

        vector<VertexFormat> vertices;

        vector<unsigned int> indices;
        // Merge with your existing vertices/indices
        for (const auto &v : stationVertices) {
            vertices.push_back(v);
        }
        for (unsigned int idx : stationIndices) {
            indices.push_back(idx);
        }

        // Or create separate mesh for the station
        Mesh *stationMesh = new Mesh("station");
        stationMesh->SetDrawMode(GL_TRIANGLES);
        stationMesh->InitFromData(stationVertices, stationIndices);
    }

    {
        vector<VertexFormat> vertices{
            VertexFormat(glm::vec3(0, 0, 1)), VertexFormat(glm::vec3(1, 0, 1)),
            VertexFormat(glm::vec3(0, 1, 0)), VertexFormat(glm::vec3(1, 1, 0))};

        vector<unsigned int> indices = {0, 1, 2, // indices for first triangle
                                        1, 3, 2, // indices for second triangle
                                        2, 3, 7, 2, 7, 6, 1, 7, 3, 1,
                                        5, 7, 6, 7, 4, 7, 5, 4, 0, 4,
                                        1, 1, 4, 5, 2, 6, 4, 0, 2, 4};

        CreateMesh("line", vertices, indices);
    }

    {
        vector<VertexFormat> vertices{
            VertexFormat(glm::vec3(0, 0, 1), glm::vec3(1, 0, 0)), // Red
            VertexFormat(glm::vec3(1, 0, 1), glm::vec3(0, 1, 0)), // Green
            VertexFormat(glm::vec3(0, 1, 1), glm::vec3(0, 0, 1)), // Blue
            VertexFormat(glm::vec3(1, 1, 1), glm::vec3(1, 1, 0)), // Yellow
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 1)), // Magenta
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 1, 1)), // Cyan
            VertexFormat(glm::vec3(0, 1, 0),
                         glm::vec3(0.9, 0.4, 0.2)),                    // Orange
            VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0.5, 0.2, 0.8)) // Purple
        };

        vector<unsigned int> indices = {
            // 0, 1, 2,    // indices for first triangle
            1, 3, 2, // indices for second triangle
                     // TODO(student): Complete indices data for the cube mesh
            2, 3, 7,
            // 2, 7, 6,
            1, 7, 3, 1, 2, 7
            // 1, 5, 7,
            // 6, 7, 4,
            // 7, 5, 4,
            // 0, 4, 1,
            // 1, 4, 5,
            // 2, 6, 4,
            // 0, 2, 4
        };

        // Actually create the mesh from the data
        CreateMesh("tetra", vertices, indices);
    }

    {
        vector<VertexFormat> vertices{
            VertexFormat(glm::vec3(0, 0, 0)), VertexFormat(glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(0, 0, 1)), VertexFormat(glm::vec3(0, 1, 1))};

        vector<unsigned int> indices = {0, 1, 2, 1, 2, 3};

        CreateMesh("schizo", vertices, indices);
    }

    // TODO(student): Create a tetrahedron mesh. You can create it with
    // only 4 vertices, or you can choose the harder route and create it
    // with 12 vertices. Think about it, why would you want that, and how
    // would you do it? After all, a tetrahedron has only 4 vertices
    // by definition!

    // TODO(student): Create a square using two triangles with
    // opposing vertex orientations.
    {
        vector<VertexFormat> vertices;
        vertices.push_back(VertexFormat(glm::vec3(0, 0, 0)));
        for (unsigned int i = 1; i <= poligons; i++) {
            vertices.push_back(
                VertexFormat(glm::vec3(cos(2 * M_PI / poligons * i),
                                       sin(2 * M_PI / poligons * i), 0)));
        }

        std::vector<unsigned int> indices;
        for (unsigned int i = 1; i <= poligons; i++) {
            unsigned int next = (i == poligons) ? 1 : (i + 1);
            indices.push_back(0);
            indices.push_back(next);
            indices.push_back(i);
        }

        CreateMesh("math_expert", vertices, indices);
    }
}

void Lab2::CreateMesh(const char *name,
                      const std::vector<VertexFormat> &vertices,
                      const std::vector<unsigned int> &indices) {
    unsigned int VAO = 0;
    // TODO(student): Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    // TODO(student): Create the VBO and bind it
    GLuint VBO_ID;
    glGenBuffers(1, &VBO_ID);

    // TODO(student): Send vertices data into the VBO buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(),
                 &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    // TODO(student): Create the IBO and bind it
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(),
                 &indices[0], GL_STATIC_DRAW);

    // TODO(student): Send indices data into the IBO buffer

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data. It will be learned later, when GLSL shaders will be
    // introduced. For the moment, just think that each property value from
    // our vertex format needs to be sent to a certain channel, in order to
    // know how to receive it in the GLSL vertex shader.

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
    // ========================================================================

    // TODO(student): Unbind the VAO
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

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO,
                                 static_cast<unsigned int>(indices.size()));
}

void Lab2::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab2::Update(float deltaTimeSeconds) {
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // TODO(student): Enable face culling
    glEnable(GL_CULL_FACE);

    // TODO(student): Set face custom culling. Use the `cullFace` variable.

    // Render an object using face normals for color
    RenderMesh(meshes["box"], shaders["VertexNormal"],
               glm::vec3(0, 0.5f, -1.5f), glm::vec3(0.75f));

    RenderMesh(meshes["station"], shaders["VertexNormal"],
               glm::vec3(0, 0.5f, -1.5f), glm::vec3(0.75f));

    // Render an object using colors from vertex
    RenderMesh(meshes["cube_A"], shaders["VertexColor"], glm::vec3(0.5f, 0, 0),
               glm::vec3(0.25f));

    // TODO(student): Draw the mesh that was created with `CreateMesh()`
    RenderMesh(meshes["math_expert"], shaders["VertexColor"],
               glm::vec3(10, 10, 10), glm::vec3(1));

    // TODO(student): Draw the tetrahedron

    // TODO(student): Draw the square

    // TODO(student): Disable face culling
}

void Lab2::FrameEnd() { DrawCoordinateSystem(); }

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Lab2::OnInputUpdate(float deltaTime, int mods) {}

void Lab2::OnKeyPress(int key, int mods) {
    // TODO(student): Switch between GL_FRONT and GL_BACK culling.
    // Save the state in `cullFace` variable and apply it in the
    // `Update()` method, NOT here!

    if (key == GLFW_KEY_SPACE) {
        switch (polygonMode) {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }

    if (key == GLFW_KEY_F2) {
        if (en_cull == 0) {
            glCullFace(GL_BACK);
            en_cull = 1;
        } else {
            glCullFace(GL_FRONT);
            en_cull = 0;
        }
    }
}

void Lab2::OnKeyRelease(int key, int mods) {
    // Add key release event
}

void Lab2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    // Add mouse move event
}

void Lab2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button press event
}

void Lab2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button release event
}

void Lab2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Lab2::OnWindowResize(int width, int height) {}
