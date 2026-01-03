#pragma once

#include "core/gpu/vertex_format.h"
#include <vector>

struct MeshData {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    float sizeX;
    float sizeY;
    float sizeZ;

    MeshData();
    MeshData(std::vector<VertexFormat> vertices,
             std::vector<unsigned int> indices);
    ~MeshData();

    void AddMeshData(MeshData m);
    void AddMeshData(std::vector<VertexFormat> vertices,
                     std::vector<unsigned int> indices);
    void AddMeshData(int offset, std::vector<VertexFormat> vertices,
                     std::vector<unsigned int> indices);
};
