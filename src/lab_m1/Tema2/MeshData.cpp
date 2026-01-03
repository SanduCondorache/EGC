#include "core/gpu/vertex_format.h"
#include "lab_m1/Tema2/MeshData.h"

MeshData::MeshData() {}

MeshData::MeshData(std::vector<VertexFormat> vertices,
                   std::vector<unsigned int> indices)
    : vertices(vertices), indices(indices) {}

MeshData::~MeshData() {}

void MeshData::AddMeshData(MeshData m) {
    for (const auto &v : m.vertices) {
        vertices.push_back(v);
    }

    for (const auto &v : m.indices) {
        indices.push_back(v);
    }
}

void MeshData::AddMeshData(std::vector<VertexFormat> verts,
                           std::vector<unsigned int> idx) {
    for (const auto &v : verts) {
        vertices.push_back(v);
    }

    for (const auto &i : idx) {
        indices.push_back(i);
    }
}

void MeshData::AddMeshData(int offset, std::vector<VertexFormat> verts,
                           std::vector<unsigned int> idx) {

    for (const auto &v : verts) {
        vertices.push_back(v);
    }

    for (const auto &i : idx) {
        indices.push_back(i + offset);
    }
}
