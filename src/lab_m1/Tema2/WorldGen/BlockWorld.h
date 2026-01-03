#pragma once

#include "core/gpu/mesh.h"
#include "lab_m1/Tema2/DrawUtils.h"
#include "lab_m1/Tema2/MeshData.h"
#include "lab_m1/Tema2/WorldGen/PerlinNoise.h"
#include "lab_m1/Tema2/WorldGen/Special.h"
#include <cmath>
#include <memory>
#include <vector>

#define WORLD_WIDTH 150
#define WORLD_HEIGHT 13
#define WORLD_DEPTH 150
#define RAILS_HEIGHT 5
#define RIVER_HEIGHT 3

enum class BlockType {
    Air = 0,
    Grass,
    River,
    Station,
};

class BlockWorld {
  public:
    BlockWorld();
    ~BlockWorld();

    void GenerateTerrain();
    void GenerateRiver();

    void GenerateMesh();
    bool CheckBlockExists(int x, int y, int z);
    void AddCubeFace(std::vector<VertexFormat> &vertices, float x, float y,
                     float z, int face, BlockType type);
    void AddRailTile(std::vector<VertexFormat> &vertices, int x, int y, int z,
                     int sw);
    bool RailCoversBlock(int x, int y, int z);
    vector<pair<int, int>> ComputeNumberOfNeighs(int x, int z);

    PerlinNoise perlin;
    Mesh *terrainMesh;
    int heights[WORLD_WIDTH][WORLD_DEPTH];
    int train[WORLD_WIDTH][WORLD_DEPTH];

  private:
    unique_ptr<DrawUtils> dr;
    MeshData mesh;
    BlockType blocks[WORLD_WIDTH][WORLD_HEIGHT + 1][WORLD_DEPTH];
    int riverRails[WORLD_WIDTH][WORLD_DEPTH];
    int mouintainRails[WORLD_WIDTH][WORLD_DEPTH];
    std::vector<Special> temp;
    std::map<std::pair<int, int>, int> special_coords;

    inline float maxf(float a, float b) { return (a > b) ? a : b; }
    glm::vec3 computeColor(int y);
    class WorldGeneration {
        BlockWorld &self;

      public:
        explicit WorldGeneration(BlockWorld &s) : self(s) {}
        void computeMinx(int start, int end, int idx);

        void computeMinz(int start, int end, int idx);

        void flattenTx(int start, int end, int idx);

        void flattenTz(int start, int end, int idx);

        void addRailsToStationx(int start, int end, int offx, int offz,
                                int idx);
        void addRailsToStationz(int start, int end, int offx, int offz,
                                int idx);
        void addStationRails(int x, int z, int offx, int offz);

        void flattenTerrain();
        void flattenTerrainStation(int x, int z, int h);
    };
    void generate();
    void addVerticesV(std::vector<VertexFormat> &vertices, const glm::vec3 &p0,
                      const glm::vec3 &p1, const glm::vec3 &p2,
                      const glm::vec3 &p3, const glm::vec3 &temp1,
                      const glm::vec3 &temp2, const glm::vec3 &color);
    void addVerticesN(std::vector<VertexFormat> &vertices, const glm::vec3 &p0,
                      const glm::vec3 &p1, const glm::vec3 &p2,
                      const glm::vec3 &p3, const glm::vec3 &color);
    bool checkBlocksUnderIsRiver(int x, int z);
    bool checkBlockAboveExists(int x, int z);
    WorldGeneration wg;
};
