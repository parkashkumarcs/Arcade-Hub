/*******************************************************************************************
*
*   GAMING HUB - Professional C++ Arcade
*   
*   A collection of 5 mini-games with beautiful GUI
*   Built with raylib - A simple and easy-to-use game programming library
*
*   Games included:
*   1. Tic-Tac-Toe (2-Player)
*   2. Space Survivor (Dodge asteroids)
*   3. Break The Code (Mastermind puzzle)
*   4. Escape Zombie Land (Turn-based escape)
*   5. Swap Cards (Memory match)
*
*   First Semester C++ Project
*
********************************************************************************************/

#include "raylib.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <algorithm>

using namespace std;

// ═══════════════════════════════════════════════════════════════════════════
// GLOBAL CONSTANTS AND ENUMS
// ═══════════════════════════════════════════════════════════════════════════

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 700;

enum GameState {
    MENU,
    TICTACTOE,
    SPACE_SURVIVOR,
    BREAK_CODE,
    ZOMBIE_LAND,
    SWAP_CARDS
};

// ═══════════════════════════════════════════════════════════════════════════
// COLOR PALETTE
// ═══════════════════════════════════════════════════════════════════════════

namespace Colors {
    const Color BACKGROUND = {15, 15, 35, 255};
    const Color PRIMARY = {100, 149, 237, 255};      // Cornflower blue
    const Color SECONDARY = {138, 43, 226, 255};     // Blue violet
    const Color ACCENT = {255, 215, 0, 255};         // Gold
    const Color SUCCESS = {50, 205, 50, 255};        // Lime green
    const Color DANGER = {220, 20, 60, 255};         // Crimson
    const Color CARD_BG = {40, 40, 80, 255};
    const Color CARD_HOVER = {60, 60, 120, 255};
    const Color TEXT_LIGHT = {240, 240, 255, 255};
    const Color TEXT_DIM = {150, 150, 180, 255};
}

// ═══════════════════════════════════════════════════════════════════════════
// UTILITY FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

// Draw text centered horizontally
void DrawTextCentered(const char* text, int y, int fontSize, Color color) {
    int width = MeasureText(text, fontSize);
    DrawText(text, (SCREEN_WIDTH - width) / 2, y, fontSize, color);
}

// Draw a rounded button and return true if clicked
bool DrawButton(Rectangle rect, const char* text, Color bgColor, Color hoverColor, int fontSize = 24) {
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rect);
    bool clicked = hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    Color color = hover ? hoverColor : bgColor;
    DrawRectangleRounded(rect, 0.3f, 8, color);
    DrawRectangleRoundedLines(rect, 0.3f, 8, 2, WHITE);
    
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, rect.x + (rect.width - textWidth) / 2, 
             rect.y + (rect.height - fontSize) / 2, fontSize, WHITE);
    
    return clicked;
}

