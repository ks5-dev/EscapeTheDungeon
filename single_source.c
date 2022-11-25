#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include "string.h"
#include "player.h"

#define SCREEN_WIDTH 47*24
#define SCREEN_HEIGHT 47*18

#define PLAYER_AREA_WIDTH 114 // 14 + 50 * 2
#define PLAYER_AREA_HEIGHT 124 // 24 + 50 * 2 
#define HIGH_NOISE_PLAYER_AREA_WIDTH 174 // 14 + 80 * 2
#define HIGH_NOISE_PLAYER_AREA_HEIGHT 184 // 24 + 80 * 2 
#define CYCLOPS_AREA 152 // 32 + 60 * 2

#define NUMCHEST 1

#define BLUE_GRAY (Color){24, 35, 35, 1}
#define BLUE_GRAY_2 (Color){48, 60, 60, 1}

#define DARK (Color){ 36, 39, 37, 100}
#define DARKER (Color){21, 21, 21, 100}

#define mapRow 15
#define mapColumn 24

typedef enum GameScreen { GAMEPLAY, DEATH_SCREEN } GameScreen;

GameScreen screen = GAMEPLAY;

int MatrixMap[15][24] = {
    {1,  3,  3,  3,  1,  1,  1,  1,  1,  1,  1,  3,  3,  1,  1,  1,  3,  3,  1,  1,  1,  1,  1, 1},
    {1,  3,  3,  3,  1,  1,  1,  3,  1,  1,  1,  1,  1,  1,  1,  1,  3,  3,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  3,  3,  3,  1,  1,  1,  1,  1},
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

    
};

/*
{1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  3,  3,  1,  1,  3},
    {2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  3,  1,  1,  3,  3,  1,  1,  1,  3},
*/



void updateCyclop(Enemy *cyclops, Player *player, float delta, Vector2 spawningPointEnemy[]);

