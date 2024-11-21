#include <raylib.h>
#include <iostream>

using namespace std;

Color green = Color{38, 185, 154, 255};
Color darkGreen = Color{20, 160, 133, 255};
Color lightGreen = Color{129, 204, 184, 255};
Color yellow = Color{243, 213, 91, 255};

int playerScore = 0;
int cpuScore = 0;

class Ball 
{
    public:
        float x, y;
        int speedX, speedY;
        int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, yellow);
    }

    void Update()
    {
        x += speedX;
        y += speedY;

        if(y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speedY *= -1;
        }
        // If this occurs, CPU gets a point
        if(x + radius >= GetScreenWidth())
        {
            cpuScore++;
            ResetBall();
        }   
        // If this occurs, player gets a point
        if(x - radius <= 0)
        {
            playerScore++;
            ResetBall();
        }
    }

    void ResetBall() 
    {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speedChoices[2] = {-1, 1};
        speedX *= speedChoices[GetRandomValue(0, 1)];
        speedY *= speedChoices[GetRandomValue(0, 1)];
    }
};

class Paddle
{   
    protected:
        void LimitMovement()
        {
            // Prevent the paddle from going off-screen
            if(y <= 0)
            {
                y = 0;
            }
            if(y + height >= GetScreenHeight())
            {
                y = GetScreenHeight() - height;
            }
        }

    public:
        float x, y;
        float width, height;
        int speed;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update()
    {
        if(IsKeyDown(KEY_UP))
        {
            y -= speed;
        }
        if(IsKeyDown(KEY_DOWN))
        {
            y += speed;
        }

        LimitMovement();
    }
};

class CPU_Paddle: public Paddle
{
    public:
        void Update(int ballY)
        {
            if(y + height/2 > ballY)
            {
                y -= speed;
            }
            if(y + height/2 <= ballY)
            {
                y += speed;
            }
            LimitMovement();
        }
};

int main() 
{
    // Initialize game window and FPS settings
    const int screenWidth = 1280;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "My Pong Game");  
    SetTargetFPS(60);

    // Initialize game objects
    Ball ball;
    ball.radius = 20;
    ball.x = screenWidth/2;
    ball.y = screenHeight/2;
    ball.speedX = 7;
    ball.speedY = 7;

    Paddle player;
    player.width = 25;
    player.height = 120;
    player.x = screenWidth - player.width - 10;
    player.y = screenHeight/2 - player.height/2;
    player.speed = 6;

    CPU_Paddle cpuPaddle;
    cpuPaddle.height = 120;
    cpuPaddle.width = 25;
    cpuPaddle.x = 10;
    cpuPaddle.y = screenHeight/2 - cpuPaddle.height/2;
    cpuPaddle.speed = 6;

    // Main game loop
    while(WindowShouldClose() == false) // Checks for escape button or if X was pressed on the window
    {   
        // --- Updating --- //
        ball.Update();
        player.Update();
        cpuPaddle.Update(ball.y);
        // --- Updating --- //

        // Check for collisions
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speedX *= -1;
        }
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpuPaddle.x, cpuPaddle.y, cpuPaddle.width, cpuPaddle.height}))
        {
            ball.speedX *= -1;
        }

        // --- Drawing --- //
        ClearBackground(darkGreen); // Clears screen after movement to prevent tracers
        DrawRectangle(screenWidth/2, 0, screenWidth/2, screenHeight, green);
        DrawCircle(screenWidth/2, screenHeight/2, 150, lightGreen);

        BeginDrawing();

        DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight, WHITE);
        ball.Draw();
        cpuPaddle.Draw();
        player.Draw();

        DrawText(TextFormat("%i",cpuScore), screenWidth/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i",playerScore), 3 * screenWidth/4 - 20, 20, 80, WHITE);

        EndDrawing();
        // --- Drawing --- //
    }

    CloseWindow();

    return 0;
}