// Draw decorative animated background
void DrawAnimatedBackground(float time) {
    ClearBackground(Colors::BACKGROUND);
    
    // Draw floating particles
    for (int i = 0; i < 50; i++) {
        float x = (float)((i * 73) % SCREEN_WIDTH);
        float y = (float)(((int)(time * 20 + i * 50) % SCREEN_HEIGHT));
        float size = 2 + (i % 3);
        unsigned char alpha = 50 + (i % 100);
        DrawCircle(x, y, size, (Color){100, 149, 237, alpha});
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// FORWARD DECLARATIONS
// ═══════════════════════════════════════════════════════════════════════════

class TicTacToeGame;
class SpaceSurvivorGame;
class BreakTheCodeGame;
class ZombieLandGame;
class SwapCardsGame;

// ═══════════════════════════════════════════════════════════════════════════
// TIC-TAC-TOE GAME CLASS
// ═══════════════════════════════════════════════════════════════════════════

class TicTacToeGame {
private:
    char board[9];
    char currentPlayer;
    bool gameOver;
    char winner;
    float cellSize;
    float offsetX, offsetY;
    float animTime;

public:
    void Init() {
        for (int i = 0; i < 9; i++) board[i] = ' ';
        currentPlayer = 'X';
        gameOver = false;
        winner = ' ';
        cellSize = 120;
        offsetX = (SCREEN_WIDTH - cellSize * 3) / 2;
        offsetY = 200;
        animTime = 0;
    }

    bool CheckWin(char symbol) {
        int wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
        for (int i = 0; i < 8; i++) {
            if (board[wins[i][0]] == symbol && board[wins[i][1]] == symbol && board[wins[i][2]] == symbol)
                return true;
        }
        return false;
    }

    bool IsBoardFull() {
        for (int i = 0; i < 9; i++) if (board[i] == ' ') return false;
        return true;
    }

    bool Update() {  // Returns true if should go back to menu
        animTime += GetFrameTime();

        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M)) return true;

        if (!gameOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            int col = (int)((mouse.x - offsetX) / cellSize);
            int row = (int)((mouse.y - offsetY) / cellSize);

            if (col >= 0 && col < 3 && row >= 0 && row < 3) {
                int idx = row * 3 + col;
                if (board[idx] == ' ') {
                    board[idx] = currentPlayer;

                    if (CheckWin(currentPlayer)) {
                        gameOver = true;
                        winner = currentPlayer;
                    } else if (IsBoardFull()) {
                        gameOver = true;
                        winner = 'D';  // Draw
                    } else {
                        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                    }
                }
            }
        }

        if (gameOver && IsKeyPressed(KEY_SPACE)) Init();

        return false;
    }

    void Draw() {
        DrawAnimatedBackground(animTime);

        // Title
        DrawTextCentered("TIC-TAC-TOE", 50, 50, Colors::ACCENT);
        DrawTextCentered("2-Player Game", 110, 24, Colors::TEXT_DIM);

        // Draw grid
        for (int i = 0; i <= 3; i++) {
            // Vertical lines
            DrawLineEx({offsetX + i * cellSize, offsetY},
                       {offsetX + i * cellSize, offsetY + 3 * cellSize}, 4, Colors::PRIMARY);
            // Horizontal lines
            DrawLineEx({offsetX, offsetY + i * cellSize},
                       {offsetX + 3 * cellSize, offsetY + i * cellSize}, 4, Colors::PRIMARY);
        }

        // Draw X's and O's
        for (int i = 0; i < 9; i++) {
            int row = i / 3, col = i % 3;
            float cx = offsetX + col * cellSize + cellSize / 2;
            float cy = offsetY + row * cellSize + cellSize / 2;

            if (board[i] == 'X') {
                float s = 35;
                DrawLineEx({cx - s, cy - s}, {cx + s, cy + s}, 8, Colors::SUCCESS);
                DrawLineEx({cx + s, cy - s}, {cx - s, cy + s}, 8, Colors::SUCCESS);
            } else if (board[i] == 'O') {
                DrawCircleLines(cx, cy, 40, Colors::DANGER);
                DrawCircleLines(cx, cy, 38, Colors::DANGER);
                DrawCircleLines(cx, cy, 36, Colors::DANGER);
            }
        }

        // Current player or result
        if (!gameOver) {
            const char* playerText = (currentPlayer == 'X') ? "Player X's Turn" : "Player O's Turn";
            Color playerColor = (currentPlayer == 'X') ? Colors::SUCCESS : Colors::DANGER;
            DrawTextCentered(playerText, 580, 30, playerColor);
        } else {
            if (winner == 'D') {
                DrawTextCentered("IT'S A DRAW!", 560, 40, Colors::ACCENT);
            } else {
                const char* winText = (winner == 'X') ? "PLAYER X WINS!" : "PLAYER O WINS!";
                Color winColor = (winner == 'X') ? Colors::SUCCESS : Colors::DANGER;
                DrawTextCentered(winText, 560, 40, winColor);
            }
            DrawTextCentered("Press SPACE to play again", 610, 20, Colors::TEXT_DIM);
        }

        // Instructions
        DrawText("Press M or ESC to return to menu", 20, SCREEN_HEIGHT - 30, 16, Colors::TEXT_DIM);
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// SPACE SURVIVOR GAME CLASS
// ═══════════════════════════════════════════════════════════════════════════

class SpaceSurvivorGame {
private:
    struct Asteroid {
        float x, y;
        float speed;
        float size;
    };

    float playerX;
    float playerY;
    int score;
    bool gameOver;
    vector<Asteroid> asteroids;
    float animTime;
    vector<pair<float, float>> stars;

public:
    void Init() {
        playerX = SCREEN_WIDTH / 2;
        playerY = SCREEN_HEIGHT - 80;
        score = 0;
        gameOver = false;
        animTime = 0;
        asteroids.clear();

        // Create initial asteroids
        for (int i = 0; i < 8; i++) {
            Asteroid a;
            a.x = rand() % SCREEN_WIDTH;
            a.y = -(rand() % 300);
            a.speed = 3 + (rand() % 4);
            a.size = 15 + (rand() % 20);
            asteroids.push_back(a);
        }

        // Create stars
        stars.clear();
        for (int i = 0; i < 100; i++) {
            stars.push_back({(float)(rand() % SCREEN_WIDTH), (float)(rand() % SCREEN_HEIGHT)});
        }
    }

    bool Update() {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M)) return true;

        animTime += GetFrameTime();

        if (gameOver) {
            if (IsKeyPressed(KEY_SPACE)) Init();
            return false;
        }

        // Player movement
        float speed = 8.0f;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) playerX -= speed;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) playerX += speed;

        if (playerX < 30) playerX = 30;
        if (playerX > SCREEN_WIDTH - 30) playerX = SCREEN_WIDTH - 30;

        // Update asteroids
        for (auto& a : asteroids) {
            a.y += a.speed;

            // Collision detection
            float dx = a.x - playerX;
            float dy = a.y - playerY;
            if (dx * dx + dy * dy < (a.size + 20) * (a.size + 20)) {
                gameOver = true;
            }

            // Respawn asteroid if off screen
            if (a.y > SCREEN_HEIGHT + 50) {
                a.y = -50;
                a.x = rand() % SCREEN_WIDTH;
                a.speed = 3 + (rand() % 4) + score / 500;
                a.size = 15 + (rand() % 20);
                score += 10;
            }
        }

        // Add more asteroids as score increases
        if (score > 0 && score % 200 == 0 && asteroids.size() < 20) {
            Asteroid a;
            a.x = rand() % SCREEN_WIDTH;
            a.y = -50;
            a.speed = 4 + (rand() % 5);
            a.size = 15 + (rand() % 25);
            asteroids.push_back(a);
        }

        return false;
    }

    void Draw() {
        ClearBackground({5, 5, 20, 255});

        // Draw moving stars
        for (auto& star : stars) {
            float y = star.second + animTime * 30;
            while (y > SCREEN_HEIGHT) y -= SCREEN_HEIGHT;
            DrawCircle(star.first, y, 1, (Color){255, 255, 255, 150});
        }

        // Title and score
        DrawTextCentered("SPACE SURVIVOR", 20, 36, Colors::ACCENT);

        char scoreText[32];
        sprintf(scoreText, "SCORE: %d", score);
        DrawTextCentered(scoreText, 65, 28, Colors::TEXT_LIGHT);

        // Draw asteroids
        for (auto& a : asteroids) {
            DrawCircle(a.x, a.y, a.size, (Color){139, 69, 19, 255});
            DrawCircle(a.x - a.size/4, a.y - a.size/4, a.size/4, (Color){100, 50, 10, 255});
            DrawCircleLines(a.x, a.y, a.size, (Color){80, 40, 10, 255});
        }

        // Draw player ship
        if (!gameOver) {
            // Ship body (triangle)
            Vector2 v1 = {playerX, playerY - 25};
            Vector2 v2 = {playerX - 20, playerY + 20};
            Vector2 v3 = {playerX + 20, playerY + 20};
            DrawTriangle(v1, v3, v2, Colors::PRIMARY);
            DrawTriangleLines(v1, v3, v2, WHITE);

            // Engine flame
            float flameSize = 10 + sin(animTime * 20) * 5;
            DrawTriangle({playerX, playerY + 20},
                        {playerX + 8, playerY + 20 + flameSize},
                        {playerX - 8, playerY + 20 + flameSize}, ORANGE);
        }

        // Game over screen
        if (gameOver) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 180});
            DrawTextCentered("GAME OVER", SCREEN_HEIGHT/2 - 60, 60, Colors::DANGER);

            sprintf(scoreText, "Final Score: %d", score);
            DrawTextCentered(scoreText, SCREEN_HEIGHT/2 + 10, 30, Colors::ACCENT);
            DrawTextCentered("Press SPACE to play again", SCREEN_HEIGHT/2 + 60, 20, Colors::TEXT_DIM);
        }

        // Controls hint
        DrawText("Controls: A/D or Arrow Keys | Press M to return to menu",
                 20, SCREEN_HEIGHT - 30, 16, Colors::TEXT_DIM);
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// BREAK THE CODE GAME CLASS
// ═══════════════════════════════════════════════════════════════════════════

