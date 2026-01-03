#include "lab_m1/Tema2/WorldGen/BlockWorld.h"
#include <iostream>

using namespace std;

void BlockWorld::WorldGeneration::computeMinx(int start, int end, int idx) {
    int h = self.special_coords[{idx, end}];

    for (int i = start; i <= end; i++) {
        self.special_coords[{idx, i}] = h;
    }
}

void BlockWorld::WorldGeneration::computeMinz(int start, int end, int idx) {
    int h = self.special_coords[{end, idx}];

    for (int i = start; i <= end; i++) {
        self.special_coords[{i, idx}] = h;
    }
}

void BlockWorld::WorldGeneration::flattenTx(int start, int end, int idx) {
    int h = self.special_coords[{idx, start}];

    for (int i = start; i <= end; i++) {
        self.blocks[idx][h][i] = BlockType::Air;
        self.heights[idx][i] = h;
    }
}

void BlockWorld::WorldGeneration::flattenTz(int start, int end, int idx) {
    int h = self.special_coords[{start, idx}];

    for (int i = start; i <= end; i++) {
        self.blocks[i][h][idx] = BlockType::Air;
        self.heights[i][idx] = h;
    }
}

void BlockWorld::WorldGeneration::addRailsToStationx(int start, int end,
                                                     int offx, int offz,
                                                     int idx) {
    for (int i = start; i <= end; i++) {
        self.train[idx][i] = 1;
    }
}

void BlockWorld::WorldGeneration::addRailsToStationz(int start, int end,
                                                     int offx, int offz,
                                                     int idx) {
    for (int i = start; i <= end; i++) {
        self.train[i][idx] = 1;
    }
}

void BlockWorld::WorldGeneration::addStationRails(int x, int z, int offx,
                                                  int offz) {
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int i = 0; i < offx; i++) {
            for (int j = 0; j < offz; j++) {
                if (i == 0 || i == offx - 1 || j == 0 || j == offz - 1) {
                    self.train[x + i][z + j] = 3;
                }
            }
        }
    }
}

void BlockWorld::WorldGeneration::flattenTerrain() {
    for (auto &v : self.temp) {
        int x = v.x, z = v.z;

        int dx[5] = {-1, 1, 0, 0, 0};
        int dz[5] = {0, 0, -1, 1, 0};
        int minim = self.heights[x][z];

        for (int i = 0; i < 5; i++) {
            int x1 = x + dx[i];
            int z1 = z + dz[i];

            int h = self.heights[x1][z1];
            minim = std::min(minim, h);
        }

        self.special_coords[{x, z}] = minim;

        for (int i = 0; i < 5; i++) {
            int x1 = x + dx[i];
            int z1 = z + dz[i];

            if (minim < self.heights[x1][z1]) {
                for (int y = minim; y <= self.heights[x1][z1]; y++) {
                    self.blocks[x1][y][z1] = BlockType::Air;
                }
            }
            self.heights[x1][z1] = minim;
        }
    }
}

void BlockWorld::WorldGeneration::flattenTerrainStation(int x, int z, int h) {
    int offx = 10, offz = 10;
    for (int y = 0; y < h; y++) {
        for (int i = 0; i < offx; i++) {
            for (int j = 0; j < offz; j++) {
                self.blocks[x + i][y][z + j] = BlockType::Grass;
                self.heights[x + i][z + j] = h;
            }
        }
    }

    for (int y = h; y < WORLD_HEIGHT; y++) {
        for (int i = 0; i < offx; i++) {
            for (int j = 0; j < offz; j++) {
                self.blocks[x + i][y][z + j] = BlockType::Air;
            }
        }
    }
}
