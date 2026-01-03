#include "core/gpu/vertex_format.h"
#include "lab_m1/Tema2/DrawUtils.h"
#include "lab_m1/Tema2/MeshData.h"
#include "utils/math_utils.h"
#include <iostream>

DrawUtils::DrawUtils() {}

DrawUtils::~DrawUtils() {}

MeshData DrawUtils::InitSquare(glm::vec3 center, glm::vec3 color,
                               glm::vec3 scale) {

    std::vector<VertexFormat> vertices{
        VertexFormat(glm::vec3(-0.5, -0.5, -0.5) * scale + center, color),
        VertexFormat(glm::vec3(0.5, -0.5, -0.5) * scale + center, color),
        VertexFormat(glm::vec3(0.5, 0.5, -0.5) * scale + center, color),
        VertexFormat(glm::vec3(-0.5, 0.5, -0.5) * scale + center, color),
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,

        3, 0, 2,
    };

    return MeshData(vertices, indices);
}

MeshData DrawUtils::InitParallelepiped(glm::vec3 center, glm::vec3 color,
                                       glm::vec3 scale) {
    vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(-0.5, -0.5, -0.5) * scale + center, color),
        VertexFormat(glm::vec3(0.5, -0.5, -0.5) * scale + center, color),
        VertexFormat(glm::vec3(0.5, 0.5, -0.5) * scale + center, color),
        VertexFormat(glm::vec3(-0.5, 0.5, -0.5) * scale + center, color),
        VertexFormat(glm::vec3(-0.5, -0.5, 0.5) * scale + center, color),
        VertexFormat(glm::vec3(0.5, -0.5, 0.5) * scale + center, color),
        VertexFormat(glm::vec3(0.5, 0.5, 0.5) * scale + center, color),
        VertexFormat(glm::vec3(-0.5, 0.5, 0.5) * scale + center, color),
    };

    vector<unsigned int> indices = {
        0, 1, 2,

        2, 3, 0,

        4, 7, 6,

        6, 5, 4,

        1, 5, 6,

        6, 2, 1,

        4, 0, 3,

        3, 7, 4,

        3, 2, 6,

        6, 7, 3,

        4, 5, 1,

        1, 0, 4,
    };

    MeshData data(vertices, indices);
    data.sizeX = scale.x;
    data.sizeY = scale.y;
    data.sizeZ = scale.z;

    return data;
}

MeshData DrawUtils::InitHemisphere(glm::vec3 center, glm::vec3 color,
                                   float radius, int stacks, int slices) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= stacks; i++) {
        float v = float(i) / stacks;
        float phi = v * (M_PI / 2.0f);

        for (int j = 0; j <= slices; j++) {
            float u = float(j) / slices;
            float theta = u * glm::two_pi<float>();

            float x = radius * cos(theta) * sin(phi);
            float y = radius / 3.0 * cos(phi);
            float z = radius * sin(theta) * sin(phi);

            vertices.push_back(
                VertexFormat(center + glm::vec3(x, y, z), color));
        }
    }

    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    return MeshData(vertices, indices);
}

glm::vec3 DrawUtils::position(const glm::vec3 &p, const glm::mat4 &rot,
                              const glm::vec3 &center, const glm::vec3 &scale) {
    return glm::vec3(rot * glm::vec4(p * scale, 1.0f)) + center;
};

MeshData DrawUtils::InitCylinder(glm::vec3 center, glm::vec3 color,
                                 glm::vec3 scale, glm::vec3 rotationEuler) {
    int segments = 32;
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    float radius = 0.5f;

    glm::mat4 rot =
        glm::rotate(glm::mat4(1.0f), rotationEuler.x, glm::vec3(1, 0, 0)) *
        glm::rotate(glm::mat4(1.0f), rotationEuler.y, glm::vec3(0, 1, 0)) *
        glm::rotate(glm::mat4(1.0f), rotationEuler.z, glm::vec3(0, 0, 1));

    vertices.push_back(VertexFormat(
        position(glm::vec3(0, -0.5, 0), rot, center, scale), color));
    vertices.push_back(VertexFormat(
        position(glm::vec3(0, 0.5, 0), rot, center, scale), color));

    int bottom = vertices.size();

    for (int i = 0; i < segments; i++) {
        float angle = float(i) / segments * 2.f * M_PI;
        float x = cos(angle) * radius;
        float z = sin(angle) * radius;
        vertices.push_back(VertexFormat(
            position(glm::vec3(x, -0.5, z), rot, center, scale), color));
    }

    int top = vertices.size();

    for (int i = 0; i < segments; i++) {
        float angle = float(i) / segments * 2.f * M_PI;
        float x = cos(angle) * radius;
        float z = sin(angle) * radius;
        vertices.push_back(VertexFormat(
            position(glm::vec3(x, 0.5, z), rot, center, scale), color));
    }

    for (int i = 0; i < segments; i++) {
        int idx = (i + 1) % segments;
        indices.push_back(0);
        indices.push_back(bottom + idx);
        indices.push_back(bottom + i);
    }

    for (int i = 0; i < segments; i++) {
        int idx = (i + 1) % segments;
        indices.push_back(1);
        indices.push_back(top + i);
        indices.push_back(top + idx);
    }

    for (int i = 0; i < segments; i++) {
        int idx = (i + 1) % segments;

        int tmp1 = bottom + i;
        int tmp2 = bottom + idx;
        int tmp3 = top + i;
        int tmp4 = top + idx;

        indices.push_back(tmp1);
        indices.push_back(tmp2);
        indices.push_back(tmp3);
        indices.push_back(tmp3);
        indices.push_back(tmp2);
        indices.push_back(tmp4);
    }

    MeshData mesh(vertices, indices);
    return mesh;
}