class BreakTheCodeGame {
private:
    int secretCode[4];
    int guesses[10][4];
    int feedback[10][2];  // [correctPos, correctDigit]
    int currentGuess;
    int inputDigits[4];
    int inputIndex;
    bool gameOver;
    bool won;
    float animTime;

public:
    void Init() {
        // Generate secret code with unique digits
        vector<int> digits = {0,1,2,3,4,5,6,7,8,9};
        for (int i = 9; i > 0; i--) {
            int j = rand() % (i + 1);
            swap(digits[i], digits[j]);
        }
        for (int i = 0; i < 4; i++) secretCode[i] = digits[i];

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 4; j++) guesses[i][j] = -1;
            feedback[i][0] = feedback[i][1] = 0;
        }

        currentGuess = 0;
        inputIndex = 0;
        for (int i = 0; i < 4; i++) inputDigits[i] = -1;
        gameOver = false;
        won = false;
        animTime = 0;
    }

    void CheckGuess() {
        int correctPos = 0, correctDigit = 0;
        bool secretUsed[4] = {false}, guessUsed[4] = {false};

        for (int i = 0; i < 4; i++) {
            if (inputDigits[i] == secretCode[i]) {
                correctPos++;
                secretUsed[i] = guessUsed[i] = true;
            }
        }

        for (int i = 0; i < 4; i++) {
            if (!guessUsed[i]) {
                for (int j = 0; j < 4; j++) {
                    if (!secretUsed[j] && inputDigits[i] == secretCode[j]) {
                        correctDigit++;
                        secretUsed[j] = true;
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < 4; i++) guesses[currentGuess][i] = inputDigits[i];
        feedback[currentGuess][0] = correctPos;
        feedback[currentGuess][1] = correctDigit;

        if (correctPos == 4) {
            won = gameOver = true;
            // Save score
            ofstream file("codebreaker_leaderboard.txt", ios::app);
            if (file.is_open()) {
                file << "Cracked in " << (currentGuess + 1) << " attempts\n";
                file.close();
            }
        } else {
            currentGuess++;
            if (currentGuess >= 10) gameOver = true;
        }

        inputIndex = 0;
        for (int i = 0; i < 4; i++) inputDigits[i] = -1;
    }

    bool Update() {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M)) return true;

        animTime += GetFrameTime();

        if (gameOver) {
            if (IsKeyPressed(KEY_SPACE)) Init();
            return false;
        }

        // Number input
        for (int key = KEY_ZERO; key <= KEY_NINE; key++) {
            if (IsKeyPressed(key) && inputIndex < 4) {
                inputDigits[inputIndex++] = key - KEY_ZERO;
            }
        }

        // Backspace
        if (IsKeyPressed(KEY_BACKSPACE) && inputIndex > 0) {
            inputDigits[--inputIndex] = -1;
        }

        // Submit guess
        if (IsKeyPressed(KEY_ENTER) && inputIndex == 4) {
            CheckGuess();
        }

        return false;
    }

    void Draw() {
        DrawAnimatedBackground(animTime);

        DrawTextCentered("BREAK THE CODE", 30, 40, Colors::ACCENT);
        DrawTextCentered("Guess the 4-digit code (unique digits 0-9)", 80, 20, Colors::TEXT_DIM);

        // Draw previous guesses
        float startY = 120;
        for (int g = 0; g < 10; g++) {
            float y = startY + g * 45;

            // Attempt number
            char numText[8];
            sprintf(numText, "%2d.", g + 1);
            DrawText(numText, 250, y + 8, 20, g < currentGuess ? Colors::TEXT_DIM : Colors::TEXT_LIGHT);

            // Digit boxes
            for (int d = 0; d < 4; d++) {
                Rectangle box = {300 + d * 60.0f, y, 50, 40};
                Color boxColor = (g == currentGuess) ? Colors::CARD_HOVER : Colors::CARD_BG;
                DrawRectangleRounded(box, 0.2f, 4, boxColor);
                DrawRectangleRoundedLines(box, 0.2f, 4, 2, Colors::PRIMARY);

                if (guesses[g][d] >= 0) {
                    char digit[2] = {(char)('0' + guesses[g][d]), 0};
                    DrawText(digit, box.x + 18, box.y + 8, 24, Colors::TEXT_LIGHT);
                } else if (g == currentGuess && d < inputIndex) {
                    char digit[2] = {(char)('0' + inputDigits[d]), 0};
                    DrawText(digit, box.x + 18, box.y + 8, 24, Colors::ACCENT);
                }
            }

            // Feedback
            if (g < currentGuess) {
                char fb[32];
                sprintf(fb, "%d correct, %d misplaced", feedback[g][0], feedback[g][1]);
                DrawText(fb, 560, y + 10, 18,
                         feedback[g][0] == 4 ? Colors::SUCCESS : Colors::TEXT_DIM);
            }
        }

        // Current input instruction
        if (!gameOver) {
            DrawTextCentered("Type 4 digits and press ENTER to guess",
                           SCREEN_HEIGHT - 80, 20, Colors::TEXT_LIGHT);
        } else {
            if (won) {
                char winText[64];
                sprintf(winText, "YOU WON in %d attempts!", currentGuess);
                DrawTextCentered(winText, SCREEN_HEIGHT - 100, 30, Colors::SUCCESS);
            } else {
                char loseText[64];
                sprintf(loseText, "Game Over! Code was: %d%d%d%d",
                        secretCode[0], secretCode[1], secretCode[2], secretCode[3]);
                DrawTextCentered(loseText, SCREEN_HEIGHT - 100, 26, Colors::DANGER);
            }
            DrawTextCentered("Press SPACE to play again", SCREEN_HEIGHT - 60, 18, Colors::TEXT_DIM);
        }

        DrawText("Press M or ESC to return to menu", 20, SCREEN_HEIGHT - 30, 16, Colors::TEXT_DIM);
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// ESCAPE ZOMBIE LAND GAME CLASS
// ═══════════════════════════════════════════════════════════════════════════

class ZombieLandGame {
private:
    static const int GRID_SIZE = 10;
    int playerX, playerY;
    int zombieX, zombieY;
    int safeX, safeY;
    bool gameOver;
    bool won;
    float animTime;
    float cellSize;
    float offsetX, offsetY;

public:
    void Init() {
        playerX = playerY = 0;
        zombieX = zombieY = GRID_SIZE / 2;
        safeX = safeY = GRID_SIZE - 1;
        gameOver = false;
        won = false;
        animTime = 0;
        cellSize = 50;
        offsetX = (SCREEN_WIDTH - GRID_SIZE * cellSize) / 2;
        offsetY = 150;
    }

    void MoveZombie() {
        int dir = rand() % 4;
        int newX = zombieX, newY = zombieY;
        if (dir == 0) newY--;
        else if (dir == 1) newY++;
        else if (dir == 2) newX--;
        else newX++;

        if (newX >= 0 && newX < GRID_SIZE) zombieX = newX;
        if (newY >= 0 && newY < GRID_SIZE) zombieY = newY;
    }

    bool Update() {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M)) return true;

        animTime += GetFrameTime();

        if (gameOver) {
            if (IsKeyPressed(KEY_SPACE)) Init();
            return false;
        }

        bool moved = false;
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { if (playerY > 0) playerY--; moved = true; }
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { if (playerY < GRID_SIZE-1) playerY++; moved = true; }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) { if (playerX > 0) playerX--; moved = true; }
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) { if (playerX < GRID_SIZE-1) playerX++; moved = true; }

        if (moved) {
            MoveZombie();

            // Check win
            if (playerX == safeX && playerY == safeY) {
                won = gameOver = true;
                ofstream file("zombie_escape_results.txt", ios::app);
                if (file.is_open()) { file << "Survived!\n"; file.close(); }
            }
            // Check lose
            else if (playerX == zombieX && playerY == zombieY) {
                gameOver = true;
                ofstream file("zombie_escape_results.txt", ios::app);
                if (file.is_open()) { file << "Caught by zombie!\n"; file.close(); }
            }
        }

        return false;
    }

    void Draw() {
        DrawAnimatedBackground(animTime);

        DrawTextCentered("ESCAPE ZOMBIE LAND", 30, 40, Colors::ACCENT);
        DrawTextCentered("Reach the Safe Zone! Avoid the Zombie!", 80, 20, Colors::TEXT_DIM);

        // Legend
        DrawText("You", 100, 120, 18, Colors::SUCCESS);
        DrawText("Zombie", 200, 120, 18, Colors::DANGER);
        DrawText("Safe Zone", 320, 120, 18, Colors::ACCENT);

        // Draw grid
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int x = 0; x < GRID_SIZE; x++) {
                Rectangle cell = {offsetX + x * cellSize, offsetY + y * cellSize, cellSize - 2, cellSize - 2};

                Color cellColor = Colors::CARD_BG;
                if (x == safeX && y == safeY) cellColor = (Color){255, 215, 0, 100};

                DrawRectangleRounded(cell, 0.1f, 4, cellColor);
                DrawRectangleRoundedLines(cell, 0.1f, 4, 1, Colors::PRIMARY);
            }
        }

        // Draw zombie
        float zx = offsetX + zombieX * cellSize + cellSize/2;
        float zy = offsetY + zombieY * cellSize + cellSize/2;
        float pulse = sin(animTime * 5) * 3;
        DrawCircle(zx, zy, 18 + pulse, Colors::DANGER);
        DrawText("Z", zx - 7, zy - 10, 24, WHITE);

        // Draw safe zone marker
        float sx = offsetX + safeX * cellSize + cellSize/2;
        float sy = offsetY + safeY * cellSize + cellSize/2;
        DrawCircle(sx, sy, 20, Colors::ACCENT);
        DrawText("S", sx - 7, sy - 10, 24, BLACK);

        // Draw player
        float px = offsetX + playerX * cellSize + cellSize/2;
        float py = offsetY + playerY * cellSize + cellSize/2;
        DrawCircle(px, py, 18, Colors::SUCCESS);
        DrawText("P", px - 7, py - 10, 24, WHITE);

        // Result
        if (gameOver) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 180});
            if (won) {
                DrawTextCentered("YOU ESCAPED!", SCREEN_HEIGHT/2 - 40, 50, Colors::SUCCESS);
            } else {
                DrawTextCentered("CAUGHT BY ZOMBIE!", SCREEN_HEIGHT/2 - 40, 50, Colors::DANGER);
            }
            DrawTextCentered("Press SPACE to play again", SCREEN_HEIGHT/2 + 30, 20, Colors::TEXT_DIM);
        }

        DrawText("Move: WASD or Arrow Keys | Press M to return to menu",
                 20, SCREEN_HEIGHT - 30, 16, Colors::TEXT_DIM);
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// SWAP CARDS GAME CLASS
// ═══════════════════════════════════════════════════════════════════════════

