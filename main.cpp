#include <iostream>
#include <raylib.h>

using namespace std;

class Ball {
    public:
    int x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x,y,radius, WHITE);
    }
};

class Paddle {
    public:
    int x, y;
    int width, height;
    int speed;

    void Draw(){
        DrawRectangle(x, y, width, height, WHITE);
    }

    void update() {

        if (IsKeyDown(KEY_DOWN))
        {
            y += speed;
            if (y >= GetScreenHeight() - height)
            {
                y = GetScreenHeight() - height;     
            }
        }

        if (IsKeyDown(KEY_UP))
        {
            y -= speed;
            if (y <= 0)
            {
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
            if (y <= 0)
            {
                y = 0;     
            }
        }

        if ( y + height < ball_y + 15 )
        {
            y += speed;
            if (y >= GetScreenHeight() - height)
            {
                y = GetScreenHeight() - height;     
            }
        }
    }
};

int main () {

    Ball ball;
    CpuPaddle cpu;
    Paddle player;

    const int screenWidth = 800;
    const int screenHeight = 600;
    ball.x = screenWidth/2;
    ball.y = screenHeight/2;
    ball.speed_x = 5;
    ball.speed_y = 5;
    ball.radius = 15;

    cpu.height = 100;
    cpu.width = 20;
    cpu.speed = 6;
    cpu.x = 0;
    cpu.y = screenHeight/2 - cpu.height/2;

    player.height = 100;
    player.width = 20;
    player.x = screenWidth - 20;
    player.y = screenHeight/2 - player.height/2;
    player.speed = 6;

    cout << "Hello World" << endl;

    InitWindow(screenWidth, screenHeight, "My Pong Game!");
    SetTargetFPS(60);

    while (WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(BLACK);

        ball.x += ball.speed_x;
        ball.y += ball.speed_y;

        if(ball.x + ball.radius >= screenWidth  || ball.x - ball.radius <= 0)
        {
            ball.speed_x *= -1;
        }

        if(ball.y + ball.radius >= screenHeight  || ball.y - ball.radius <= 0)
        {
            ball.speed_y *= -1;
        }

        player.update();
        cpu.update(ball.y);

        ball.Draw();
        cpu.Draw();
        player.Draw();

        EndDrawing();
    }

    return 0;
}