MeshData DrawUtils::InitPyramid(glm::vec3 center, glm::vec3 color,
                                glm::vec3 scale) {
    MeshData data;

    std::vector<VertexFormat> vertices{
        VertexFormat(glm::vec3(-0.5, 0, -0.5) * scale + center, color),
        VertexFormat(glm::vec3(0.5, 0, -0.5) * scale + center, color),
        VertexFormat(glm::vec3(0.5, 0, 0.5) * scale + center, color),
        VertexFormat(glm::vec3(-0.5, 0, 0.5) * scale + center, color),
        VertexFormat(glm::vec3(0, 0.5, 0) * scale + center, color),
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,

        2, 3, 0,

        4, 3, 0,

        4, 0, 1,

        4, 1, 2,

        4, 3, 2,
    };

    data.AddMeshData(vertices, indices);

    return data;
}

MeshData DrawUtils::InitTrain(glm::vec3 center) {
    MeshData data =
        InitParallelepiped(glm::vec3(-1, 0.8, 0) + center, glm::vec3(0, 1, 0),
                           glm::vec3(1.4, 1.5, 1));
    int last = data.vertices.size();

    MeshData temp =
        InitParallelepiped(glm::vec3(0, 0, 0) + center, glm::vec3(1, 1, 0),
                           glm::vec3(3.4, 0.1, 1));

    data.AddMeshData(last, temp.vertices, temp.indices);

    data.sizeX = temp.sizeX;
    data.sizeY = temp.sizeY;
    data.sizeZ = temp.sizeZ;

    last = data.vertices.size();

    temp = InitCylinder(glm::vec3(0.5, 0.6, 0) + center, glm::vec3(0, 0, 1),
                        glm::vec3(1, 2, 1), glm::vec3(0, 0, RADIANS(90)));

    data.AddMeshData(last, temp.vertices, temp.indices);

    last = data.vertices.size();

    temp = InitCylinder(glm::vec3(0.5, 0.6, 0) + center, glm::vec3(1, 0, 1),
                        glm::vec3(0.3, 2.3, 0.3), glm::vec3(0, 0, RADIANS(90)));

    data.AddMeshData(last, temp.vertices, temp.indices);

    float off = -0.2;
    for (int i = 0; i < 5; i++) {
        last = data.vertices.size();

        temp =
            InitCylinder(glm::vec3(off + i * 0.6 - 1.0, -0.35, -0.4) + center,
                         glm::vec3(1, 0, 0), glm::vec3(0.6, 0.2, 0.6),
                         glm::vec3(0, RADIANS(90), RADIANS(90)));

        data.AddMeshData(last, temp.vertices, temp.indices);
    }

    off = -0.2;
    for (int i = 0; i < 5; i++) {
        last = data.vertices.size();

        temp = InitCylinder(glm::vec3(off + i * 0.6 - 1.0, -0.35, 0.4) + center,
                            glm::vec3(1, 0, 0), glm::vec3(0.6, 0.2, 0.6),
                            glm::vec3(0, RADIANS(90), RADIANS(90)));

        data.AddMeshData(last, temp.vertices, temp.indices);
    }

    return data;
}

MeshData DrawUtils::InitTrainCar(glm::vec3 center) {
    MeshData data =
        InitParallelepiped(glm::vec3(0, 0.8, 0) + center, glm::vec3(0, 1, 0),
                           glm::vec3(3.4, 1.5, 1));
    int last = data.vertices.size();

    MeshData temp =
        InitParallelepiped(center, glm::vec3(1, 1, 0), glm::vec3(3.4, 0.1, 1));

    data.sizeX = temp.sizeX;
    data.sizeY = temp.sizeY;
    data.sizeZ = temp.sizeZ;

    data.AddMeshData(last, temp.vertices, temp.indices);

    float off = -1.0;
    for (int i = 0; i < 2; i++) {
        last = data.vertices.size();

        temp = InitCylinder(glm::vec3(off + i * 2, -0.35, 0.4) + center,
                            glm::vec3(1, 0, 0), glm::vec3(0.6, 0.2, 0.6),
                            glm::vec3(0, RADIANS(90), RADIANS(90)));

        data.AddMeshData(last, temp.vertices, temp.indices);
    }

    off = -1.0;
    for (int i = 0; i < 2; i++) {
        last = data.vertices.size();

        temp = InitCylinder(glm::vec3(off + i * 2, -0.35, -0.4) + center,
                            glm::vec3(1, 0, 0), glm::vec3(0.6, 0.2, 0.6),
                            glm::vec3(0, RADIANS(90), RADIANS(90)));

        data.AddMeshData(last, temp.vertices, temp.indices);
    }

    return data;
}

