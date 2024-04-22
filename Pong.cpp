#include <iostream>
#include <raylib.h>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;
Color Green = Color{ 38, 185, 154, 255 };
Color Dark_Green = Color{ 20, 160, 133, 255 };
Color Light_Green = Color{ 129, 204, 184, 255 };
Color Yellow = Color{ 243, 213, 91, 255 };

int player_score = 0;
int cpu_score = 0;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    Ball(float _x, float _y, int _speed_x, int _speed_y, int _radius) : x(_x), y(_y), speed_x(_speed_x), speed_y(_speed_y), radius(_radius) {}

    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update() {
        if ((x + radius) >= SCREEN_WIDTH) {
            // CPU score
            cpu_score++;
            ResetBall();
        }

        if  ((x - radius) <= 0) {
            // Player score
            player_score++;
            ResetBall();
        }

        if ((y + radius) >= SCREEN_HEIGHT || (y - radius) <= 0) {
            speed_y *= -1;
        }

        x += speed_x;
        y += speed_y;
    }

    void ResetBall() {
        x = SCREEN_WIDTH / 2;
        y = SCREEN_HEIGHT / 2;

        int speed_choices[2] = { -1, 1 };
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};


class Paddle {
public:
    float x, y;
    float width, height;
    int speed;

    Paddle(float _x, float _y, float _width, float _height, int _speed) : x(_x), y(_y), width(_width), height(_height), speed(_speed) {}

    void Draw() {
        DrawRectangleRounded(Rectangle{ x , y , width, height }, 0.8, 0, WHITE);
    }

     virtual void Update() {
        if (IsKeyDown(KEY_UP) && (y) >= 0) {
            y -= speed;
        }
        if (IsKeyDown(KEY_DOWN) && (y + height) <= GetScreenHeight()){
            y += speed;
        }
    }
};

class CpuPaddle : public Paddle {
public:
    CpuPaddle(float _x, float _y, int _width, int _height, int _speed) : Paddle(_x, _y, _width, _height, _speed) {}

    void Update(int ball_y){
        if (y + height / 2 > ball_y) {
            if (y - height / 2 <= 0) {
                y = height / 2;
            } else {
                y -= speed;
            }
        }
     

        if (y + height / 2 < ball_y) {
            if (y + height / 2 >= GetScreenHeight()) {
                y = GetScreenHeight() - height;
            }
            else {
                y += speed;
            }
        }
    }

};


int player_paddle_offset = -10;
int cpu_paddle_offset = 10;
Ball ball = Ball(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 7, 7, 20);
Paddle player = Paddle(SCREEN_WIDTH + player_paddle_offset - 25, SCREEN_HEIGHT / 2, 25, 120, 7);
CpuPaddle cpu = CpuPaddle(cpu_paddle_offset, SCREEN_HEIGHT / 2, 25, 120, 6);

int main()
{
    cout << "Starting the Game" << endl;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My Pong Game!");
    SetTargetFPS(60);

    while (WindowShouldClose() == false) {
        BeginDrawing();

        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height })) 
        {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height })) 
        {
            ball.speed_x *= -1;
        }


        ClearBackground(Dark_Green);
        DrawRectangle(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, Green);
        DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 150, Light_Green);
        DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i",cpu_score), SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i",player_score), 3 * SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}