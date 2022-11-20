#include "raylib.h"

#define SCREEN_WIDTH 47*24
#define SCREEN_HEIGHT 47*18

#define mapRow 18
#define mapColumn 24

int MatrixMap[18][24] = {
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1, 1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},

    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},

    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1},

};

typedef struct Player
{
    Texture2D playerSprite;
    Vector2 position;
    float speed;
    float sprintSpeed;
    enum {low, high} noise;

} Player;

typedef struct Enemy
{
    Texture2D enemySprite;
    float speed;
    float sprintSpeed;
} Enemy;


int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Save The Prince");
    
    Texture2D dungeon_tile = LoadTexture("asset/dungeon_tiles.png");

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);

            for(int i = 0 ; i < mapRow; i++){
                for(int j = 0; j < mapColumn; j++){
                    if (MatrixMap[i][j] == 1 ){
                        DrawTexture(dungeon_tile, 47*j, 47*i, WHITE);
                    }
                }
            }

        EndDrawing();
    }
    
    UnloadTexture(dungeon_tile);

    CloseWindow();

    return 0;
}

void updatePlayer(Player *player){
    if (IsKeyDown(KEY_A)){
        player->position.x -= player->speed;
        player->noise = low;
    }
    if (IsKeyDown(KEY_D)){
        player->position.x += player->speed;
        player->noise = low;
    }
    if (IsKeyDown(KEY_W)){
        player->position.y -= player->speed;
        player->noise = low;
    }
    if (IsKeyDown(KEY_S)){
        player->position.y += player->speed;
        player->noise = low;
    }
}