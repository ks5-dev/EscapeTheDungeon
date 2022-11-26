#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
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

typedef enum GameScreen { GAMEPLAY, DEATH_SCREEN, WIN_SCREEN } GameScreen;

GameScreen screen = GAMEPLAY;

int MatrixMap[15][24] = {
    {1,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  2,  2,  1,  1,  1,  2,  2,  1,  1,  1,  1,  1, 4},
    {1,  2,  2,  2,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  1,  1,  1,  1,  1,  2},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},

    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  1,  1,  1,  1,  1},

    
};

/*
{1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  3,  3,  1,  1,  3},
    {2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  3,  1,  1,  3,  3,  1,  1,  1,  3},
*/

const char *dialouge[10];
    

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


void updatePlayer(Player *player, Inventory *inventory, Item *item, Texture2D chestOpened,float delta, int MatrixMap[18][24]);
void updateCyclop(Enemy *cyclops, Player *player, float delta, Vector2 spawningPointEnemy[]);

int main(){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Save The Prince");
    InitAudioDevice();

    dialouge[0] = "You are finally awake!";
    dialouge[1] = "Confusing? Let me tell you what happened.";
    dialouge[2] = "You are captured by these one-eye goblin \n when they suddenly attack the kingdom";
    dialouge[3] = "Many were left dead, including me, I was \n killed and turned into a vengeful spirit";
    dialouge[4] = "I want to help you to get out, so \n you can seal this dungeon forever...";
    dialouge[5] = "They hid the gate to get out, \nfind it along with the key to open";
    dialouge[6] = "There are hidden chests that may grant you \n items as well";
    dialouge[7] = "Now find your way out, I hope you shall \n be the last one I ever helped";
    
    Texture2D dungeon_tile = LoadTexture("asset/dungeon_tiles.png");
    Texture2D princess = LoadTexture("asset/princess.png");
    Texture2D cyclop = LoadTexture("asset/cyclop.png");
    Texture2D wall = LoadTexture("asset/wall.png");
    Texture2D empty_inventory = LoadTexture("asset/inventory.png");
    Texture2D chest = LoadTexture("asset/chest.png");
    Texture2D openedChest = LoadTexture("asset/chest_opened.png");
    Texture2D gate = LoadTexture("asset/gate.png");
    
    Texture ghost = LoadTexture("asset/ghost.png");
    Texture2D skull = LoadTexture("asset/skull.png");

    Music dungeon_sound = LoadMusicStream("asset/dungeon_ambient_1.ogg");

    
    Vector2 spawningPointEnemy[] = {
        {300.0, 300.0},
        {800.0, 600.0}
    };

    
    PlayMusicStream(dungeon_sound);

    Player player = {0};
    player.playerSprite = princess;
    player.position = (Vector2) { 100, 100};
    player.speed = 100.0;
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
    player.hasTorch = false;
    player.hasBoot = false;
    player.hasPoison = false;

    Inventory inventory[] = {
        {empty_inventory, false},
        {empty_inventory, false},
        {empty_inventory, false},
        {empty_inventory, false}
    };

    Item item[] = {
        {chest, torch, false, 
        (Rectangle){400.0, 400.0, 24, 21} },
        {chest, poison, false, 
        (Rectangle){0.0, 500.0, 24, 21} },
        {chest, magic_boot, false, 
        (Rectangle){1000.0, 100.0, 24, 21} },
        {chest, key, false, 
        (Rectangle){1100.0, 500.0, 24, 21} },
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
        (Vector2){800,600}, 
        (Rectangle){
            800 - CYCLOPS_AREA + 60 + 32,
            600 - CYCLOPS_AREA + 60 + 32,
            CYCLOPS_AREA,
            CYCLOPS_AREA
        },
        (Rectangle){
            800,
            600,
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
                if(player.hasPoison){
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                        Vector2 mouse_pos = GetMousePosition();
                        if (CheckCollisionPointRec(mouse_pos, cyclops[i].rect)){
                            cyclops[i].speed = 0.0;
                        }
                    }
                    else if(IsMouseButtonUp(MOUSE_BUTTON_LEFT)){
                        cyclops[i].speed = 30.0;
                    }                                
                }

                if (CheckCollisionRecs(cyclops[i].rect, player.rect)){
                    screen = DEATH_SCREEN;
                }
                if (CheckCollisionRecs(player.rect, (Rectangle){47.0*23, 0, 47.0, 47.0} ) && player.hasKey){
                    screen = WIN_SCREEN;
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
                        if (MatrixMap[i][j] == 1 ){
                            DrawRectangleV( (Vector2){47 * j, 47 * i}, (Vector2){47.0, 47.0}, DARK );
                    }
                        else if (MatrixMap[i][j] == 2){
                            DrawRectangleV( (Vector2){47 * j, 47 * i}, (Vector2){47.0, 47.0}, BLACK );
                        }
                        else if (MatrixMap[i][j] == 3){
                            DrawTexture(wall, 47*j, 47*i, WHITE);
                        }
                        else if (MatrixMap[i][j] == 4){
                            DrawTexture(gate, 47*j, 47*i, WHITE);
                        }
                    }
                }

                for(int i = 0; i < 2; i++){
                    DrawTexture(cyclops[i].enemySprite, cyclops[i].position.x, cyclops[i].position.y, WHITE);
                    DrawRectangleLinesEx(cyclops[i].area, 5.0, RED);
                    // Decomment the line above to see the range of the Cyclops
                }
                
                DrawTexture(player.playerSprite, player.position.x, player.position.y, WHITE);
                DrawRectangleLinesEx(player.area, 5.0, RED);
                // Decomment the line above to see the range of the PLayer
                
                //Chests
                for(int i = 0; i < 4; i++){
                    DrawTexture(item[i].itemSprite, item[i].rect.x, item[i].rect.y, WHITE);
                }

            // Dialogue & inventory screen
                for(int i = 0; i < 4; i++){
                    DrawTexture(inventory[i].inventorySprite, 64 * (i+1), 753, WHITE);
                }
                DrawTexture(ghost, 1000, 720, WHITE);
                // Dialouge box
                DrawRectangleRoundedLines( (Rectangle){500, 715, 500, 120}, 0.5, 3, 5.0, BLACK);
                DrawRectangleRounded( (Rectangle){500, 715, 500, 120}, 0.5, 3 , WHITE);
                DrawText(dialouge[7], 515, 730, 20, BLACK);
                break;
            case DEATH_SCREEN:
                ClearBackground(BLACK);
                DrawText("YOU DIED", 475, 500, 30 ,RED);
                DrawText("Press [Enter] for another chance", 300, 550, 30 ,RED);
                DrawTexture(skull, 485, 300, WHITE);
                break;
            default:
                ClearBackground(WHITE);
                DrawText("YOU WIN", 475, 500, 30 ,BLUE);
                DrawText("Press [Enter] for another retry", 300, 550, 30 ,BLUE);
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