MeshData DrawUtils::InitRail1(int x1, int x2, float h, int z1, int z2,
                              const glm::vec3 &color) {
    MeshData data;
    vector<VertexFormat> vertices;

    glm::vec3 p0 = glm::vec3(x1, h, z1);
    glm::vec3 p1 = glm::vec3(x2, h, z1);
    glm::vec3 p2 = glm::vec3(x2, h, z2);
    glm::vec3 p3 = glm::vec3(x1, h, z2);

    glm::vec3 n1 = glm::cross(p1 - p0, p2 - p0);
    glm::vec3 n2 = glm::cross(p2 - p0, p3 - p0);
    glm::vec3 normal = glm::normalize(n1 + n2);

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p1, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));
    vertices.push_back(VertexFormat(p3, normal, color));

    data.vertices = vertices;

    return data;
}

MeshData DrawUtils::InitRail2x(int x1, int x2, float h1, float h2, int z1,
                               int z2, const glm::vec3 &color) {
    // p0 = glm::vec3(x, heights[x - 1][z] + 0.01f, z);
    // p1 = glm::vec3(x + 1, heights[x + 1][z] + 0.01f, z);
    // p2 = glm::vec3(x + 1, heights[x + 1][z] + 0.01f, z + 1);
    // p3 = glm::vec3(x, heights[x - 1][z] + 0.01f, z + 1);
    //
    // glm::vec3 temp1(x + 0.5f, h, z);
    // glm::vec3 temp2(x + 0.5f, h, z + 1);
    //
    // addVerticesV(vertices, p0, p1, p2, p3, temp1, temp2, color);

    MeshData data;
    vector<VertexFormat> vertices;

    glm::vec3 p0, p1, p2, p3;
    p0 = glm::vec3(x1, h1, z1);
    p1 = glm::vec3(x2, h2, z1);
    p2 = glm::vec3(x2, h2, z2);
    p3 = glm::vec3(x1, h1, z2);

    glm::vec3 n1 = glm::cross(p1 - p0, p2 - p0);
    glm::vec3 n2 = glm::cross(p2 - p0, p3 - p0);
    glm::vec3 normal = glm::normalize(n1 + n2);

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p1, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));
    vertices.push_back(VertexFormat(p3, normal, color));

    data.vertices = vertices;

    return data;
}

MeshData DrawUtils::InitRail2z(int x1, int x2, float h1, float h2, int z1,
                               int z2, const glm::vec3 &color) {
    // p0 = glm::vec3(x, heights[x][z - 1] + 0.01f, z);
    // p1 = glm::vec3(x, heights[x][z - 1] + 0.01f, z + 1);
    // p2 = glm::vec3(x + 1, heights[x][z + 1] + 0.01f, z + 1);
    // p3 = glm::vec3(x + 1, heights[x][z + 1] + 0.01f, z);
    //
    // glm::vec3 temp1(x, h, z + 0.5f);
    // glm::vec3 temp2(x + 1, h, z + 0.5f);
    MeshData data;
    vector<VertexFormat> vertices;

    glm::vec3 p0, p1, p2, p3;
    p0 = glm::vec3(x1, h1, z1);
    p1 = glm::vec3(x1, h2, z2);
    p2 = glm::vec3(x2, h2, z2);
    p3 = glm::vec3(x2, h1, z1);

    glm::vec3 n1 = glm::cross(p1 - p0, p2 - p0);
    glm::vec3 n2 = glm::cross(p2 - p0, p3 - p0);
    glm::vec3 normal = glm::normalize(n1 + n2);

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p1, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));
    vertices.push_back(VertexFormat(p3, normal, color));

    data.vertices = vertices;

    return data;
}

MeshData DrawUtils::InitRailVx(int x, float h, float h1, float h2, int z,
                               const glm::vec3 &color) {
    MeshData data;
    vector<VertexFormat> vertices;

    glm::vec3 p0, p1, p2, p3;
    p0 = glm::vec3(x, h1 + 0.01f, z);
    p1 = glm::vec3(x + 1, h2 + 0.01f, z);
    p2 = glm::vec3(x + 1, h2 + 0.01f, z + 1);
    p3 = glm::vec3(x, h1 + 0.01f, z + 1);

    glm::vec3 temp1(x + 0.5f, h, z);
    glm::vec3 temp2(x + 0.5f, h, z + 1);

    glm::vec3 n0 = glm::normalize(glm::cross(temp1 - p0, p3 - p0));
    glm::vec3 n1 = glm::normalize(glm::cross(temp1 - temp2, p3 - temp2));

    vertices.push_back(VertexFormat(p0, n0, color));
    vertices.push_back(VertexFormat(temp1, n0, color));
    vertices.push_back(VertexFormat(p3, n0, color));

    vertices.push_back(VertexFormat(temp1, n1, color));
    vertices.push_back(VertexFormat(temp2, n1, color));
    vertices.push_back(VertexFormat(p3, n1, color));

    glm::vec3 n2 = glm::normalize(glm::cross(p1 - temp1, p2 - temp1));
    glm::vec3 n3 = glm::normalize(glm::cross(p2 - temp2, temp2 - temp1));

    vertices.push_back(VertexFormat(temp1, n2, color));
    vertices.push_back(VertexFormat(p1, n2, color));
    vertices.push_back(VertexFormat(p2, n1, color));

    vertices.push_back(VertexFormat(temp1, n3, color));
    vertices.push_back(VertexFormat(p2, n3, color));
    vertices.push_back(VertexFormat(temp2, n3, color));

    data.vertices = vertices;

    return data;
}

