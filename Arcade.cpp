/*
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                        GAMING HUB ARCADE                                  ║
 * ║                    Professional C++ Console Game                          ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║  Games Included:                                                          ║
 * ║    1. Tic-Tac-Toe (2-Player)                                             ║
 * ║    2. Space Survivor (Real-time Dodge)                                   ║
 * ║    3. Break The Code (Mastermind)                                        ║
 * ║    4. Escape Zombie Land (Turn-based)                                    ║
 * ║    5. Swap Cards (Memory Match)                                          ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <algorithm>

using namespace std;

// ═══════════════════════════════════════════════════════════════════════════
// CONSOLE UTILITIES - Colors and UI Helpers
// ═══════════════════════════════════════════════════════════════════════════

namespace Console {
    enum Color {
        BLACK = 0, DARK_BLUE = 1, DARK_GREEN = 2, DARK_CYAN = 3,
        DARK_RED = 4, DARK_MAGENTA = 5, DARK_YELLOW = 6, LIGHT_GRAY = 7,
        DARK_GRAY = 8, BLUE = 9, GREEN = 10, CYAN = 11,
        RED = 12, MAGENTA = 13, YELLOW = 14, WHITE = 15
    };

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    void setColor(int textColor, int bgColor = BLACK) {
        SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
    }

    void resetColor() { SetConsoleTextAttribute(hConsole, LIGHT_GRAY); }
    void clearScreen() { system("cls"); }

    void setCursor(int x, int y) {
        COORD coord = {(SHORT)x, (SHORT)y};
        SetConsoleCursorPosition(hConsole, coord);
    }

    void hideCursor() {
        CONSOLE_CURSOR_INFO info; GetConsoleCursorInfo(hConsole, &info);
        info.bVisible = FALSE; SetConsoleCursorInfo(hConsole, &info);
    }

    void showCursor() {
        CONSOLE_CURSOR_INFO info; GetConsoleCursorInfo(hConsole, &info);
        info.bVisible = TRUE; SetConsoleCursorInfo(hConsole, &info);
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// UI COMPONENTS - Professional Box Drawing
// ═══════════════════════════════════════════════════════════════════════════

namespace UI {
    const int BOX_WIDTH = 60;

    void drawTopBorder(string title = "") {
        Console::setColor(Console::CYAN);
        cout << " " << char(201);
        for (int i = 0; i < BOX_WIDTH - 2; i++) cout << char(205);
        cout << char(187) << "\n";

        if (!title.empty()) {
            cout << " " << char(186);
            Console::setColor(Console::YELLOW);
            int pad = (BOX_WIDTH - 2 - title.length()) / 2;
            cout << string(pad, ' ') << title << string(BOX_WIDTH - 2 - pad - title.length(), ' ');
            Console::setColor(Console::CYAN);
            cout << char(186) << "\n";
            cout << " " << char(204);
            for (int i = 0; i < BOX_WIDTH - 2; i++) cout << char(205);
            cout << char(185) << "\n";
        }
    }

    void drawBottomBorder() {
        Console::setColor(Console::CYAN);
        cout << " " << char(200);
        for (int i = 0; i < BOX_WIDTH - 2; i++) cout << char(205);
        cout << char(188) << "\n";
        Console::resetColor();
    }

    void drawLine(string text, int color = Console::WHITE) {
        Console::setColor(Console::CYAN);
        cout << " " << char(186) << " ";
        Console::setColor(color);
        cout << left << setw(BOX_WIDTH - 4) << text;
        Console::setColor(Console::CYAN);
        cout << char(186) << "\n";
    }

    void drawEmptyLine() {
        Console::setColor(Console::CYAN);
        cout << " " << char(186) << string(BOX_WIDTH - 2, ' ') << char(186) << "\n";
    }

    void drawSeparator() {
        Console::setColor(Console::CYAN);
        cout << " " << char(199);
        for (int i = 0; i < BOX_WIDTH - 2; i++) cout << char(196);
        cout << char(182) << "\n";
    }

    void printSuccess(string msg) {
        Console::setColor(Console::GREEN);
        cout << " [+] " << msg << "\n";
        Console::resetColor();
    }

    void printError(string msg) {
        Console::setColor(Console::RED);
        cout << " [!] " << msg << "\n";
        Console::resetColor();
    }

    void printInfo(string msg) {
        Console::setColor(Console::CYAN);
        cout << " [*] " << msg << "\n";
        Console::resetColor();
    }

    void pressEnter() {
        Console::setColor(Console::DARK_GRAY);
        cout << "\n Press Enter to continue...";
        Console::resetColor();
        cin.ignore(10000, '\n');
        cin.get();
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// GAME 1: TIC-TAC-TOE (2-Player)
// ═══════════════════════════════════════════════════════════════════════════

class TicTacToe {
private:
    vector<char> board;
    char currentPlayer;
    int moves;
    bool gameOver;
    bool exitToMenu;

    void drawBoard() {
        Console::clearScreen();
        UI::drawTopBorder("TIC-TAC-TOE");
        UI::drawLine("Player 1: X  |  Player 2: O", Console::WHITE);
        UI::drawLine("Press 'M' to return to menu", Console::DARK_GRAY);
        UI::drawSeparator();
        UI::drawEmptyLine();

        // Draw the game board with colors
        Console::setColor(Console::CYAN);
        cout << " " << char(186) << "          ";

        for (int row = 2; row >= 0; row--) {
            if (row < 2) {
                Console::setColor(Console::CYAN);
                cout << " " << char(186) << "          ";
            }

            for (int col = 0; col < 3; col++) {
                int idx = row * 3 + col;
                if (board[idx] == 'X') Console::setColor(Console::GREEN);
                else if (board[idx] == 'O') Console::setColor(Console::RED);
                else Console::setColor(Console::DARK_GRAY);

                cout << " " << board[idx] << " ";

                if (col < 2) {
                    Console::setColor(Console::WHITE);
                    cout << char(179);
                }
            }

            Console::setColor(Console::CYAN);
            cout << "                         " << char(186) << "\n";

            if (row > 0) {
                cout << " " << char(186) << "         ";
                Console::setColor(Console::WHITE);
                cout << char(196) << char(196) << char(196) << char(197);
                cout << char(196) << char(196) << char(196) << char(197);
                cout << char(196) << char(196) << char(196);
                Console::setColor(Console::CYAN);
                cout << "                        " << char(186) << "\n";
            }
        }

        UI::drawEmptyLine();
        UI::drawSeparator();

        // Position reference
        Console::setColor(Console::CYAN);
        cout << " " << char(186) << "   ";
        Console::setColor(Console::DARK_GRAY);
        cout << "Positions:  7|8|9    Current: ";
        Console::setColor(currentPlayer == 'X' ? Console::GREEN : Console::RED);
        cout << "Player " << currentPlayer;
        Console::setColor(Console::CYAN);
        cout << "            " << char(186) << "\n";

        cout << " " << char(186) << "   ";
        Console::setColor(Console::DARK_GRAY);
        cout << "            4|5|6";
        Console::setColor(Console::CYAN);
        cout << "                              " << char(186) << "\n";

        cout << " " << char(186) << "   ";
        Console::setColor(Console::DARK_GRAY);
        cout << "            1|2|3";
        Console::setColor(Console::CYAN);
        cout << "                              " << char(186) << "\n";

        UI::drawBottomBorder();
    }

    bool checkWin(char symbol) {
        vector<vector<int>> wins = {
            {0,1,2}, {3,4,5}, {6,7,8},
            {0,3,6}, {1,4,7}, {2,5,8},
            {0,4,8}, {2,4,6}
        };
        for (auto& w : wins) {
            if (board[w[0]] == symbol && board[w[1]] == symbol && board[w[2]] == symbol)
                return true;
        }
        return false;
    }

public:
    void run() {
        board = {'1','2','3','4','5','6','7','8','9'};
        currentPlayer = 'X';
        moves = 0;
        gameOver = false;
        exitToMenu = false;

        while (moves < 9 && !gameOver && !exitToMenu) {
            drawBoard();

            Console::setColor(Console::WHITE);
            cout << "\n Enter position (1-9): ";

            string input;
            if (!(cin >> input)) break;

            if (input == "M" || input == "m") {
                exitToMenu = true;
                break;
            }

            try {
                int choice = stoi(input);
                int idx = choice - 1;

                if (idx >= 0 && idx < 9 && board[idx] >= '1' && board[idx] <= '9') {
                    board[idx] = currentPlayer;

                    if (checkWin(currentPlayer)) {
                        gameOver = true;
                        drawBoard();
                        Console::setColor(Console::GREEN);
                        cout << "\n ";
                        for (int i = 0; i < 50; i++) cout << char(176);
                        cout << "\n   CONGRATULATIONS! Player " << currentPlayer << " WINS!\n ";
                        for (int i = 0; i < 50; i++) cout << char(176);
                        cout << "\n";
                    } else {
                        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                        moves++;
                    }
                } else {
                    UI::printError("Invalid move! Position taken or out of range.");
                    Sleep(1200);
                }
            } catch (...) {
                UI::printError("Please enter a number (1-9) or 'M' for menu.");
                Sleep(1200);
            }
        }

        if (moves == 9 && !gameOver && !exitToMenu) {
            drawBoard();
            Console::setColor(Console::YELLOW);
            cout << "\n   It's a DRAW! Good game!\n";
        }

        if (!exitToMenu) UI::pressEnter();
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// GAME 2: SPACE SURVIVOR (Real-time Dodge)
// ═══════════════════════════════════════════════════════════════════════════

class SpaceSurvivor {
private:
    static const int WIDTH = 30;
    static const int HEIGHT = 15;
    static const int MAX_ASTEROIDS = 12;

    int playerX;
    int score;
    bool gameOver;
    bool exitToMenu;

    struct Asteroid { int x, y; };
    Asteroid asteroids[MAX_ASTEROIDS];

    void draw() {
        Console::clearScreen();
        Console::hideCursor();

        UI::drawTopBorder("SPACE SURVIVOR");
        UI::drawLine("Dodge the asteroids! | Move: A/D | Menu: M", Console::WHITE);
        UI::drawSeparator();

        // Game field
        for (int i = 0; i < HEIGHT; i++) {
            Console::setColor(Console::CYAN);
            cout << " " << char(186) << " ";

            for (int j = 0; j < WIDTH; j++) {
                bool printed = false;

                // Check for asteroids
                for (int a = 0; a < MAX_ASTEROIDS; a++) {
                    if (asteroids[a].x == j && asteroids[a].y == i) {
                        Console::setColor(Console::RED);
                        cout << char(15);  // Asteroid symbol
                        printed = true;
                        break;
                    }
                }

                // Player ship
                if (!printed && i == HEIGHT - 1 && j == playerX) {
                    Console::setColor(Console::GREEN);
                    cout << char(30);  // Ship pointing up
                    printed = true;
                }

                // Empty space
                if (!printed) {
                    Console::setColor(Console::DARK_BLUE);
                    cout << ((rand() % 50 == 0) ? '.' : ' ');
                }
            }

            Console::setColor(Console::CYAN);
            cout << string(UI::BOX_WIDTH - WIDTH - 4, ' ') << char(186) << "\n";
        }

        UI::drawSeparator();

        // Score display
        Console::setColor(Console::CYAN);
        cout << " " << char(186) << "  ";
        Console::setColor(Console::YELLOW);
        cout << "SCORE: " << setw(5) << score;
        Console::setColor(Console::DARK_GRAY);
        cout << "  |  Avoid the " ;
        Console::setColor(Console::RED);
        cout << char(15);
        Console::setColor(Console::DARK_GRAY);
        cout << " asteroids!";
        Console::setColor(Console::CYAN);
        cout << "                   " << char(186) << "\n";

        UI::drawBottomBorder();
    }

    void handleInput() {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' || ch == 'A') playerX--;
            else if (ch == 'd' || ch == 'D') playerX++;
            else if (ch == 'm' || ch == 'M') exitToMenu = true;
        }
        if (playerX < 0) playerX = 0;
        if (playerX >= WIDTH) playerX = WIDTH - 1;
    }

    void updateGame() {
        for (int i = 0; i < MAX_ASTEROIDS; i++) {
            asteroids[i].y++;

            // Collision detection
            if (asteroids[i].y == HEIGHT - 1 && asteroids[i].x == playerX) {
                gameOver = true;
                return;
            }

            // Respawn asteroid
            if (asteroids[i].y >= HEIGHT) {
                asteroids[i].y = 0;
                asteroids[i].x = rand() % WIDTH;
                score += 10;
            }
        }
    }

public:
    void run() {
        playerX = WIDTH / 2;
        score = 0;
        gameOver = false;
        exitToMenu = false;

        // Initialize asteroids
        for (int i = 0; i < MAX_ASTEROIDS; i++) {
            asteroids[i].x = rand() % WIDTH;
            asteroids[i].y = rand() % HEIGHT;
        }

        // Instructions
        Console::clearScreen();
        UI::drawTopBorder("SPACE SURVIVOR");
        UI::drawEmptyLine();
        UI::drawLine("OBJECTIVE:", Console::YELLOW);
        UI::drawLine("  Dodge the falling asteroids with your spaceship!", Console::WHITE);
        UI::drawEmptyLine();
        UI::drawLine("CONTROLS:", Console::YELLOW);
        UI::drawLine("  A - Move Left    D - Move Right    M - Menu", Console::WHITE);
        UI::drawEmptyLine();
        UI::drawLine("Press any key to start...", Console::GREEN);
        UI::drawBottomBorder();
        _getch();

        while (!gameOver && !exitToMenu) {
            draw();
            handleInput();
            if (exitToMenu) break;
            updateGame();
            Sleep(80);
        }

        Console::showCursor();

        if (gameOver && !exitToMenu) {
            draw();
            Console::setColor(Console::RED);
            cout << "\n ";
            for (int i = 0; i < 50; i++) cout << char(177);
            cout << "\n      CRASH! You hit an asteroid!\n";
            cout << "      Final Score: " << score << "\n ";
            for (int i = 0; i < 50; i++) cout << char(177);
            cout << "\n";
            Console::resetColor();
            UI::pressEnter();
        }
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// GAME 3: BREAK THE CODE (Mastermind)
// ═══════════════════════════════════════════════════════════════════════════

class BreakTheCode {
private:
    string secretCode;
    string playerName;
    int maxAttempts;
    bool exitToMenu;

    string generateCode() {
        string code = "";
        bool used[10] = {false};
        while (code.length() < 4) {
            int digit = rand() % 10;
            if (!used[digit]) {
                used[digit] = true;
                code += to_string(digit);
            }
        }
        return code;
    }

    void checkGuess(const string& guess, int& correctPos, int& correctDigit) {
        correctPos = correctDigit = 0;
        bool secretUsed[4] = {false}, guessUsed[4] = {false};

        for (int i = 0; i < 4; i++) {
            if (guess[i] == secretCode[i]) {
                correctPos++;
                secretUsed[i] = guessUsed[i] = true;
            }
        }

        for (int i = 0; i < 4; i++) {
            if (!guessUsed[i]) {
                for (int j = 0; j < 4; j++) {
                    if (!secretUsed[j] && guess[i] == secretCode[j]) {
                        correctDigit++;
                        secretUsed[j] = true;
                        break;
                    }
                }
            }
        }
    }

    void saveScore(int attempts) {
        ofstream file("codebreaker_leaderboard.txt", ios::app);
        if (file.is_open()) {
            file << playerName << " cracked the code in " << attempts << " attempts.\n";
            file.close();
        }
    }

    void showLeaderboard() {
        Console::setColor(Console::CYAN);
        cout << "\n " << string(50, char(196)) << "\n";
        Console::setColor(Console::YELLOW);
        cout << "   LEADERBOARD\n";
        Console::setColor(Console::CYAN);
        cout << " " << string(50, char(196)) << "\n";

        ifstream file("codebreaker_leaderboard.txt");
        string line;
        int count = 0;
        if (file.is_open()) {
            while (getline(file, line) && count < 5) {
                Console::setColor(Console::WHITE);
                cout << "   " << line << "\n";
                count++;
            }
            file.close();
        }
        if (count == 0) {
            Console::setColor(Console::DARK_GRAY);
            cout << "   No records yet.\n";
        }
        Console::setColor(Console::CYAN);
        cout << " " << string(50, char(196)) << "\n";
    }

public:
    void run() {
        Console::clearScreen();
        UI::drawTopBorder("BREAK THE CODE");
        UI::drawEmptyLine();
        UI::drawLine("Crack the 4-digit secret code!", Console::WHITE);
        UI::drawLine("Each digit is unique (0-9).", Console::DARK_GRAY);
        UI::drawEmptyLine();
        UI::drawBottomBorder();

        Console::setColor(Console::WHITE);
        cout << "\n Enter your name: ";
        cin.ignore(10000, '\n');
        getline(cin, playerName);

        secretCode = generateCode();
        maxAttempts = 10;
        exitToMenu = false;
        int turn = 0;
        bool won = false;

        Console::clearScreen();
        UI::drawTopBorder("BREAK THE CODE");
        UI::drawLine("A 4-digit code has been generated!", Console::GREEN);
        UI::drawLine("You have 10 attempts. Enter 'M' to quit.", Console::DARK_GRAY);
        UI::drawSeparator();
        UI::drawEmptyLine();

        while (turn < maxAttempts && !exitToMenu && !won) {
            Console::setColor(Console::WHITE);
            cout << " Attempt " << (turn + 1) << "/" << maxAttempts << " | Enter guess: ";

            string input;
            if (!(cin >> input)) break;

            if (input == "M" || input == "m") {
                exitToMenu = true;
                break;
            }

            // Validate
            if (input.length() != 4) {
                UI::printError("Enter exactly 4 digits!");
                continue;
            }
            bool valid = true;
            for (char c : input) if (!isdigit(c)) valid = false;
            if (!valid) {
                UI::printError("Only digits allowed!");
                continue;
            }

            int correctPos, correctDigit;
            checkGuess(input, correctPos, correctDigit);

            if (correctPos == 4) {
                won = true;
                Console::setColor(Console::GREEN);
                cout << "\n " << string(50, char(219)) << "\n";
                cout << "   CONGRATULATIONS " << playerName << "!\n";
                cout << "   You cracked the code: " << secretCode << "\n";
                cout << "   Attempts used: " << (turn + 1) << "\n";
                cout << " " << string(50, char(219)) << "\n";
                saveScore(turn + 1);
            } else {
                Console::setColor(Console::CYAN);
                cout << "   Result: ";
                Console::setColor(Console::GREEN);
                cout << correctPos << " correct position(s), ";
                Console::setColor(Console::YELLOW);
                cout << correctDigit << " wrong position(s)\n\n";
            }
            turn++;
        }

        if (!won && !exitToMenu) {
            Console::setColor(Console::RED);
            cout << "\n   Out of attempts! The code was: " << secretCode << "\n";
        }

        if (!exitToMenu) {
            showLeaderboard();
            UI::pressEnter();
        }
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// GAME 4: ESCAPE ZOMBIE LAND (Turn-based)
// ═══════════════════════════════════════════════════════════════════════════

class EscapeZombieLand {
private:
    static const int GRID_SIZE = 10;
    char grid[GRID_SIZE][GRID_SIZE];
    int playerX, playerY;
    int zombieX, zombieY;
    int safeX, safeY;
    string playerName;
    bool exitToMenu;

    void initGrid() {
        for (int i = 0; i < GRID_SIZE; i++)
            for (int j = 0; j < GRID_SIZE; j++)
                grid[i][j] = '.';

        playerX = playerY = 0;
        safeX = safeY = GRID_SIZE - 1;
        zombieX = zombieY = GRID_SIZE / 2;

        grid[playerX][playerY] = 'P';
        grid[safeX][safeY] = 'S';
        grid[zombieX][zombieY] = 'Z';
        exitToMenu = false;
    }

    void displayGrid() {
        Console::clearScreen();
        UI::drawTopBorder("ESCAPE ZOMBIE LAND");
        UI::drawLine("Reach the Safe Zone [S] | Avoid the Zombie [Z]", Console::WHITE);
        UI::drawLine("Move: W/A/S/D | Menu: M", Console::DARK_GRAY);
        UI::drawSeparator();

        // Column headers
        Console::setColor(Console::CYAN);
        cout << " " << char(186) << "      ";
        Console::setColor(Console::DARK_GRAY);
        for (int j = 0; j < GRID_SIZE; j++) cout << j << " ";
        Console::setColor(Console::CYAN);
        cout << "                          " << char(186) << "\n";

        // Grid
        for (int i = 0; i < GRID_SIZE; i++) {
            Console::setColor(Console::CYAN);
            cout << " " << char(186) << "    ";
            Console::setColor(Console::DARK_GRAY);
            cout << i << " ";

            for (int j = 0; j < GRID_SIZE; j++) {
                char cell = grid[i][j];
                if (cell == 'P') Console::setColor(Console::GREEN);
                else if (cell == 'Z') Console::setColor(Console::RED);
                else if (cell == 'S') Console::setColor(Console::YELLOW);
                else Console::setColor(Console::DARK_GRAY);

                cout << cell << " ";
            }

            Console::setColor(Console::CYAN);
            cout << "                        " << char(186) << "\n";
        }

        UI::drawSeparator();
        Console::setColor(Console::CYAN);
        cout << " " << char(186) << "  ";
        Console::setColor(Console::GREEN);
        cout << "P(" << playerX << "," << playerY << ")";
        Console::setColor(Console::DARK_GRAY);
        cout << " You  ";
        Console::setColor(Console::RED);
        cout << "Z(" << zombieX << "," << zombieY << ")";
        Console::setColor(Console::DARK_GRAY);
        cout << " Zombie  ";
        Console::setColor(Console::YELLOW);
        cout << "S(" << safeX << "," << safeY << ")";
        Console::setColor(Console::DARK_GRAY);
        cout << " Safe";
        Console::setColor(Console::CYAN);
        cout << "   " << char(186) << "\n";
        UI::drawBottomBorder();
    }

    void movePlayer(char move) {
        grid[playerX][playerY] = '.';
        int newX = playerX, newY = playerY;

        if (move == 'w' || move == 'W') newX--;
        else if (move == 's' || move == 'S') newX++;
        else if (move == 'a' || move == 'A') newY--;
        else if (move == 'd' || move == 'D') newY++;
        else if (move == 'm' || move == 'M') { exitToMenu = true; return; }

        if (newX >= 0 && newX < GRID_SIZE) playerX = newX;
        if (newY >= 0 && newY < GRID_SIZE) playerY = newY;

        grid[playerX][playerY] = 'P';
        if (grid[safeX][safeY] == '.') grid[safeX][safeY] = 'S';
    }

    void moveZombie() {
        if (grid[zombieX][zombieY] == 'Z') grid[zombieX][zombieY] = '.';

        int dir = rand() % 4;
        int newX = zombieX, newY = zombieY;

        if (dir == 0) newX--;
        else if (dir == 1) newX++;
        else if (dir == 2) newY--;
        else newY++;

        if (newX >= 0 && newX < GRID_SIZE) zombieX = newX;
        if (newY >= 0 && newY < GRID_SIZE) zombieY = newY;

        if (zombieX != safeX || zombieY != safeY)
            grid[zombieX][zombieY] = 'Z';
    }

    bool checkWin() { return playerX == safeX && playerY == safeY; }
    bool checkLose() { return playerX == zombieX && playerY == zombieY; }

    void saveResult(string result) {
        ofstream file("zombie_escape_results.txt", ios::app);
        if (file.is_open()) {
            file << playerName << " - " << result << " | " << time(0) << "\n";
            file.close();
        }
    }

public:
    void run() {
        Console::clearScreen();
        UI::drawTopBorder("ESCAPE ZOMBIE LAND");
        UI::drawEmptyLine();
        UI::drawLine("Navigate to the Safe Zone while avoiding zombies!", Console::WHITE);
        UI::drawEmptyLine();
        UI::drawBottomBorder();

        Console::setColor(Console::WHITE);
        cout << "\n Enter your name: ";
        cin.ignore(10000, '\n');
        getline(cin, playerName);

        initGrid();
        bool gameOver = false;

        while (!gameOver && !exitToMenu) {
            displayGrid();
            char move = _getch();
            movePlayer(move);
            if (exitToMenu) break;
            moveZombie();

            if (checkWin()) {
                displayGrid();
                Console::setColor(Console::GREEN);
                cout << "\n " << string(50, char(219)) << "\n";
                cout << "   YOU ESCAPED! " << playerName << " SURVIVED!\n";
                cout << " " << string(50, char(219)) << "\n";
                saveResult("Survived");
                gameOver = true;
            } else if (checkLose()) {
                displayGrid();
                Console::setColor(Console::RED);
                cout << "\n " << string(50, char(178)) << "\n";
                cout << "   CAUGHT BY ZOMBIE! Game Over.\n";
                cout << " " << string(50, char(178)) << "\n";
                saveResult("Caught");
                gameOver = true;
            }
            Sleep(50);
        }

        if (!exitToMenu) {
            UI::printInfo("Result saved to 'zombie_escape_results.txt'");
            UI::pressEnter();
        }
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// GAME 5: SWAP CARDS (Memory Match)
// ═══════════════════════════════════════════════════════════════════════════

class SwapCards {
private:
    static const int GRID_SIZE = 4;
    char board[GRID_SIZE][GRID_SIZE];
    bool revealed[GRID_SIZE][GRID_SIZE];
    int totalMatches, moves;
    bool exitToMenu;

    void shuffleBoard() {
        char symbols[8] = {'A','B','C','D','E','F','G','H'};
        char temp[16];

        for (int i = 0; i < 8; i++) {
            temp[i * 2] = symbols[i];
            temp[i * 2 + 1] = symbols[i];
        }

        for (int i = 0; i < 16; i++) {
            int r = rand() % 16;
            swap(temp[i], temp[r]);
        }

        int idx = 0;
        for (int i = 0; i < GRID_SIZE; i++)
            for (int j = 0; j < GRID_SIZE; j++)
                board[i][j] = temp[idx++];
    }

    void displayBoard() {
        Console::clearScreen();
        UI::drawTopBorder("SWAP CARDS - Memory Match");
        UI::drawLine("Match all 8 pairs of cards (A-H)", Console::WHITE);
        UI::drawLine("Enter 0 to return to menu", Console::DARK_GRAY);
        UI::drawSeparator();
        UI::drawEmptyLine();

        int num = 1;
        for (int i = 0; i < GRID_SIZE; i++) {
            Console::setColor(Console::CYAN);
            cout << " " << char(186) << "       ";

            for (int j = 0; j < GRID_SIZE; j++) {
                cout << char(218) << char(196) << char(196) << char(196) << char(191) << " ";
            }
            Console::setColor(Console::CYAN);
            cout << "                " << char(186) << "\n";

            cout << " " << char(186) << "       ";
            for (int j = 0; j < GRID_SIZE; j++) {
                cout << char(179) << " ";
                if (revealed[i][j]) {
                    Console::setColor(Console::YELLOW);
                    cout << board[i][j];
                } else {
                    Console::setColor(Console::DARK_GRAY);
                    if (num < 10) cout << num;
                    else cout << (char)('A' + num - 10);
                }
                Console::setColor(Console::CYAN);
                cout << " " << char(179) << " ";
                num++;
            }
            cout << "                " << char(186) << "\n";

            cout << " " << char(186) << "       ";
            for (int j = 0; j < GRID_SIZE; j++) {
                cout << char(192) << char(196) << char(196) << char(196) << char(217) << " ";
            }
            cout << "                " << char(186) << "\n";
        }

        UI::drawSeparator();
        Console::setColor(Console::CYAN);
        cout << " " << char(186) << "  ";
        Console::setColor(Console::WHITE);
        cout << "Moves: " << moves << "  |  Matches: " << totalMatches << "/8";
        Console::setColor(Console::CYAN);
        cout << "                           " << char(186) << "\n";
        UI::drawBottomBorder();
    }

    void getPosition(int pos, int& row, int& col) {
        pos--;
        row = pos / GRID_SIZE;
        col = pos % GRID_SIZE;
    }

    void saveScore() {
        ofstream file("swapcards_best_score.txt", ios::app);
        if (file.is_open()) {
            file << "Completed in " << moves << " moves | " << time(0) << "\n";
            file.close();
        }
    }

public:
    void run() {
        shuffleBoard();
        for (int i = 0; i < GRID_SIZE; i++)
            for (int j = 0; j < GRID_SIZE; j++)
                revealed[i][j] = false;

        totalMatches = moves = 0;
        exitToMenu = false;

        Console::clearScreen();
        UI::drawTopBorder("SWAP CARDS - Memory Match");
        UI::drawEmptyLine();
        UI::drawLine("Match all 8 pairs by remembering card positions!", Console::WHITE);
        UI::drawLine("Enter card numbers to flip them.", Console::DARK_GRAY);
        UI::drawEmptyLine();
        UI::drawLine("Press Enter to start...", Console::GREEN);
        UI::drawBottomBorder();
        cin.ignore(10000, '\n');
        cin.get();

        while (totalMatches < 8 && !exitToMenu) {
            displayBoard();

            int pos1, pos2;
            Console::setColor(Console::WHITE);
            cout << "\n First card (1-16, 0=exit): ";
            if (!(cin >> pos1)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
            if (pos1 == 0) { exitToMenu = true; break; }

            cout << " Second card (1-16, 0=exit): ";
            if (!(cin >> pos2)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
            if (pos2 == 0) { exitToMenu = true; break; }

            int r1, c1, r2, c2;
            getPosition(pos1, r1, c1);
            getPosition(pos2, r2, c2);

            // Validate
            if (pos1 < 1 || pos1 > 16 || pos2 < 1 || pos2 > 16) {
                UI::printError("Choose positions 1-16!");
                Sleep(1200); continue;
            }
            if (revealed[r1][c1] || revealed[r2][c2] || pos1 == pos2) {
                UI::printError("Invalid selection!");
                Sleep(1200); continue;
            }

            revealed[r1][c1] = revealed[r2][c2] = true;
            moves++;
            displayBoard();

            if (board[r1][c1] == board[r2][c2]) {
                UI::printSuccess("MATCH FOUND!");
                totalMatches++;
                Sleep(1000);
            } else {
                UI::printError("Not a match!");
                Sleep(1500);
                revealed[r1][c1] = revealed[r2][c2] = false;
            }
        }

        if (totalMatches == 8) {
            displayBoard();
            Console::setColor(Console::GREEN);
            cout << "\n " << string(50, char(219)) << "\n";
            cout << "   CONGRATULATIONS! All pairs matched!\n";
            cout << "   Total moves: " << moves << "\n";
            cout << " " << string(50, char(219)) << "\n";
            saveScore();
        }

        if (!exitToMenu) UI::pressEnter();
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// MAIN MENU SYSTEM
// ═══════════════════════════════════════════════════════════════════════════

class ArcadeMenu {
public:
    void displayMenu() {
        Console::clearScreen();
        Console::setColor(Console::CYAN);

        // ASCII Art Header
        cout << "\n";
        cout << "   " << char(201);
        for (int i = 0; i < 54; i++) cout << char(205);
        cout << char(187) << "\n";

        cout << "   " << char(186);
        Console::setColor(Console::YELLOW);
        cout << "   ____    _    __  __ ___ _   _  ____   _   _ _   _ ____  ";
        Console::setColor(Console::CYAN);
        cout << char(186) << "\n";

        cout << "   " << char(186);
        Console::setColor(Console::YELLOW);
        cout << "  / ___|  / \\  |  \\/  |_ _| \\ | |/ ___| | | | | | | | __ ) ";
        Console::setColor(Console::CYAN);
        cout << char(186) << "\n";

        cout << "   " << char(186);
        Console::setColor(Console::YELLOW);
        cout << " | |  _  / _ \\ | |\\/| || ||  \\| | |  _  | |_| | | | |  _ \\ ";
        Console::setColor(Console::CYAN);
        cout << char(186) << "\n";

        cout << "   " << char(186);
        Console::setColor(Console::YELLOW);
        cout << " | |_| |/ ___ \\| |  | || || |\\  | |_| | |  _  | |_| | |_) |";
        Console::setColor(Console::CYAN);
        cout << char(186) << "\n";

        cout << "   " << char(186);
        Console::setColor(Console::YELLOW);
        cout << "  \\____/_/   \\_\\_|  |_|___|_| \\_|\\____| |_| |_|\\___/|____/ ";
        Console::setColor(Console::CYAN);
        cout << char(186) << "\n";

        cout << "   " << char(186);
        Console::setColor(Console::DARK_GRAY);
        cout << "                    Professional C++ Arcade                ";
        Console::setColor(Console::CYAN);
        cout << char(186) << "\n";

        cout << "   " << char(204);
        for (int i = 0; i < 54; i++) cout << char(205);
        cout << char(185) << "\n";

        // Menu Options
        struct MenuItem { string num; string name; int color; };
        MenuItem items[] = {
            {"1", "Tic-Tac-Toe", Console::GREEN},
            {"2", "Space Survivor", Console::CYAN},
            {"3", "Break The Code", Console::YELLOW},
            {"4", "Escape Zombie Land", Console::RED},
            {"5", "Swap Cards", Console::MAGENTA},
            {"6", "Exit Arcade", Console::DARK_GRAY}
        };

        for (int i = 0; i < 6; i++) {
            cout << "   " << char(186) << "    ";
            Console::setColor(Console::WHITE);
            cout << "[" << items[i].num << "] ";
            Console::setColor(items[i].color);
            cout << left << setw(20) << items[i].name;

            // Add game type description
            Console::setColor(Console::DARK_GRAY);
            if (i == 0) cout << "(2-Player Strategy)";
            else if (i == 1) cout << "(Real-time Dodge)";
            else if (i == 2) cout << "(Mastermind Puzzle)";
            else if (i == 3) cout << "(Turn-based Escape)";
            else if (i == 4) cout << "(Memory Challenge)";
            else cout << "";

            Console::setColor(Console::CYAN);
            cout << "       " << char(186) << "\n";
        }

        cout << "   " << char(200);
        for (int i = 0; i < 54; i++) cout << char(205);
        cout << char(188) << "\n";

        Console::setColor(Console::WHITE);
        cout << "\n   Enter your choice (1-6): ";
    }

    void run() {
        int choice = -1;
        srand(time(0));

        while (choice != 6) {
            displayMenu();

            if (!(cin >> choice)) {
                UI::printError("Invalid input! Enter a number 1-6.");
                cin.clear();
                cin.ignore(10000, '\n');
                choice = -1;
                Sleep(1500);
                continue;
            }

            switch (choice) {
                case 1: { TicTacToe game; game.run(); break; }
                case 2: { SpaceSurvivor game; game.run(); break; }
                case 3: { BreakTheCode game; game.run(); break; }
                case 4: { EscapeZombieLand game; game.run(); break; }
                case 5: { SwapCards game; game.run(); break; }
                case 6: {
                    Console::clearScreen();
                    Console::setColor(Console::CYAN);
                    cout << "\n\n   " << string(50, char(205)) << "\n";
                    Console::setColor(Console::YELLOW);
                    cout << "   Thank you for playing GAMING HUB!\n";
                    Console::setColor(Console::DARK_GRAY);
                    cout << "   See you next time!\n";
                    Console::setColor(Console::CYAN);
                    cout << "   " << string(50, char(205)) << "\n\n";
                    Console::resetColor();
                    break;
                }
                default:
                    UI::printError("Invalid choice! Select 1-6.");
                    Sleep(1500);
                    break;
            }
        }
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// MAIN ENTRY POINT
// ═══════════════════════════════════════════════════════════════════════════

int main() {
    // Set console title
    SetConsoleTitleA("GAMING HUB - Professional C++ Arcade");

    // Run the arcade
    ArcadeMenu arcade;
    arcade.run();

    return 0;
}
