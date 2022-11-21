#include <iostream>
#include <raylib.h>

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

using namespace std;

class Paddle
{

protected:
    void LimitMovement()
    {
        if (y <= 0)
        {
            y = 0;
        }
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;
    int score = 0;

    void Draw()
    {
        // DrawRectangle(x, y, width, height, WHITE);
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8f, 0, WHITE);
    }

    void Update()
    {
        if (IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }

        if (IsKeyDown(KEY_UP))
        {
            y -= speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle
{
public:
    void Update(int ball_y)
    {
        if (y + height / 2 > ball_y)
        {
            y = y - speed;
        }

        if (y + height / 2 < ball_y)
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

class Ball
{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update(Paddle &player, CpuPaddle &cpu)
    {
        x = x + speed_x;
        y = y + speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }

        if (x + radius >= GetScreenWidth()) // Computer Wins
        {
            cpu.score++;
            ResetBall();
        }

        if (x - radius <= 0) // Player Wins
        {
            player.score++;
            ResetBall();
        }
    }

    void ResetBall()
    {
        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main()
{
    cout << "Starting the game." << endl;

    const int screen_width = 1280;
    const int screen_height = 800;

    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;
    ball.radius = 15;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;
    player.score = 0;

    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;
    cpu.score = 0;

    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(60);

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        // Updating
        ball.Update(player, cpu);
        player.Update();
        cpu.Update(ball.y);

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            ball.speed_x *= -1;
        }

        // Drawing
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", cpu.score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player.score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}