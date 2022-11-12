#include <raylib.h>

int cpu_score = 0;
int player_score = 0;

class Ball {
    public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x,y,radius, WHITE);
    }

    void update(){
        x += speed_x;
        y += speed_y;

        if(x + radius >= GetScreenWidth()) //Cpu wins a point
        {
            cpu_score += 1;
            reset_ball();
        }

        if(x - radius <= 0) // Player wins a point
        {
            player_score += 1;
            reset_ball();
        } 

        if(y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
    }

    void reset_ball() {
        int speed_choices[2] = {-1,1}; 
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
        x = GetScreenWidth()/2 -radius;
        y = 20;
    }
};

class Paddle {
    public:
    float x, y;
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
    ball.speed_x = 7;
    ball.speed_y = 7;
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

    InitWindow(screenWidth, screenHeight, "My Pong Game!");
    SetTargetFPS(60);

    while (WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(BLACK);

        DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight, WHITE);     
        ball.update();

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x,cpu.y,20,100}) )
        {
            ball.speed_x *= -1;
        }

        if(CheckCollisionCircleRec (Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x,player.y,20,100}) )
        {
            ball.speed_x *= -1;
        }


        player.update();
        cpu.update(ball.y);

        ball.Draw();
        cpu.Draw();
        player.Draw();

        DrawText(TextFormat("%i", cpu_score),screenWidth/4-40,20,80,WHITE);
        DrawText(TextFormat("%i", player_score),3*screenWidth/4-40,20,80,WHITE);

        EndDrawing();
    }
    return 0;
}