MeshData DrawUtils::InitRailVz(int x, float h, float h1, float h2, int z,
                               const glm::vec3 &color) {
    MeshData data;
    vector<VertexFormat> vertices;

    glm::vec3 p0, p1, p2, p3;
    p0 = glm::vec3(x, h1 + 0.01f, z);
    p1 = glm::vec3(x, h1 + 0.01f, z + 1);
    p2 = glm::vec3(x + 1, h2 + 0.01f, z + 1);
    p3 = glm::vec3(x + 1, h2 + 0.01f, z);

    glm::vec3 temp1(x, h, z + 0.5f);
    glm::vec3 temp2(x + 1, h, z + 0.5f);

    glm::vec3 n0 = glm::normalize(glm::cross(temp1 - p0, p3 - p0));
    glm::vec3 n1 = glm::normalize(glm::cross(temp1 - temp2, p3 - temp2));

    vertices.push_back(VertexFormat(p0, n0, color));
    vertices.push_back(VertexFormat(temp1, n0, color));
    vertices.push_back(VertexFormat(p3, n0, color));

    vertices.push_back(VertexFormat(temp1, n1, color));
    vertices.push_back(VertexFormat(temp2, n1, color));
    vertices.push_back(VertexFormat(p3, n1, color));

    glm::vec3 n2 = glm::normalize(glm::cross(p1 - temp1, p2 - temp1));
    glm::vec3 n3 = glm::normalize(glm::cross(p2 - temp2, temp2 - temp1));

    vertices.push_back(VertexFormat(temp1, n2, color));
    vertices.push_back(VertexFormat(p1, n2, color));
    vertices.push_back(VertexFormat(p2, n1, color));

    vertices.push_back(VertexFormat(temp1, n3, color));
    vertices.push_back(VertexFormat(p2, n3, color));
    vertices.push_back(VertexFormat(temp2, n3, color));

    data.vertices = vertices;

    return data;
}

MeshData DrawUtils::InitRailRiverx(int x, float h, int z,
                                   const glm::vec3 &color) {
    MeshData data;
    vector<VertexFormat> vertices;

    glm::vec3 p0, p1, p2, p3;
    p0 = glm::vec3(x, h, z);
    p1 = glm::vec3(x + 1, h, z);
    p2 = glm::vec3(x + 1, h, z + 1);
    p3 = glm::vec3(x, h, z + 1);

    glm::vec3 p4 = glm::vec3(x, h, z + 1.0 / 4);
    glm::vec3 p5 = glm::vec3(x + 1, h, z + 1.0 / 4);
    glm::vec3 p6 = glm::vec3(x, h, z + 1 - 1.0 / 4);
    glm::vec3 p7 = glm::vec3(x + 1, h, z + 1 - 1.0 / 4);

    glm::vec3 p8 = glm::vec3(x, h, z + 0.45);
    glm::vec3 p9 = glm::vec3(x + 1, h, z + 0.45);
    glm::vec3 p10 = glm::vec3(x, h, z + 0.55);
    glm::vec3 p11 = glm::vec3(x + 1, h, z + 0.55);

    glm::vec3 p12 = glm::vec3(x + 0.25, h, z);
    glm::vec3 p13 = glm::vec3(x + 0.75, h, z);
    glm::vec3 p14 = glm::vec3(x + 0.25, h, z + 1);
    glm::vec3 p15 = glm::vec3(x + 0.75, h, z + 1);
    glm::vec3 n1 = glm::cross(p1 - p0, p2 - p0);
    glm::vec3 n2 = glm::cross(p2 - p0, p3 - p0);
    glm::vec3 normal = glm::normalize(n1 + n2);

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p1, normal, color));
    vertices.push_back(VertexFormat(p5, normal, color));

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p5, normal, color));
    vertices.push_back(VertexFormat(p4, normal, color));

    vertices.push_back(VertexFormat(p6, normal, color));
    vertices.push_back(VertexFormat(p7, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p6, normal, color));
    vertices.push_back(VertexFormat(p3, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p8, normal, color));
    vertices.push_back(VertexFormat(p9, normal, color));
    vertices.push_back(VertexFormat(p11, normal, color));

    vertices.push_back(VertexFormat(p8, normal, color));
    vertices.push_back(VertexFormat(p11, normal, color));
    vertices.push_back(VertexFormat(p10, normal, color));

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p12, normal, color));
    vertices.push_back(VertexFormat(p14, normal, color));

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p14, normal, color));
    vertices.push_back(VertexFormat(p3, normal, color));

    vertices.push_back(VertexFormat(p13, normal, color));
    vertices.push_back(VertexFormat(p1, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p13, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));
    vertices.push_back(VertexFormat(p15, normal, color));

    data.vertices = vertices;

    return data;
}

