#include "raylib.h"
#include "raymath.h"
#include "stdio.h"

#define SCREEN_WIDTH 47*24
#define SCREEN_HEIGHT 47*18

#define PLAYER_AREA_WIDTH 114 // 14 + 50 * 2
#define PLAYER_AREA_HEIGHT 124 // 24 + 50 * 2 
#define HIGH_NOISE_PLAYER_AREA_WIDTH 174 // 14 + 80 * 2
#define HIGH_NOISE_PLAYER_AREA_HEIGHT 184 // 24 + 80 * 2 
#define CYCLOPS_AREA 152 // 32 + 60 * 2

#define DARK (Color){ 36, 39, 37, 100}
#define DARKER (Color){21, 21, 21, 100}

#define mapRow 18
#define mapColumn 24

typedef enum GameScreen { GAMEPLAY, DEATH_SCREEN } GameScreen;

GameScreen screen = GAMEPLAY;

int MatrixMap[18][24] = {
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

    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  3,  3,  1,  1,  3},
    {2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  3,  1,  1,  3,  3,  1,  1,  1,  3},
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
    Rectangle rect;
} Enemy;


typedef struct Item
{
    Texture2D itemSprite;
    Vector2 position;
    enum {gunpowder_chest, key_chest} itemType;
    bool beenOpened;
} Item;

typedef struct Inventory
{
    Texture2D InventorySprite;
} Inventory;


void updatePlayer(Player *player, float delta, int MatrixMap[18][24]);
void updateCyclop(Enemy *cyclops, Player *player, float delta, Vector2 *spawningPoints);

int main(){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Save The Prince");
    InitAudioDevice();
    
    Texture2D dungeon_tile = LoadTexture("asset/dungeon_tiles.png");
    Texture2D princess = LoadTexture("asset/princess.png");
    Texture2D cyclop = LoadTexture("asset/cyclop.png");

    Texture2D skull = LoadTexture("asset/skull.png");

    Music dungeon_sound = LoadMusicStream("asset/dungeon_ambient_1.ogg");
    Music chasing_scene = LoadMusicStream("goblin_breathing.wav");
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

    Item chest1 = { 0 };
    chest1.itemSprite = LoadTexture("asset/chestYellow.png");
    chest1.position = (Vector2){ 400, 400};
    chest1.itemType = key_chest;
    chest1.beenOpened = false;

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
        },
        (Rectangle){
            300,
            300,
            32,
            32
        } },
        {cyclop, 20.0, false,
        (Vector2){700,700}, 
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
            

            updatePlayer(&player, deltaTime, MatrixMap);
            updateCyclop(cyclops, &player, deltaTime, spawningPoint);
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
                ClearBackground(BLACK);
                
                for(int i = 0 ; i < mapRow; i++){
                    for(int j = 0; j < mapColumn; j++){
                        if (MatrixMap[i][j] == 0 ){
                            DrawTexture(dungeon_tile, 47*j, 47*i, WHITE);
                        }
                        else if (MatrixMap[i][j] == 2){
                            DrawRectangleV( (Vector2){47 * j, 47 * i}, (Vector2){47.0, 47.0}, RAYWHITE );
                        }
                        else if (MatrixMap[i][j] == 3){
                            DrawRectangleV( (Vector2){47 * j, 47 * i}, (Vector2){47.0, 47.0}, DARKER );
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
                DrawTexture(chest1.itemSprite, chest1.position.x, chest1.position.y, WHITE);

                for(int i = 0 ; i < mapRow; i++){
                    for(int j = 0; j < mapColumn; j++){
                        if (MatrixMap[i][j] == 1 ){
                            DrawRectangleV( (Vector2){47 * j, 47 * i}, (Vector2){47.0, 47.0}, BLACK );
                    }
                }
                }
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
    UnloadTexture(chest1.itemSprite);
    UnloadMusicStream(dungeon_sound);
    CloseAudioDevice();

    CloseWindow();

    return 0;
}

void updatePlayer(Player *player, float delta, int MatrixMap[18][24]){
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
    if (player->noise == low){
        player->area = (Rectangle){
            player->position.x - PLAYER_AREA_WIDTH + 50 + 14,
            player->position.y - PLAYER_AREA_HEIGHT + 50 + 24,
            PLAYER_AREA_WIDTH,
            PLAYER_AREA_HEIGHT
        };
    }
    else{
        player->area = (Rectangle){
            player->position.x - HIGH_NOISE_PLAYER_AREA_WIDTH + 80 + 14,
            player->position.y - HIGH_NOISE_PLAYER_AREA_HEIGHT +80 + 24,
            HIGH_NOISE_PLAYER_AREA_WIDTH,
            HIGH_NOISE_PLAYER_AREA_HEIGHT
        };
    };
    player->rect = (Rectangle){
        player->position.x,
        player->position.y,
        14,
        24
    };
    
    float convertedCoordinateX = player->position.x / 47.0;
    float convertedCoordinateY = player->position.y / 47.0;
    int j = (int)convertedCoordinateX;
    int i = (int)convertedCoordinateY;

    if (MatrixMap[i][j] == 3){

        if(player->position.x >= j * 47.0 && player->position.x <= (j+1) * 47.0 ){
            if ( IsKeyDown(KEY_A) || IsKeyPressed(KEY_A) ){
                player->position.x += 1;
            }
            if ( IsKeyDown(KEY_D) || IsKeyPressed(KEY_D) ){
                player->position.x -= 1;
            }
        }
        if(player->position.y + 14 >= i * 47.0 && player->position.y <= (i+1) * 47.0){
            if ( IsKeyDown(KEY_W) || IsKeyPressed(KEY_W) ){
                player->position.y += 1;
            }
            if ( IsKeyDown(KEY_S) || IsKeyPressed(KEY_S) ){
                player->position.y -= 1;
            }
        }
    }

            
    for(int x = -2; x < 3; x++){
        for(int y = -2; y < 3; y++){
            if(i + x >= 0 && j + y >= 0 && MatrixMap[i+x][j+y] != 3){
                if( (x == -2 || y == -2 || x == 2 || y == 2) && ( MatrixMap[i+x][j+y] != 2 && MatrixMap[i+x][j+y] != 3) ){
                    MatrixMap[i+x][j+y] = 1;
                }
                else if( (x == -1 || y == -1 || x == 1|| y == 1) && (MatrixMap[i+x][j+y] != 2 && MatrixMap[i+x][j+y] != 3) ){
                    MatrixMap[i+x][j+y] = 0;
                }
                else if(x == 0 & y == 0 && MatrixMap[i][j] != 3){
                    MatrixMap[i][j] = 0;
                }
            }
        }
    }

    // Player Boundaries
    if(player->position.y >= SCREEN_HEIGHT - (47.0 * 3.0) ){
        player->position.y -= 1;
    }

    if(player->position.y <= 0 ){
        player->position.y += 1;
    }

    if(player->position.x >= SCREEN_WIDTH ){
        player->position.x -= 1;
    }

    if(player->position.x <= 0 ){
        player->position.x += 1;
    }

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
        cyclops[i].rect = (Rectangle){
            cyclops[i].position.x,
            cyclops[i].position.y,
            32,
            32
        };
    }
}