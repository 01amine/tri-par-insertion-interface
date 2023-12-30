#include "raylib.h"
void main(){
    InitWindow(800,800,"Project");
    IsWindowResized();
    MaximizeWindow();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText("IDK", 90, 90, 20, BLACK);
        DrawEllipseLines(130, 100, 100, 70, BLACK);
        EndDrawing();
        
    }
    CloseWindow();
}