MeshData DrawUtils::InitRailRiverz(int x, float h, int z,
                                   const glm::vec3 &color) {
    MeshData data;
    vector<VertexFormat> vertices;

    glm::vec3 p0, p1, p2, p3;
    p0 = glm::vec3(x, h, z);
    p1 = glm::vec3(x, h, z + 1);
    p2 = glm::vec3(x + 1, h, z + 1);
    p3 = glm::vec3(x + 1, h, z);

    glm::vec3 p4 = glm::vec3(x + 1.0 / 4, h, z);
    glm::vec3 p5 = glm::vec3(x + 1.0 / 4, h, z + 1);
    glm::vec3 p6 = glm::vec3(x + 1 - 1.0 / 4, h, z);
    glm::vec3 p7 = glm::vec3(x + 1 - 1.0 / 4, h, z + 1);

    glm::vec3 p8 = glm::vec3(x + 0.45, h, z);
    glm::vec3 p9 = glm::vec3(x + 0.45, h, z + 1);
    glm::vec3 p10 = glm::vec3(x + 0.55, h, z);
    glm::vec3 p11 = glm::vec3(x + 0.55, h, z + 1);

    glm::vec3 p12 = glm::vec3(x, h, z + 0.25);
    glm::vec3 p13 = glm::vec3(x + 1, h, z + 0.25);
    glm::vec3 p14 = glm::vec3(x, h, z + 0.75);
    glm::vec3 p15 = glm::vec3(x + 1, h, z + 0.75);

    glm::vec3 n1 = glm::cross(p1 - p0, p2 - p0);
    glm::vec3 n2 = glm::cross(p2 - p0, p3 - p0);
    glm::vec3 normal = glm::normalize(n1 + n2);

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p1, normal, color));
    vertices.push_back(VertexFormat(p5, normal, color));

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p5, normal, color));
    vertices.push_back(VertexFormat(p4, normal, color));

    vertices.push_back(VertexFormat(p6, normal, color));
    vertices.push_back(VertexFormat(p7, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p6, normal, color));
    vertices.push_back(VertexFormat(p3, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p8, normal, color));
    vertices.push_back(VertexFormat(p9, normal, color));
    vertices.push_back(VertexFormat(p11, normal, color));

    vertices.push_back(VertexFormat(p8, normal, color));
    vertices.push_back(VertexFormat(p11, normal, color));
    vertices.push_back(VertexFormat(p10, normal, color));

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p12, normal, color));
    vertices.push_back(VertexFormat(p3, normal, color));

    vertices.push_back(VertexFormat(p12, normal, color));
    vertices.push_back(VertexFormat(p13, normal, color));
    vertices.push_back(VertexFormat(p3, normal, color));

    vertices.push_back(VertexFormat(p14, normal, color));
    vertices.push_back(VertexFormat(p1, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p14, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));
    vertices.push_back(VertexFormat(p15, normal, color));

    data.vertices = vertices;
    return data;
}

MeshData DrawUtils::InitRailMx(int x, float h, int z, const glm::vec3 &color) {
    MeshData data;
    vector<VertexFormat> vertices;

    glm::vec3 p0, p1, p2, p3;
    p0 = glm::vec3(x, h, z);
    p1 = glm::vec3(x, h, z + 1);
    p2 = glm::vec3(x + 1, h, z + 1);
    p3 = glm::vec3(x + 1, h, z);

    glm::vec3 p4 = glm::vec3(x + 0.1, h, z);
    glm::vec3 p5 = glm::vec3(x + 0.1, h, z + 1);
    glm::vec3 p6 = glm::vec3(x + 1 - 0.1, h, z);
    glm::vec3 p7 = glm::vec3(x + 1 - 0.1, h, z + 1);

    glm::vec3 p8 = glm::vec3(x + 0.20, h, z);
    glm::vec3 p9 = glm::vec3(x + 0.20, h, z + 1);
    glm::vec3 p10 = glm::vec3(x + 0.30, h, z);
    glm::vec3 p11 = glm::vec3(x + 0.30, h, z + 1);

    glm::vec3 p12 = glm::vec3(x + 0.70, h, z);
    glm::vec3 p13 = glm::vec3(x + 0.70, h, z + 1);
    glm::vec3 p14 = glm::vec3(x + 0.80, h, z);
    glm::vec3 p15 = glm::vec3(x + 0.80, h, z + 1);

    glm::vec3 p16 = glm::vec3(x + 0.45, h, z);
    glm::vec3 p17 = glm::vec3(x + 0.45, h, z + 1);
    glm::vec3 p18 = glm::vec3(x + 0.55, h, z);
    glm::vec3 p19 = glm::vec3(x + 0.55, h, z + 1);

    glm::vec3 n1 = glm::cross(p1 - p0, p2 - p0);
    glm::vec3 n2 = glm::cross(p2 - p0, p3 - p0);
    glm::vec3 normal = glm::normalize(n1 + n2);

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p1, normal, color));
    vertices.push_back(VertexFormat(p5, normal, color));

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p5, normal, color));
    vertices.push_back(VertexFormat(p4, normal, color));

    vertices.push_back(VertexFormat(p6, normal, color));
    vertices.push_back(VertexFormat(p7, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p6, normal, color));
    vertices.push_back(VertexFormat(p3, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p8, normal, color));
    vertices.push_back(VertexFormat(p9, normal, color));
    vertices.push_back(VertexFormat(p11, normal, color));

    vertices.push_back(VertexFormat(p8, normal, color));
    vertices.push_back(VertexFormat(p11, normal, color));
    vertices.push_back(VertexFormat(p10, normal, color));

    vertices.push_back(VertexFormat(p12, normal, color));
    vertices.push_back(VertexFormat(p13, normal, color));
    vertices.push_back(VertexFormat(p14, normal, color));

    vertices.push_back(VertexFormat(p14, normal, color));
    vertices.push_back(VertexFormat(p13, normal, color));
    vertices.push_back(VertexFormat(p15, normal, color));

    vertices.push_back(VertexFormat(p16, normal, color));
    vertices.push_back(VertexFormat(p17, normal, color));
    vertices.push_back(VertexFormat(p18, normal, color));

    vertices.push_back(VertexFormat(p18, normal, color));
    vertices.push_back(VertexFormat(p17, normal, color));
    vertices.push_back(VertexFormat(p19, normal, color));

    data.vertices = vertices;

    return data;
}

