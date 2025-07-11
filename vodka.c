#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#define MAX_OBJECTS 100

typedef enum GameMode { MODE_2D, MODE_3D } GameMode;
typedef enum ObjectType { OBJECT_BLOCK, OBJECT_CAR, OBJECT_NPC, OBJECT_WATER } ObjectType;

typedef struct {
    ObjectType type;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
} GameObject;

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Vodka - Toolbox Game");
    SetTargetFPS(60);

    GameMode mode = MODE_3D;
    int selectedTool = OBJECT_BLOCK;
    GameObject objects[MAX_OBJECTS];
    int objectCount = 0;

    Camera3D camera = { 0 };
    float angle = 45.0f;
    float radius = 7.0f;
    float height = 3.0f;
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose()) {
        // Mode switching
        if (IsKeyPressed(KEY_TAB)) mode = (mode == MODE_2D) ? MODE_3D : MODE_2D;

        // Toolbox keys
        if (IsKeyPressed(KEY_ONE)) selectedTool = OBJECT_BLOCK;
        if (IsKeyPressed(KEY_TWO)) selectedTool = OBJECT_CAR;
        if (IsKeyPressed(KEY_THREE)) selectedTool = OBJECT_NPC;
        if (IsKeyPressed(KEY_FOUR)) selectedTool = OBJECT_WATER;

        // Add object
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mode == MODE_3D && objectCount < MAX_OBJECTS) {
            objects[objectCount++] = (GameObject){
                .type = selectedTool,
                .position = { 0, 0.5f, 0 },
                .rotation = { 0, 0, 0 },
                .scale = { 1, 1, 1 }
            };
        }

        // Camera controls
        if (IsKeyDown(KEY_A)) angle -= 1.5f;
        if (IsKeyDown(KEY_D)) angle += 1.5f;
        if (IsKeyDown(KEY_W)) radius -= 0.1f;
        if (IsKeyDown(KEY_S)) radius += 0.1f;

        float rad = DEG2RAD * angle;
        camera.position = (Vector3){ radius * cosf(rad), height, radius * sinf(rad) };
        camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };

        // Edit last object
        if (objectCount > 0) {
            GameObject *obj = &objects[objectCount - 1];
            if (IsKeyDown(KEY_RIGHT)) obj->position.x += 0.1f;
            if (IsKeyDown(KEY_LEFT))  obj->position.x -= 0.1f;
            if (IsKeyDown(KEY_UP))    obj->position.z -= 0.1f;
            if (IsKeyDown(KEY_DOWN))  obj->position.z += 0.1f;
            if (IsKeyDown(KEY_Q))     obj->rotation.y -= 2.0f;
            if (IsKeyDown(KEY_E))     obj->rotation.y += 2.0f;
            if (IsKeyDown(KEY_Z))     obj->scale = Vector3Scale(obj->scale, 0.95f);
            if (IsKeyDown(KEY_X))     obj->scale = Vector3Scale(obj->scale, 1.05f);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (mode == MODE_2D) {
            DrawText("Vodka - 2D Mode (TAB = 3D)", 10, 10, 20, DARKGRAY);
        } else {
            DrawText("Vodka - 3D Mode (TAB = 2D)", 10, 10, 20, DARKGRAY);
            DrawText("Tools: [1]=Block [2]=Car [3]=NPC [4]=Water", 10, 40, 20, GRAY);
            DrawText("Edit Last: Arrows=Move Q/E=Rotate Z/X=Scale", 10, 70, 20, GRAY);

            BeginMode3D(camera);
            DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 20.0f, 20.0f }, LIGHTGRAY);

            for (int i = 0; i < objectCount; i++) {
                GameObject *obj = &objects[i];
                Color color = GRAY;
                switch (obj->type) {
                    case OBJECT_BLOCK: color = GRAY; break;
                    case OBJECT_CAR: color = RED; break;
                    case OBJECT_NPC: color = PURPLE; break;
                    case OBJECT_WATER: color = BLUE; break;
                }

                rlPushMatrix();
                rlTranslatef(obj->position.x, obj->position.y, obj->position.z);
                rlRotatef(obj->rotation.y, 0, 1, 0);
                rlScalef(obj->scale.x, obj->scale.y, obj->scale.z);
                DrawCube((Vector3){ 0, 0.5f, 0 }, 1, 1, 1, color);
                rlPopMatrix();
            }

            EndMode3D();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

