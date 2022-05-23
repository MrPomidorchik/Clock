#include <raylib.h>
#include <cmath>
#include <sys/time.h>

using namespace std;

int main()
{
    float CLOCK_RADIUS = 200.0;
    float CLOCK_WIDTH = 200.0;
    float CLOCK_HEIGHT = 200.0;
    float SIZE = CLOCK_RADIUS / 10.0;
    InitWindow(1920, 1080, "Clock");
    HideCursor();
    SetTargetFPS(60);
    ToggleFullscreen();

    while (!WindowShouldClose())
    {
        //ОБНОВЛЕНИЕ
        struct  timeval tp{};
        gettimeofday(&tp,nullptr);
        time_t t = tp.tv_sec;
        struct tm tm = *localtime(&t);

        double seconds = tm.tm_sec + ((float )tp.tv_usec * 0.000001);
        double minutes = tm.tm_min + (seconds / 60.0);
        double hours = tm.tm_hour + (minutes / 60.0);

        //секундная стрелка
        seconds -= 15.0;
        double secXDir = cos((float)seconds / 60.0 * 2.0 * PI);
        double secYDir = sin((float)seconds / 60.0 * 2.0 * PI);
        float secHandEndX = secXDir * (CLOCK_RADIUS * 0.9) + CLOCK_WIDTH;
        float secHandEndY = secYDir * (CLOCK_RADIUS * 0.9) + CLOCK_HEIGHT;

        //минутная стрелка
        minutes -= 15.0;
        double minXDir = cos((float)minutes / 60.0 * 2.0 * PI);
        double minYDir = sin((float)minutes / 60.0 * 2.0 * PI);
        float minHandEndX = minXDir * (CLOCK_RADIUS * 0.9) + CLOCK_WIDTH;
        float minHandEndY = minYDir * (CLOCK_RADIUS * 0.9) + CLOCK_HEIGHT;

        //часовая стрелка
        hours -= 3.0;
        double hourXDir = cos((float)hours / 12.0 * 2.0 * PI);
        double hourYDir = sin((float)hours / 12.0 * 2.0 * PI);
        float hourHandEndX = hourXDir * (CLOCK_RADIUS * 0.7) + CLOCK_WIDTH;
        float hourHandEndY = hourYDir * (CLOCK_RADIUS * 0.7) + CLOCK_HEIGHT;

        //ЦИФЕРБЛАТ
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawCircle(CLOCK_WIDTH, CLOCK_HEIGHT, CLOCK_RADIUS * (1.0 - 0.05) + 20, BLACK);
        DrawCircle(CLOCK_WIDTH, CLOCK_HEIGHT, CLOCK_RADIUS * (1.0 - 0.05), RAYWHITE);
        DrawText(TextFormat("%i", tm.tm_sec), CLOCK_WIDTH + 15, CLOCK_HEIGHT + (CLOCK_RADIUS / 2), 20, GREEN);
        DrawText(TextFormat("%i", tm.tm_min), CLOCK_WIDTH - 10, CLOCK_HEIGHT + (CLOCK_RADIUS / 2), 20, GREEN);
        DrawText(TextFormat("%i", tm.tm_hour), CLOCK_WIDTH - 35, CLOCK_HEIGHT + (CLOCK_RADIUS / 2), 20, GREEN);


        //отображение цифр
        for (int i = 0; i < 12; i++) {
            const char *hourString = TextFormat("%d", i + 1);
            struct Vector2 stringCenter = MeasureTextEx(GetFontDefault(), hourString, SIZE, 0);
            stringCenter.x = stringCenter.x / 2;
            stringCenter.y = stringCenter.y / 2;
            double angle = (float)(i - 2) / 12.0 * 2.0 * PI;
            double xDir = cos(angle);
            double yDir = sin(angle);
            double fontDist = 0.8;
            DrawText(hourString, CLOCK_WIDTH + xDir * CLOCK_RADIUS * fontDist - stringCenter.x, CLOCK_HEIGHT + yDir * CLOCK_RADIUS * fontDist - stringCenter.y, SIZE, BLACK);
        }

        //отображение делений
        for (int l = 0; l < 60; l++) {
            float angleL = (l - 2) / 60.0 * 2.0 * PI;
            float angleB = (l - 2) / 12.0 * 2.0 * PI;
            float xDirL = cos(angleL);
            float yDirL = sin(angleL);
            float xDirB = cos(angleB);
            float yDirB = sin(angleB);
            float dist = 0.93;
            DrawLineEx((Vector2){CLOCK_WIDTH + xDirL * CLOCK_RADIUS * dist, CLOCK_HEIGHT + yDirL * CLOCK_RADIUS * dist},(Vector2){CLOCK_WIDTH + xDirL * (CLOCK_RADIUS - 10) * dist, CLOCK_HEIGHT + yDirL * (CLOCK_RADIUS - 10) * dist}, CLOCK_RADIUS / 100.0, BLACK);
            DrawLineEx((Vector2){CLOCK_WIDTH + xDirB * CLOCK_RADIUS * dist, CLOCK_HEIGHT + yDirB * CLOCK_RADIUS * dist},(Vector2){CLOCK_WIDTH + xDirB * (CLOCK_RADIUS - 15) * dist, CLOCK_HEIGHT + yDirB * (CLOCK_RADIUS - 15) * dist}, CLOCK_RADIUS / 50.0, BLACK);
        }
        // отображение часовой стрелки
        DrawLineEx((Vector2){CLOCK_WIDTH, CLOCK_HEIGHT}, (Vector2){hourHandEndX, hourHandEndY}, CLOCK_RADIUS / 50.0, BLACK);

        //отображение минутной стрелки
        DrawLineEx((Vector2){CLOCK_WIDTH, CLOCK_HEIGHT}, (Vector2){minHandEndX, minHandEndY}, CLOCK_RADIUS / 50.0, BLACK);

        //отображение секундной стрелки
        DrawLineEx((Vector2){CLOCK_WIDTH, CLOCK_HEIGHT}, (Vector2){secHandEndX, secHandEndY}, CLOCK_RADIUS / 100.0, RED);

        //УПРАВЛЕНИЕ

        //увеличение
        if((IsKeyPressed(KEY_EQUAL) && CLOCK_RADIUS != 560.0) || (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_EQUAL) && CLOCK_RADIUS != 560.0)){
            CLOCK_RADIUS = CLOCK_RADIUS + 20.0;
            SIZE = SIZE + 2.0;
        }
        //уменьшение
        if((IsKeyPressed(KEY_MINUS) && CLOCK_RADIUS != 200.0) || (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_MINUS) && CLOCK_RADIUS != 200.0)) {
            CLOCK_RADIUS = CLOCK_RADIUS - 20.0;
            SIZE = SIZE - 2.0;
        }
        //движение вправо
        if((IsKeyPressed(KEY_RIGHT) && CLOCK_WIDTH != 1720.0) || (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_RIGHT) && CLOCK_WIDTH != 1720.0)) {
            CLOCK_WIDTH = CLOCK_WIDTH + 20.0;
        }
        //движение влево
        if((IsKeyPressed(KEY_LEFT) && CLOCK_WIDTH != 200.0) || (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT) && CLOCK_WIDTH != 200.0)) {
            CLOCK_WIDTH = CLOCK_WIDTH - 20.0;
        }
        //движение вверх
        if((IsKeyPressed(KEY_UP) && CLOCK_HEIGHT != 200.0) || (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_UP) && CLOCK_HEIGHT != 200.0)) {
            CLOCK_HEIGHT = CLOCK_HEIGHT - 20.0;
        }
        //движение вниз
        if((IsKeyPressed(KEY_DOWN) && CLOCK_HEIGHT != 880.0) || (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_DOWN) && CLOCK_HEIGHT != 880.0)) {
            CLOCK_HEIGHT = CLOCK_HEIGHT + 20.0;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}