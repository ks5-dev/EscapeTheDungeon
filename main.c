#include "raylib.h"
#include "raymath.h"
#include "stdio.h"

#define SCREEN_WIDTH 47*24
#define SCREEN_HEIGHT 47*18

#define PLAYER_AREA_WIDTH 114 // 14 + 50 * 2
#define PLAYER_AREA_HEIGHT 124 // 24 + 50 * 2 
#define CYCLOPS_AREA 152 // 32 + 60 * 2

#define mapRow 18
#define mapColumn 24

typedef enum GameScreen { GAMEPLAY, DEATH_SCREEN } GameScreen;

GameScreen screen = DEATH_SCREEN;

int MatrixMap[18][24] = {
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  0,  0,  1,  1,  1,  1,  1, 1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1},
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
    {1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0},
    {1,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  0,  1,  1,  1,  0},
};

typedef struct Player
{
    Texture2D playerSprite;
    Vector2 position;
    float speed;
    float sprintSpeed;
    enum {low, high} noise;
    Rectangle area;
    Rectangle rect;
} Player;

typedef struct Enemy
{
    Texture2D enemySprite;
    float speed;
    bool trigger;
    Vector2 position;
    Rectangle area;
} Enemy;


typedef struct Item
{
    Texture2D itemSprite;
    Vector2 position;
    enum {gunpowder_chest, key_chest} itemType;
} Item;

typedef struct Inventory
{
    Texture2D InventorySprite;
} Inventory;


void updatePlayer(Player *player, float delta);
void updateCyclop(Enemy *cyclops, Player *player, float delta, Vector2 *spawningPoints);

int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Save The Prince");
    InitAudioDevice();
    
    Texture2D dungeon_tile = LoadTexture("asset/dungeon_tiles.png");
    Texture2D princess = LoadTexture("asset/princess.png");
    Texture2D cyclop = LoadTexture("asset/cyclop.png");

    Music dungeon_sound = LoadMusicStream("asset/dungeon_ambient_1.ogg");
    Music chasing_scene = LoadMusicStream("goblin_breathing.wav");
    PlayMusicStream(dungeon_sound);

    Player player = {0};
    player.playerSprite = princess;
    player.position = (Vector2) { 100, 100};
    player.speed = 50.0;
    player.sprintSpeed = 20.0;
    player.noise = 0;
    player.area = (Rectangle){
        player.position.x - PLAYER_AREA_WIDTH + 50 + 14,
        player.position.y - PLAYER_AREA_HEIGHT +50 + 24,
        PLAYER_AREA_WIDTH,
        PLAYER_AREA_HEIGHT
    };

    Item chest1 = { 0 };
    chest1.itemSprite = LoadTexture("asset/chestYellow.png");
    chest1.position = (Vector2){ 400, 400};
    chest1.itemType = key_chest;

    Vector2 spawningPoint[] = {
        {300, 300},
        {700, 700}
    };

    Enemy cyclops[] = {
        {cyclop, 20.0, false,
        (Vector2){300,300}, 
        (Rectangle){
            300 - CYCLOPS_AREA + 60 + 32,
            300 - CYCLOPS_AREA +60 + 32,
            CYCLOPS_AREA,
            CYCLOPS_AREA
        } },
        {cyclop, 20.0, false,
        (Vector2){700,700}, 
        (Rectangle){
            300 - CYCLOPS_AREA + 60 + 32,
            300 - CYCLOPS_AREA + 60 + 32,
            CYCLOPS_AREA,
            CYCLOPS_AREA
        } },
    };


    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        switch (screen)
        {
        case GAMEPLAY:
            

            updatePlayer(&player, deltaTime);
            updateCyclop(cyclops, &player, deltaTime, spawningPoint);
            UpdateMusicStream(dungeon_sound);
            /*
            for(int i = 0; i < 2; i++){
                if (CheckC(cyclops[i].enemySprite.rect, player.playerSprite.rect)){
                    screen = DEATH_SCREEN;
                }
            }
            */

            break;
        

        case DEATH_SCREEN:
            break;
        default:
            break;
        }

        BeginDrawing();
            switch (screen)
            {
            case GAMEPLAY:
                ClearBackground(RAYWHITE);
                
                for(int i = 0 ; i < mapRow; i++){
                    for(int j = 0; j < mapColumn; j++){
                        if (MatrixMap[i][j] == 1 ){
                            DrawTexture(dungeon_tile, 47*j, 47*i, WHITE);
                        }
                    }
                }
                for(int i =0; i < 2; i++){
                    DrawTexture(cyclops[i].enemySprite, cyclops[i].position.x, cyclops[i].position.y, WHITE);
                    DrawRectangleLinesEx(cyclops[i].area, 5.0, RED);
                }
                DrawTexture(player.playerSprite, player.position.x, player.position.y, WHITE);
                DrawRectangleLinesEx(player.area, 5.0, RED);
                DrawTexture(chest1.itemSprite, chest1.position.x, chest1.position.y, WHITE);
                break;
            case DEATH_SCREEN:
                ClearBackground(BLACK);
                DrawText("YOU DIED", 500, 500, 30 ,RED);
                break;
            default:
                break;
            }
                

        EndDrawing();
    }
    
    UnloadTexture(dungeon_tile);
    UnloadTexture(player.playerSprite);
    UnloadTexture(chest1.itemSprite);
    UnloadMusicStream(dungeon_sound);
    CloseAudioDevice();

    CloseWindow();

    return 0;
}

void updatePlayer(Player *player, float delta){
    if (IsKeyDown(KEY_A)){
        if(IsKeyDown(KEY_LEFT_SHIFT)){
            player->position.x -= player->sprintSpeed * delta;
            player->noise = high;
        }else{
            player->position.x -= player->speed * delta;
            player->noise = low;
        }
    }
    if (IsKeyDown(KEY_D)){
        if(IsKeyDown(KEY_LEFT_SHIFT)){
            player->position.x += player->sprintSpeed * delta;
            player->noise = high;
        }else{
            player->position.x += player->speed * delta;
            player->noise = low;
        }
    }
    if (IsKeyDown(KEY_W)){
        if(IsKeyDown(KEY_LEFT_SHIFT)){
            player->position.y -= player->sprintSpeed * delta;
            player->noise = high;
        }else{
            player->position.y -= player->speed * delta;
            player->noise = low;
        }
    }
    if (IsKeyDown(KEY_S)){
        if(IsKeyDown(KEY_LEFT_SHIFT)){
            player->position.y += player->sprintSpeed * delta;
            player->noise = high;
        }else{
            player->position.y += player->speed * delta;
            player->noise = low;
        }
    }
    player->area = (Rectangle){
        player->position.x - PLAYER_AREA_WIDTH + 50 + 14,
        player->position.y - PLAYER_AREA_HEIGHT +50 + 24,
        PLAYER_AREA_WIDTH,
        PLAYER_AREA_HEIGHT
    };
}

void updateCyclop(Enemy *cyclops, Player *player, float delta, Vector2 *spawningPoints ){
    for(int i = 0; i < 2; i++){
        if(CheckCollisionRecs(cyclops[i].area, player->area) && Vector2Distance(spawningPoints[i], cyclops[i].position) <= 400.0){
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
    }
}