MeshData DrawUtils::InitRailMz(int x, float h, int z, const glm::vec3 &color) {
    MeshData data;
    vector<VertexFormat> vertices;

    glm::vec3 p0, p1, p2, p3;
    p0 = glm::vec3(x, h, z);
    p1 = glm::vec3(x, h, z + 1);
    p2 = glm::vec3(x + 1, h, z + 1);
    p3 = glm::vec3(x + 1, h, z);

    glm::vec3 p4 = glm::vec3(x + 1.0 / 4, h, z);
    glm::vec3 p5 = glm::vec3(x + 1.0 / 4, h, z + 1);
    glm::vec3 p6 = glm::vec3(x + 1 - 1.0 / 4, h, z);
    glm::vec3 p7 = glm::vec3(x + 1 - 1.0 / 4, h, z + 1);

    glm::vec3 p8 = glm::vec3(x + 0.45, h, z);
    glm::vec3 p9 = glm::vec3(x + 0.45, h, z + 1);
    glm::vec3 p10 = glm::vec3(x + 0.55, h, z);
    glm::vec3 p11 = glm::vec3(x + 0.55, h, z + 1);

    glm::vec3 p12 = glm::vec3(x, h, z + 0.25);
    glm::vec3 p13 = glm::vec3(x + 1, h, z + 0.25);
    glm::vec3 p14 = glm::vec3(x, h, z + 0.75);
    glm::vec3 p15 = glm::vec3(x + 1, h, z + 0.75);

    glm::vec3 n1 = glm::cross(p1 - p0, p2 - p0);
    glm::vec3 n2 = glm::cross(p2 - p0, p3 - p0);
    glm::vec3 normal = glm::normalize(n1 + n2);

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p1, normal, color));
    vertices.push_back(VertexFormat(p5, normal, color));

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p5, normal, color));
    vertices.push_back(VertexFormat(p4, normal, color));

    vertices.push_back(VertexFormat(p6, normal, color));
    vertices.push_back(VertexFormat(p7, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p6, normal, color));
    vertices.push_back(VertexFormat(p3, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p8, normal, color));
    vertices.push_back(VertexFormat(p9, normal, color));
    vertices.push_back(VertexFormat(p11, normal, color));

    vertices.push_back(VertexFormat(p8, normal, color));
    vertices.push_back(VertexFormat(p11, normal, color));
    vertices.push_back(VertexFormat(p10, normal, color));

    vertices.push_back(VertexFormat(p0, normal, color));
    vertices.push_back(VertexFormat(p12, normal, color));
    vertices.push_back(VertexFormat(p3, normal, color));

    vertices.push_back(VertexFormat(p12, normal, color));
    vertices.push_back(VertexFormat(p13, normal, color));
    vertices.push_back(VertexFormat(p3, normal, color));

    vertices.push_back(VertexFormat(p14, normal, color));
    vertices.push_back(VertexFormat(p1, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));

    vertices.push_back(VertexFormat(p14, normal, color));
    vertices.push_back(VertexFormat(p2, normal, color));
    vertices.push_back(VertexFormat(p15, normal, color));

    data.vertices = vertices;
    return data;
}

MeshData DrawUtils::InitStation1(const glm::vec3 &center) {
    MeshData data;

    MeshData building =
        InitParallelepiped(center + glm::vec3(0, 1.5f, 0),
                           glm::vec3(0.71f, 0.54f, 0.34f), glm::vec3(7, 3, 4));
    int last = data.vertices.size();
    data.AddMeshData(last, building.vertices, building.indices);

    MeshData roof =
        InitPyramid(center + glm::vec3(0, 3, 0), glm::vec3(0.71f, 0.54f, 0.34f),
                    glm::vec3(9, 4, 6));
    last = data.vertices.size();
    data.AddMeshData(last, roof.vertices, roof.indices);

    glm::vec3 pillarColor = glm::vec3(0.3f, 0.2f, 0.1f);

    std::vector<glm::vec3> pillarOffsets = {
        {-3, 0, -2.5}, {3, 0, -2.5}, {-3, 0, 2.5}, {3, 0, 2.5}};

    for (auto &off : pillarOffsets) {
        MeshData pillar =
            InitCylinder(center + off + glm::vec3(0, 1, 0), pillarColor,
                         glm::vec3(0.3f, 4, 0.3f), glm::vec3(0, 0, 0));
        int last = data.vertices.size();
        data.AddMeshData(last, pillar.vertices, pillar.indices);
    }

    MeshData sign = InitParallelepiped(center + glm::vec3(0, 3, -3.2f),
                                       glm::vec3(0.9f, 0.9f, 0.6f),
                                       glm::vec3(3.5f, 1, 0.2f));
    last = data.vertices.size();
    data.AddMeshData(last, sign.vertices, sign.indices);

    MeshData wood =
        InitCylinder(center + glm::vec3(0, 3, -3.4f), pillarColor,
                     glm::vec3(0.2f, 3, 0.2f), glm::vec3(0, 0, RADIANS(-80)));

    last = data.vertices.size();
    data.AddMeshData(last, wood.vertices, wood.indices);

    return data;
}

