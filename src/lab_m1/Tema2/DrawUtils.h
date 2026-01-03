#pragma once

#include "core/gpu/vertex_format.h"
#include "lab_m1/Tema2/MeshData.h"

using namespace std;

class DrawUtils {
  public:
    DrawUtils();
    ~DrawUtils();

    MeshData InitTrain(glm::vec3 center);
    MeshData InitTrainCar(glm::vec3 center);
    MeshData InitRail1(int x1, int x2, float h, int z1, int z2,
                       const glm::vec3 &color);
    MeshData InitRail2x(int x1, int x2, float h1, float h2, int z1, int z2,
                        const glm::vec3 &color);
    MeshData InitRail2z(int x1, int x2, float h1, float h2, int z1, int z2,
                        const glm::vec3 &color);
    MeshData InitRailRiverx(int x, float h, int z, const glm::vec3 &color);
    MeshData InitRailRiverz(int x, float h, int z, const glm::vec3 &color);
    MeshData InitRailVx(int x, float h, float h1, float h2, int z,
                        const glm::vec3 &color);
    MeshData InitRailVz(int x, float h, float h1, float h2, int z,
                        const glm::vec3 &color);
    MeshData InitRailMx(int x, float h, int z, const glm::vec3 &color);
    MeshData InitRailMz(int x, float h, int z, const glm::vec3 &color);
    MeshData InitStation1(const glm::vec3 &center);
    MeshData InitStation2(const glm::vec3 &center);
    MeshData InitStation3(const glm::vec3 &center);
    MeshData InitStation4(const glm::vec3 &center);
    MeshData InitWood(const glm::vec3 &center);
    MeshData InitCoal(const glm::vec3 &center);
    MeshData InitPlant(const glm::vec3 &center);

  private:
    MeshData InitSquare(glm::vec3 center, glm::vec3 color, glm::vec3 scale);
    MeshData InitParallelepiped(glm::vec3 center, glm::vec3 color,
                                glm::vec3 scale);
    MeshData InitHemisphere(glm::vec3 center, glm::vec3 color, float radius,
                            int stacks, int slices);

    MeshData InitCylinder(glm::vec3 center, glm::vec3 color, glm::vec3 scale,
                          glm::vec3 rotationEuler);
    MeshData InitPyramid(glm::vec3 center, glm::vec3 color, glm::vec3 scale);

    glm::vec3 position(const glm::vec3 &p, const glm::mat4 &rot,
                       const glm::vec3 &center, const glm::vec3 &scale);
};
