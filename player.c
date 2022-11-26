#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include "stdlib.h"

#define SCREEN_WIDTH 47*24
#define SCREEN_HEIGHT 47*18

#define PLAYER_AREA_WIDTH 114 // 14 + 50 * 2
#define PLAYER_AREA_HEIGHT 124 // 24 + 50 * 2 
#define HIGH_NOISE_PLAYER_AREA_WIDTH 174 // 14 + 80 * 2
#define HIGH_NOISE_PLAYER_AREA_HEIGHT 184 // 24 + 80 * 2 
#define CYCLOPS_AREA 152 // 32 + 60 * 2

#define NUMCHEST 4

#define BLUE_GRAY (Color){24, 35, 35, 1}
#define BLUE_GRAY_2 (Color){48, 60, 60, 1}

#define DARK (Color){ 36, 39, 37, 100}
#define DARKER (Color){21, 21, 21, 100}

#define mapRow 15
#define mapColumn 24

typedef struct Player
{
    Texture2D playerSprite;
    Vector2 position;
    float speed;
    float sprintSpeed;
    enum {low, high} noise;
    Rectangle area;
    Rectangle rect;
    bool hasKey;
    bool hasTorch;
    bool hasBoot;
    bool hasPoison;
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
    enum {key, torch, magic_boot, poison} itemType;
    bool beenOpened;
    Rectangle rect;
} Item;

typedef struct Inventory
{
    Texture2D inventorySprite;
    bool occupied;
} Inventory;

void lightning(int MatrixMap[15][24], int i, int j){
    for(int x = -2; x < 3; x++){
        for(int y = -2; y < 3; y++){
            if(i + x >= 0 && j + y >= 0 && ( MatrixMap[i+x][j+y] ==0 || MatrixMap[i+x][j+y] == 1) ){
                if( abs(x) == 2 || abs(y) == 2 ){
                    MatrixMap[i+x][j+y] = 1;
                }
                else if( abs(x) == 1 || abs(y) == 1 ){
                    MatrixMap[i+x][j+y] = 0;
                }
                else if(x == 0 & y == 0 && MatrixMap[i][j] != 3){
                    MatrixMap[i][j] = 0;
                }
            }
            else if(i + x >= 0 && j + y >= 0 ){
                if( (x == -2 || y == -2 || x == 2 || y == 2) && MatrixMap[i+x][j+y] == 3){
                    MatrixMap[i+x][j+y] = 2;
                }
                else if( (x == -1 || y == -1 || x == 1|| y == 1) && !(x == -2 || y == -2 || x == 2 || y == 2) && MatrixMap[i+x][j+y] == 2 ){
                    MatrixMap[i+x][j+y] = 3;
                }
            }
        }
    }
}

void lightningTorch(int MatrixMap[15][24], int i, int j){
    for(int x = -3; x < 4; x++){
        for(int y = -3; y < 4; y++){
            if(i + x >= 0 && j + y >= 0 && ( MatrixMap[i+x][j+y] ==0 || MatrixMap[i+x][j+y] == 1)  ){
                if(abs(x) == 3 || abs(y) == 3){
                    MatrixMap[i+x][j+y] = 1;
                }
                else if ( abs(x) == 2 || abs(x) == 1 || abs(y) == 1 || abs(y)==2 ){
                    MatrixMap[i+x][j+y] = 0;
                }
                else if(x == 0 & y == 0 && MatrixMap[i][j] != 3){
                    MatrixMap[i][j] = 0;
                }
            }
            else if(i + x >= 0 && j + y >= 0 && ( MatrixMap[i+x][j+y] == 3 || MatrixMap[i+x][j+y] == 2) ){
                if( (abs(x) == 3 || abs(y) == 3) && MatrixMap[i+x][j+y] == 3){
                    MatrixMap[i+x][j+y] = 2;
                }
                else if( (abs(x) == 2 || abs(x) == 1 || abs(y) == 1 || abs(y)==2) && !(abs(x) == 3 || abs(y) == 3) && MatrixMap[i+x][j+y] == 2 ){
                    MatrixMap[i+x][j+y] = 3;
                }

            }
        }
    }
}

void updatePlayer(Player *player, Inventory *inventory, Item *item, Texture2D chestOpened, float delta, int MatrixMap[15][24]){
    if (IsKeyDown(KEY_A)){
        if(IsKeyDown(KEY_LEFT_SHIFT) && !player->hasBoot ){
            player->position.x -= player->sprintSpeed * delta;
            player->noise = high;
        }else{
            player->position.x -= player->speed * delta;
            player->noise = low;
        }
    }
    if (IsKeyDown(KEY_D)){
        if(IsKeyDown(KEY_LEFT_SHIFT) && !player->hasBoot ){
            player->position.x += player->sprintSpeed * delta;
            player->noise = high;
        }else{
            player->position.x += player->speed * delta;
            player->noise = low;
        }
    }
    if (IsKeyDown(KEY_W)){
        if(IsKeyDown(KEY_LEFT_SHIFT) && !player->hasBoot ){
            player->position.y -= player->sprintSpeed * delta;
            player->noise = high;
        }else{
            player->position.y -= player->speed * delta;
            player->noise = low;
        }
    }
    if (IsKeyDown(KEY_S)){
        if(IsKeyDown(KEY_LEFT_SHIFT) && !player->hasBoot){
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
    if (player->hasTorch){
        lightningTorch(MatrixMap, i, j);
    }
    else{
        lightning(MatrixMap, i, j);
    }
    //Chest & items
    for(int i = 0; i < NUMCHEST; i++){
        if (CheckCollisionRecs(player->rect, item[i].rect)){
            item[i].itemSprite = chestOpened;
            switch (item[i].itemType)
            {
            case key:
                player -> hasKey = true;
                inventory[i].inventorySprite = LoadTexture("asset/inv_key.png");
                break;
            case torch:
                player -> hasTorch = true;
                inventory[i].inventorySprite = LoadTexture("asset/inv_torch.png");
                break;
            case magic_boot:
                player -> hasBoot = true;
                inventory[i].inventorySprite = LoadTexture("asset/inv_magic_boot.png");
                break;
            case poison:
                player -> hasPoison = true;
                inventory[i].inventorySprite = LoadTexture("asset/inv_potion.png");
                break;
            default:
                break;
            }
        }
    }

    
    // Player Boundaries
    if(player->position.y >= SCREEN_HEIGHT - (47.0 * 3.5) ){
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