class SwapCardsGame {
private:
    char cards[16];
    bool revealed[16];
    bool matched[16];
    int firstCard, secondCard;
    int matches, moves;
    bool gameOver;
    float animTime;
    float revealTimer;
    float cardWidth, cardHeight;
    float offsetX, offsetY;

public:
    void Init() {
        char symbols[8] = {'A','B','C','D','E','F','G','H'};
        int idx = 0;
        for (int i = 0; i < 8; i++) {
            cards[idx++] = symbols[i];
            cards[idx++] = symbols[i];
        }

        // Shuffle
        for (int i = 0; i < 16; i++) {
            int j = rand() % 16;
            swap(cards[i], cards[j]);
        }

        for (int i = 0; i < 16; i++) {
            revealed[i] = matched[i] = false;
        }

        firstCard = secondCard = -1;
        matches = moves = 0;
        gameOver = false;
        animTime = revealTimer = 0;
        cardWidth = 120;
        cardHeight = 150;
        offsetX = (SCREEN_WIDTH - 4 * cardWidth - 30) / 2;
        offsetY = 150;
    }

    bool Update() {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M)) return true;

        animTime += GetFrameTime();

        if (gameOver) {
            if (IsKeyPressed(KEY_SPACE)) Init();
            return false;
        }

        // Handle reveal timer (when two cards are shown)
        if (revealTimer > 0) {
            revealTimer -= GetFrameTime();
            if (revealTimer <= 0) {
                if (cards[firstCard] == cards[secondCard]) {
                    matched[firstCard] = matched[secondCard] = true;
                    matches++;
                    if (matches == 8) {
                        gameOver = true;
                        ofstream file("swapcards_best_score.txt", ios::app);
                        if (file.is_open()) {
                            file << "Completed in " << moves << " moves\n";
                            file.close();
                        }
                    }
                }
                revealed[firstCard] = revealed[secondCard] = false;
                firstCard = secondCard = -1;
            }
            return false;
        }

        // Card selection
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();

            for (int i = 0; i < 16; i++) {
                if (matched[i] || revealed[i]) continue;

                int row = i / 4, col = i % 4;
                Rectangle card = {offsetX + col * (cardWidth + 10), offsetY + row * (cardHeight + 10),
                                  cardWidth, cardHeight};

                if (CheckCollisionPointRec(mouse, card)) {
                    revealed[i] = true;

                    if (firstCard < 0) {
                        firstCard = i;
                    } else {
                        secondCard = i;
                        moves++;
                        revealTimer = 1.0f;  // Show for 1 second
                    }
                    break;
                }
            }
        }

        return false;
    }

    void Draw() {
        DrawAnimatedBackground(animTime);

        DrawTextCentered("SWAP CARDS", 30, 40, Colors::ACCENT);

        char statsText[64];
        sprintf(statsText, "Moves: %d  |  Matches: %d/8", moves, matches);
        DrawTextCentered(statsText, 80, 24, Colors::TEXT_LIGHT);

        // Draw cards
        for (int i = 0; i < 16; i++) {
            int row = i / 4, col = i % 4;
            Rectangle card = {offsetX + col * (cardWidth + 10), offsetY + row * (cardHeight + 10),
                              cardWidth, cardHeight};

            Vector2 mouse = GetMousePosition();
            bool hover = CheckCollisionPointRec(mouse, card) && !matched[i] && !revealed[i];

            if (matched[i]) {
                // Matched card - green
                DrawRectangleRounded(card, 0.1f, 8, Colors::SUCCESS);
                char symbol[2] = {cards[i], 0};
                DrawText(symbol, card.x + cardWidth/2 - 15, card.y + cardHeight/2 - 20, 50, WHITE);
            } else if (revealed[i]) {
                // Revealed card - gold
                DrawRectangleRounded(card, 0.1f, 8, Colors::ACCENT);
                char symbol[2] = {cards[i], 0};
                DrawText(symbol, card.x + cardWidth/2 - 15, card.y + cardHeight/2 - 20, 50, BLACK);
            } else {
                // Hidden card
                Color cardColor = hover ? Colors::CARD_HOVER : Colors::CARD_BG;
                DrawRectangleRounded(card, 0.1f, 8, cardColor);
                DrawRectangleRoundedLines(card, 0.1f, 8, 2, Colors::PRIMARY);
                DrawText("?", card.x + cardWidth/2 - 12, card.y + cardHeight/2 - 20, 50, Colors::PRIMARY);
            }
        }

        // Game over
        if (gameOver) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 180});
            DrawTextCentered("CONGRATULATIONS!", SCREEN_HEIGHT/2 - 60, 50, Colors::SUCCESS);
            sprintf(statsText, "Completed in %d moves!", moves);
            DrawTextCentered(statsText, SCREEN_HEIGHT/2, 30, Colors::ACCENT);
            DrawTextCentered("Press SPACE to play again", SCREEN_HEIGHT/2 + 50, 20, Colors::TEXT_DIM);
        }

        DrawText("Click cards to flip | Press M to return to menu",
                 20, SCREEN_HEIGHT - 30, 16, Colors::TEXT_DIM);
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// MAIN MENU AND GAME LOOP
// ═══════════════════════════════════════════════════════════════════════════

