#include "raylib.h"
#include <stdlib.h>

#define MAP_PIECES 16

int main(void) {
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Setup
    //SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1600, 900, "Test thingy");
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Variables
    int ChunkX;
    int ChunkZ;
    const int chunkHeight = 128;
    const int mapSize = 2048;
    const int chunkSize = mapSize / MAP_PIECES;
    const float mapHeightScale = 0.6f;
    Image terrainHeightmap[MAP_PIECES][MAP_PIECES];             
    Texture2D terrainTexture[MAP_PIECES][MAP_PIECES];           
    Mesh terrainMesh[MAP_PIECES][MAP_PIECES];                   
    Model terrainModel[MAP_PIECES][MAP_PIECES];                 
    Vector3 terrainPosition[MAP_PIECES][MAP_PIECES];            
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Terrain Generation & Camera
    Camera camera = {{mapSize / 2, 100.0f, mapSize / 2}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 90.0f, 0};
    SetCameraMode(camera, CAMERA_FIRST_PERSON);
    SetTargetFPS(999);

    for (ChunkX = 0; ChunkX < MAP_PIECES; ChunkX++) {
        for (ChunkZ = 0; ChunkZ < MAP_PIECES; ChunkZ++) {
            terrainHeightmap[ChunkX][ChunkZ] = GenImagePerlinNoise(chunkSize, chunkSize, ChunkX * chunkSize - ChunkX, ChunkZ * chunkSize - ChunkZ, mapHeightScale);
            terrainTexture[ChunkX][ChunkZ] = LoadTextureFromImage(terrainHeightmap[ChunkX][ChunkZ]);
            
            terrainMesh[ChunkX][ChunkZ] = GenMeshHeightmap(terrainHeightmap[ChunkX][ChunkZ], (Vector3){chunkSize, chunkHeight, chunkSize});
            terrainModel[ChunkX][ChunkZ] = LoadModelFromMesh(terrainMesh[ChunkX][ChunkZ]);
            terrainModel[ChunkX][ChunkZ].materials[0].maps[MAP_DIFFUSE].texture = terrainTexture[ChunkX][ChunkZ]; 
            
            terrainPosition[ChunkX][ChunkZ] = (Vector3){((chunkSize * ChunkX) - ChunkX), 0.0f, ((chunkSize * ChunkZ) - ChunkZ)};
            UnloadImage(terrainHeightmap[ChunkX][ChunkZ]);
       } 
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Main game loop
    while (!WindowShouldClose()) {
        Vector3 oldCamPos = camera.position;
        UpdateCamera(&camera);
        
        Vector2 playerPos = { camera.position.x, camera.position.z };
        float playerRadius = 0.1f;

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

            for (ChunkX = 0; ChunkX < MAP_PIECES; ChunkX++) {
                for (ChunkZ = 0; ChunkZ < MAP_PIECES; ChunkZ++) {
                    DrawModel(terrainModel[ChunkX][ChunkZ], terrainPosition[ChunkX][ChunkZ], 1.0f, RED);
                }
            }
            
            DrawPlane((Vector3){(mapSize / 2), 20.0f, (mapSize / 2)},(Vector2){mapSize, mapSize}, (Color){ 0, 121, 241, 100 });
            
            EndMode3D();
            DrawFPS(10, 10);
        EndDrawing();
    }

    for (ChunkX = 0; ChunkX < MAP_PIECES; ChunkX++) {
        for (ChunkZ = 0; ChunkZ < MAP_PIECES; ChunkZ++) {
            UnloadTexture(terrainTexture[ChunkX][ChunkZ]);
            UnloadModel(terrainModel[ChunkX][ChunkZ]);
        }
    }
    
    CloseWindow();

    return 0;
}