MeshData DrawUtils::InitStation2(const glm::vec3 &center) {
    MeshData data;

    MeshData building =
        InitParallelepiped(center + glm::vec3(0, 1.5f, 0),
                           glm::vec3(0.39f, 0.39f, 0.39f), glm::vec3(7, 3, 4));
    int last = data.vertices.size();
    data.AddMeshData(last, building.vertices, building.indices);

    MeshData roof = InitHemisphere(center + glm::vec3(0, 3, 0),
                                   glm::vec3(0.24, 0.24, 0.24), 4.5f, 32, 32);

    last = data.vertices.size();
    data.AddMeshData(last, roof.vertices, roof.indices);

    glm::vec3 pillarColor = glm::vec3(0.3f, 0.2f, 0.1f);

    std::vector<glm::vec3> pillarOffsets = {
        {-3, 0, -2.5}, {3, 0, -2.5}, {-3, 0, 2.5}, {3, 0, 2.5}};

    for (auto &off : pillarOffsets) {
        MeshData pillar =
            InitCylinder(center + off + glm::vec3(0, 1, 0), pillarColor,
                         glm::vec3(0.3f, 4, 0.3f), glm::vec3(0, 0, 0));
        int last = data.vertices.size();
        data.AddMeshData(last, pillar.vertices, pillar.indices);
    }

    MeshData sign = InitParallelepiped(center + glm::vec3(0, 3, -3.2f),
                                       glm::vec3(0.9f, 0.9f, 0.6f),
                                       glm::vec3(3.5f, 1, 0.2f));
    last = data.vertices.size();
    data.AddMeshData(last, sign.vertices, sign.indices);

    MeshData coal =
        InitParallelepiped(center + glm::vec3(0, 3, -3.2f), glm::vec3(0, 0, 0),
                           glm::vec3(0.5, 0.5, 0.5));
    last = data.vertices.size();
    data.AddMeshData(last, coal.vertices, coal.indices);

    return data;
}

MeshData DrawUtils::InitStation3(const glm::vec3 &center) {
    MeshData data;

    MeshData building =
        InitParallelepiped(center + glm::vec3(0, 1.5f, 0),
                           glm::vec3(0.96f, 0.87f, 0.70f), glm::vec3(7, 3, 4));
    int last = data.vertices.size();
    data.AddMeshData(last, building.vertices, building.indices);

    MeshData roof =
        InitParallelepiped(center + glm::vec3(0, 3, 0),
                           glm::vec3(0.92, 0.58, 0), glm::vec3(9, 1, 6));

    last = data.vertices.size();
    data.AddMeshData(last, roof.vertices, roof.indices);

    glm::vec3 pillarColor = glm::vec3(0.3f, 0.2f, 0.1f);

    std::vector<glm::vec3> pillarOffsets = {
        {-3, 0, -2.5}, {3, 0, -2.5}, {-3, 0, 2.5}, {3, 0, 2.5}};

    for (auto &off : pillarOffsets) {
        MeshData pillar =
            InitCylinder(center + off + glm::vec3(0, 1, 0), pillarColor,
                         glm::vec3(0.3f, 4, 0.3f), glm::vec3(0, 0, 0));
        int last = data.vertices.size();
        data.AddMeshData(last, pillar.vertices, pillar.indices);
    }

    MeshData sign = InitParallelepiped(center + glm::vec3(0, 3, -3.2f),
                                       glm::vec3(0.9f, 0.9f, 0.6f),
                                       glm::vec3(3.5f, 1, 0.2f));
    last = data.vertices.size();
    data.AddMeshData(last, sign.vertices, sign.indices);

    return data;
}

MeshData DrawUtils::InitStation4(const glm::vec3 &center) {
    MeshData data;

    glm::vec3 baseColor = glm::vec3(0.39f, 0.39f, 0.39f);
    glm::vec3 middleColor = glm::vec3(0.55f, 0.55f, 0.55f);
    glm::vec3 topColor = glm::vec3(0.7f, 0.7f, 0.7f);

    glm::vec3 size = glm::vec3(7, 2, 4);
    glm::vec3 pos = center + glm::vec3(0, size.y * 0.5f, 0);

    MeshData bottom = InitParallelepiped(pos, baseColor, size);
    data.AddMeshData(data.vertices.size(), bottom.vertices, bottom.indices);

    size = glm::vec3(5, 2, 3);
    pos = center + glm::vec3(0, 2 + size.y * 0.5f, 0);

    MeshData mid = InitParallelepiped(pos, middleColor, size);
    data.AddMeshData(data.vertices.size(), mid.vertices, mid.indices);

    MeshData sign = InitParallelepiped(center + glm::vec3(0, 3, -1.5f),
                                       glm::vec3(0.9f, 0.9f, 0.6f),
                                       glm::vec3(3.5f, 1, 0.2f));
    int last = data.vertices.size();
    data.AddMeshData(last, sign.vertices, sign.indices);

    return data;
}

MeshData DrawUtils::InitWood(const glm::vec3 &center) {
    MeshData data;

    glm::vec3 color = glm::vec3(0.3f, 0.2f, 0.1f);

    MeshData wood = InitCylinder(center, color, glm::vec3(0.2f, 1, 0.2f),
                                 glm::vec3(0, 0, RADIANS(-80)));

    data.AddMeshData(wood.vertices, wood.indices);
    return data;
}