// Menu button structure
struct MenuButton {
    const char* title;
    const char* description;
    Color color;
};

// Draw animated title
void DrawTitle(float time) {
    const char* title = "GAMING HUB";
    int fontSize = 70;
    int width = MeasureText(title, fontSize);
    float x = (SCREEN_WIDTH - width) / 2.0f;
    float y = 80 + sin(time * 2) * 5;

    // Shadow
    DrawText(title, x + 4, y + 4, fontSize, (Color){0, 0, 0, 100});
    // Main text with gradient effect
    DrawText(title, x, y, fontSize, Colors::ACCENT);

    // Subtitle
    DrawTextCentered("Professional C++ Arcade", 160, 24, Colors::TEXT_DIM);
    DrawTextCentered("First Semester Project", 190, 18, Colors::TEXT_DIM);
}

// Main function
int main() {
    // Initialize
    srand(time(0));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GAMING HUB - Professional C++ Arcade");
    SetTargetFPS(60);

    GameState currentState = MENU;
    float animTime = 0;

    // Game instances
    TicTacToeGame ticTacToe;
    SpaceSurvivorGame spaceSurvivor;
    BreakTheCodeGame breakTheCode;
    ZombieLandGame zombieLand;
    SwapCardsGame swapCards;

    // Menu buttons
    MenuButton buttons[5] = {
        {"Tic-Tac-Toe", "Classic 2-Player Strategy Game", Colors::SUCCESS},
        {"Space Survivor", "Dodge Asteroids in Space", Colors::PRIMARY},
        {"Break The Code", "Mastermind Puzzle Challenge", Colors::ACCENT},
        {"Escape Zombie Land", "Survive the Zombie Apocalypse", Colors::DANGER},
        {"Swap Cards", "Test Your Memory Skills", Colors::SECONDARY}
    };

    // Main game loop
    while (!WindowShouldClose()) {
        animTime += GetFrameTime();

        // Update based on state
        switch (currentState) {
            case MENU: {
                // Check button clicks
                float buttonY = 250;
                float buttonWidth = 350;
                float buttonHeight = 70;
                float buttonX = (SCREEN_WIDTH - buttonWidth) / 2;

                for (int i = 0; i < 5; i++) {
                    Rectangle btn = {buttonX, buttonY + i * 80, buttonWidth, buttonHeight};
                    Vector2 mouse = GetMousePosition();

                    if (CheckCollisionPointRec(mouse, btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        switch (i) {
                            case 0: currentState = TICTACTOE; ticTacToe.Init(); break;
                            case 1: currentState = SPACE_SURVIVOR; spaceSurvivor.Init(); break;
                            case 2: currentState = BREAK_CODE; breakTheCode.Init(); break;
                            case 3: currentState = ZOMBIE_LAND; zombieLand.Init(); break;
                            case 4: currentState = SWAP_CARDS; swapCards.Init(); break;
                        }
                    }
                }
                break;
            }
            case TICTACTOE:
                if (ticTacToe.Update()) currentState = MENU;
                break;
            case SPACE_SURVIVOR:
                if (spaceSurvivor.Update()) currentState = MENU;
                break;
            case BREAK_CODE:
                if (breakTheCode.Update()) currentState = MENU;
                break;
            case ZOMBIE_LAND:
                if (zombieLand.Update()) currentState = MENU;
                break;
            case SWAP_CARDS:
                if (swapCards.Update()) currentState = MENU;
                break;
        }

        // Draw
        BeginDrawing();

        switch (currentState) {
            case MENU: {
                DrawAnimatedBackground(animTime);
                DrawTitle(animTime);

                // Draw menu buttons
                float buttonY = 250;
                float buttonWidth = 350;
                float buttonHeight = 70;
                float buttonX = (SCREEN_WIDTH - buttonWidth) / 2;

                for (int i = 0; i < 5; i++) {
                    Rectangle btn = {buttonX, buttonY + i * 80, buttonWidth, buttonHeight};
                    Vector2 mouse = GetMousePosition();
                    bool hover = CheckCollisionPointRec(mouse, btn);

                    Color bgColor = hover ? buttons[i].color : Colors::CARD_BG;
                    DrawRectangleRounded(btn, 0.2f, 8, bgColor);
                    DrawRectangleRoundedLines(btn, 0.2f, 8, 2, buttons[i].color);

                    int textWidth = MeasureText(buttons[i].title, 28);
                    DrawText(buttons[i].title, btn.x + (btn.width - textWidth)/2, btn.y + 12, 28, WHITE);

                    int descWidth = MeasureText(buttons[i].description, 14);
                    DrawText(buttons[i].description, btn.x + (btn.width - descWidth)/2, btn.y + 45, 14,
                            hover ? WHITE : Colors::TEXT_DIM);
                }

                // Footer
                DrawTextCentered("Click a game to start | Press ESC to exit", SCREEN_HEIGHT - 40, 16, Colors::TEXT_DIM);
                break;
            }
            case TICTACTOE: ticTacToe.Draw(); break;
            case SPACE_SURVIVOR: spaceSurvivor.Draw(); break;
            case BREAK_CODE: breakTheCode.Draw(); break;
            case ZOMBIE_LAND: zombieLand.Draw(); break;
            case SWAP_CARDS: swapCards.Draw(); break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}