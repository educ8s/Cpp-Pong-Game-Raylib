#include <raylib.h>

int cpu_score = 0;
int player_score = 0;

Color Grey = Color{63, 92, 108, 255};
Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

class Ball {
    public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x,y,radius, Yellow);
    }

    void update(){
        x += speed_x;
        y += speed_y;

        if(x + radius >= GetScreenWidth()){
            cpu_score += 1;
            reset_ball();
        }

        if(x - radius <= 0){
            player_score += 1;
            reset_ball();
        } 

        if(y + radius >= GetScreenHeight() || y - radius <= 0){
            speed_y *= -1;
        }
    }

    void reset_ball() {
        int speed_choices[2] = {-1,1}; 
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;
    }
};

class Paddle {
    public:
    float x, y;
    float width, height;
    int speed;

    void Draw(){
        //DrawRectangle(x, y, width, height, WHITE);
        DrawRectangleRounded(Rectangle{x,y,width,height}, 0.8f, 0, WHITE);
    }

    void update() {

        if (IsKeyDown(KEY_DOWN)){
            y += speed;
            if (y >= GetScreenHeight() - height){
                y = GetScreenHeight() - height;     
            }
        }

        if (IsKeyDown(KEY_UP)){
            y -= speed;
            if (y <= 0){
                y = 0;     
            }
        }
    }
};

class CpuPaddle : public Paddle {
   public:
    void update(int ball_y) {
        if ( y > ball_y - 15){
            y -= speed;
            if (y <= 0){
                y = 0;     
            }
        }
        if ( y + height < ball_y + 15 ){
            y += speed;
            if (y >= GetScreenHeight() - height){
                y = GetScreenHeight() - height;     
            }
        }
    }
};

Ball ball;
CpuPaddle cpu;
Paddle player;

void check_for_collissions(){
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x,cpu.y,20,100})){
            ball.speed_x *= -1;
        }
        if(CheckCollisionCircleRec (Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x,player.y,20,100})){
            ball.speed_x *= -1;
        }
}

int main () {

    const int screenWidth = 800;
    const int screenHeight = 600;

    ball.x = screenWidth/2;
    ball.y = screenHeight/2;
    ball.speed_x = 7;
    ball.speed_y = 7;
    ball.radius = 15;

    cpu.height = 100;
    cpu.width = 20;
    cpu.speed = 6;
    cpu.x = 10;
    cpu.y = screenHeight/2 - cpu.height/2;

    player.height = 100;
    player.width = 20;
    player.x = screenWidth - 30;
    player.y = screenHeight/2 - player.height/2;
    player.speed = 6;

    InitWindow(screenWidth, screenHeight, "My Pong Game!");
    SetTargetFPS(60);

    while (WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(Dark_Green);
        DrawRectangle(screenWidth/2, 0, screenWidth/2, screenHeight, Green);
        DrawCircle(screenWidth/2, screenHeight/2, 100, Light_Green);
        DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight, WHITE);    

        check_for_collissions(); 

        ball.update();
        player.update();
        cpu.update(ball.y);

        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i", cpu_score),screenWidth/4-20,20,80,WHITE);
        DrawText(TextFormat("%i", player_score),3*screenWidth/4-20,20,80,WHITE);
        EndDrawing();
    }
    return 0;
}