MeshData DrawUtils::InitCoal(const glm::vec3 &center) {
    MeshData coal = InitParallelepiped(center, glm::vec3(0, 0, 0),
                                       glm::vec3(0.5, 0.5, 0.5));

    return coal;
}

MeshData DrawUtils::InitPlant(const glm::vec3 &center) {
    glm::vec3 stemColor = glm::vec3(0.4, 0.6, 0.3);
    glm::vec3 leafColor = glm::vec3(0.76, 0.94, 0.54);

    MeshData plant =
        InitCylinder(center + glm::vec3(0, 1.5, 0) * 0.3f, stemColor,
                     glm::vec3(0.2, 3, 0.2) * 0.3f, glm::vec3(0, 0, 0));

    std::vector<VertexFormat> lowerLeaves{
        VertexFormat(glm::vec3(0, 0.5, 0) * 0.3f + center, glm::vec3(0, 1, 0),
                     leafColor),
        VertexFormat(glm::vec3(0.8, 0.6, 0.3) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
        VertexFormat(glm::vec3(0.6, 0.4, 0.2) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),

        VertexFormat(glm::vec3(0, 0.6, 0) * 0.3f + center, glm::vec3(0, 1, 0),
                     leafColor),
        VertexFormat(glm::vec3(-0.7, 0.5, -0.4) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
        VertexFormat(glm::vec3(-0.5, 0.7, -0.3) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
    };

    std::vector<unsigned int> lowerIndices = {
        0, 1, 2, 3, 4, 5,
    };

    int last = plant.vertices.size();
    plant.AddMeshData(last, lowerLeaves, lowerIndices);

    std::vector<VertexFormat> middleLeaves{
        VertexFormat(glm::vec3(0, 1.2, 0) * 0.3f + center, glm::vec3(0, 1, 0),
                     leafColor),
        VertexFormat(glm::vec3(-0.6, 1.3, 0.5) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
        VertexFormat(glm::vec3(-0.4, 1.1, 0.4) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),

        VertexFormat(glm::vec3(0, 1.3, 0) * 0.3f + center, glm::vec3(0, 1, 0),
                     leafColor),
        VertexFormat(glm::vec3(0.7, 1.2, -0.3) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
        VertexFormat(glm::vec3(0.5, 1.4, -0.2) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),

        VertexFormat(glm::vec3(0, 1.25, 0) * 0.3f + center, glm::vec3(0, 1, 0),
                     leafColor),
        VertexFormat(glm::vec3(0.2, 1.4, 0.8) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
        VertexFormat(glm::vec3(-0.1, 1.3, 0.6) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
    };

    std::vector<unsigned int> middleIndices = {
        0, 1, 2, 3, 4, 5, 6, 7, 8,
    };

    last = plant.vertices.size();
    plant.AddMeshData(last, middleLeaves, middleIndices);

    std::vector<VertexFormat> upperLeaves{
        VertexFormat(glm::vec3(0, 2.0, 0) * 0.3f + center, glm::vec3(0, 1, 0),
                     leafColor),
        VertexFormat(glm::vec3(0.6, 2.2, 0.1) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
        VertexFormat(glm::vec3(0.4, 1.9, 0) * 0.3f + center, glm::vec3(0, 1, 0),
                     leafColor),

        VertexFormat(glm::vec3(0, 2.1, 0) * 0.3f + center, glm::vec3(0, 1, 0),
                     leafColor),
        VertexFormat(glm::vec3(-0.5, 2.0, -0.2) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
        VertexFormat(glm::vec3(-0.3, 2.2, -0.1) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),

        VertexFormat(glm::vec3(0, 2.15, 0) * 0.3f + center, glm::vec3(0, 1, 0),
                     leafColor),
        VertexFormat(glm::vec3(-0.1, 2.3, -0.6) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
        VertexFormat(glm::vec3(0.1, 2.1, -0.4) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
    };

    std::vector<unsigned int> upperIndices = {
        0, 1, 2, 3, 4, 5, 6, 7, 8,
    };

    last = plant.vertices.size();
    plant.AddMeshData(last, upperLeaves, upperIndices);

    std::vector<VertexFormat> topLeaves{
        VertexFormat(glm::vec3(0, 2.6, 0) * 0.3f + center, glm::vec3(0, 1, 0),
                     leafColor),
        VertexFormat(glm::vec3(0.3, 2.7, 0.2) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
        VertexFormat(glm::vec3(0.2, 2.5, 0.1) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),

        VertexFormat(glm::vec3(0, 2.65, 0) * 0.3f + center, glm::vec3(0, 1, 0),
                     leafColor),
        VertexFormat(glm::vec3(-0.2, 2.6, -0.3) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
        VertexFormat(glm::vec3(-0.1, 2.75, -0.2) * 0.3f + center,
                     glm::vec3(0, 1, 0), leafColor),
    };

    std::vector<unsigned int> topIndices = {
        0, 1, 2, 3, 4, 5,
    };

    last = plant.vertices.size();
    plant.AddMeshData(last, topLeaves, topIndices);

    for (auto &v : plant.vertices) {
        v.position.y -= 1.5f * 0.3f;
    }

    return plant;
}