int main(){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Save The Prince");
    InitAudioDevice();
    
    Texture2D dungeon_tile = LoadTexture("asset/dungeon_tiles.png");
    Texture2D princess = LoadTexture("asset/princess.png");
    Texture2D cyclop = LoadTexture("asset/cyclop.png");
    Texture2D wall = LoadTexture("asset/wall.png");
    Texture2D empty_inventory = LoadTexture("asset/inventory.png");
    Texture2D chest = LoadTexture("asset/chest.png");
    Texture2D openedChest = LoadTexture("asset/chest_opened.png");
    
    Texture ghost = LoadTexture("asset/ghost.png");
    Texture2D skull = LoadTexture("asset/skull.png");

    Music dungeon_sound = LoadMusicStream("asset/dungeon_ambient_1.ogg");
    Music chasing_scene = LoadMusicStream("goblin_breathing.wav");

    Vector2 spawningPointEnemy[] = {
        {300.0, 300.0},
        {800.0, 650.0}
    };

    
    PlayMusicStream(dungeon_sound);

    Player player = {0};
    player.playerSprite = princess;
    player.position = (Vector2) { 100, 100};
    player.speed = 25.0;
    player.sprintSpeed = 50.0;
    player.noise = 0;
    player.area = (Rectangle){
        player.position.x - PLAYER_AREA_WIDTH + 50 + 14,
        player.position.y - PLAYER_AREA_HEIGHT +50 + 24,
        PLAYER_AREA_WIDTH,
        PLAYER_AREA_HEIGHT
    };
    player.rect = (Rectangle){
        player.position.x,
        player.position.y,
        14,
        24
    };
    player.hasKey = false;

    Inventory inventory[] = {
        {empty_inventory, false},
        {empty_inventory, false},
        {empty_inventory, false},
        {empty_inventory, false}
    };

    Item item[] = {
        {chest, key, false, 
        (Rectangle){400.0, 400.0, 24, 21} },
    };

    Enemy cyclops[] = {
        {cyclop, 20.0, false,
        (Vector2){300,300}, 
        (Rectangle){
            300 - CYCLOPS_AREA + 60 + 32,
            300 - CYCLOPS_AREA +60 + 32,
            CYCLOPS_AREA,
            CYCLOPS_AREA
        },
        (Rectangle){
            300,
            300,
            32,
            32
        } },
        {cyclop, 20.0, false,
        (Vector2){800,650}, 
        (Rectangle){
            700 - CYCLOPS_AREA + 60 + 32,
            700 - CYCLOPS_AREA + 60 + 32,
            CYCLOPS_AREA,
            CYCLOPS_AREA
        },
        (Rectangle){
            700,
            700,
            32,
            32
        } },
    };


    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        switch (screen)
        {
        case GAMEPLAY:
            
            updatePlayer(&player, inventory, item, openedChest, deltaTime, MatrixMap);
            updateCyclop(cyclops, &player, deltaTime, spawningPointEnemy);
            UpdateMusicStream(dungeon_sound);
            
            for(int i = 0; i < 2; i++){
                if (CheckCollisionRecs(cyclops[i].rect, player.rect)){
                    screen = DEATH_SCREEN;
                }
            }

            break;
        

        case DEATH_SCREEN:
            if (IsKeyPressed(KEY_ENTER)){
                screen = GAMEPLAY;
                for(int i = 0 ; i < mapRow; i++){
                    for(int j = 0; j < mapColumn; j++){
                        if (MatrixMap[i][j] != 2 && MatrixMap[i][j] != 3){
                            MatrixMap[i][j] = 1;
                        }
                    }
                }
                player.position = (Vector2) { 100, 100};
            }
            break;
        default:
            break;
        }

        BeginDrawing();
            switch (screen)
            {
            case GAMEPLAY:
                ClearBackground(BLUE_GRAY);
                
                for(int i = 0 ; i < mapRow; i++){
                    for(int j = 0; j < mapColumn; j++){
                        if (MatrixMap[i][j] == 0 ){
                            DrawTexture(dungeon_tile, 47*j, 47*i, WHITE);
                        }
                        else if (MatrixMap[i][j] == 2){
                            DrawRectangleV( (Vector2){47 * j, 47 * i}, (Vector2){47.0, 47.0}, RAYWHITE );
                        }
                        else if (MatrixMap[i][j] == 3){
                            DrawTexture(wall, 47*j, 47*i, WHITE);
                        }
                    }
                }

                for(int i = 0; i < 2; i++){
                    DrawTexture(cyclops[i].enemySprite, cyclops[i].position.x, cyclops[i].position.y, WHITE);
                    // DrawRectangleLinesEx(cyclops[i].area, 5.0, RED);
                    // Decomment the line above to see the range of the Cyclops
                }
                
                DrawTexture(player.playerSprite, player.position.x, player.position.y, WHITE);
                // DrawRectangleLinesEx(player.area, 5.0, RED);
                // Decomment the line above to see the range of the PLayer
                
                //Chests
                for(int i = 0; i < 1; i++){
                    DrawTexture(item[i].itemSprite, item[i].rect.x, item[i].rect.y, WHITE);
                }

                for(int i = 0 ; i < mapRow; i++){
                    for(int j = 0; j < mapColumn; j++){
                        if (MatrixMap[i][j] == 1 ){
                            DrawRectangleV( (Vector2){47 * j, 47 * i}, (Vector2){47.0, 47.0}, BLACK );
                    }
                }
            }

            // Dialogue & inventory screen
                for(int i = 0; i < 4; i++){
                    DrawTexture(inventory[i].inventorySprite, 64 * (i+1), 753, WHITE);
                }
                DrawTexture(ghost, 1000, 720, WHITE);
                // Dialouge box
                DrawRectangleRoundedLines( (Rectangle){500, 715, 500, 120}, 0.5, 3, 5.0, BLACK);
                DrawRectangleRounded( (Rectangle){500, 715, 500, 120}, 0.5, 3 , WHITE);
                break;
            case DEATH_SCREEN:
                ClearBackground(BLACK);
                DrawText("YOU DIED", 475, 500, 30 ,RED);
                DrawText("Press [Enter] for another chance", 300, 550, 30 ,RED);
                DrawTexture(skull, 485, 300, WHITE);
                break;
            default:
                break;
            }
                
        EndDrawing();
    }
    
    UnloadTexture(dungeon_tile);
    UnloadTexture(player.playerSprite);
    UnloadMusicStream(dungeon_sound);
    CloseAudioDevice();

    CloseWindow();

    return 0;
}

void updateCyclop(Enemy *cyclops, Player *player, float delta, Vector2 spawningPointEnemy[] ){
    for(int i = 0; i < 2; i++){
        if(CheckCollisionRecs(cyclops[i].area, player->area) && Vector2Distance(spawningPointEnemy[i], cyclops[i].position) <= 400.0){
            Vector2 marked = cyclops[i].position;
            Vector2 direction = Vector2Normalize(Vector2Subtract(player->position, cyclops[i].position));
            Vector2 scaled_direction = Vector2Scale(direction, cyclops[i].speed*delta);
            cyclops[i].position = Vector2Add(cyclops[i].position, scaled_direction);
        }
        cyclops[i].area = (Rectangle){
            cyclops[i].position.x - CYCLOPS_AREA + 60 + 32,
            cyclops[i].position.y - CYCLOPS_AREA + 60 + 32,
            CYCLOPS_AREA,
            CYCLOPS_AREA
        };
        cyclops[i].rect = (Rectangle){
            cyclops[i].position.x,
            cyclops[i].position.y,
            32,
            32
        };
    }
}