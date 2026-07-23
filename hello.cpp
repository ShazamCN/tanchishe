#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

class SnakeGame {
private:
    bool gameOver;
    int score;
    int headX, headY;
    int foodX, foodY;
    Direction dir;
    vector<pair<int, int>> tail;

public:
    SnakeGame() {
        gameOver = false;
        score = 0;
        dir = STOP;
        headX = WIDTH / 2;
        headY = HEIGHT / 2;
        srand(static_cast<unsigned>(time(nullptr)));
        spawnFood();
    }

    void spawnFood() {
        foodX = rand() % (WIDTH - 2) + 1;
        foodY = rand() % HEIGHT;
    }

    void draw() {
        // Move cursor to top-left to avoid flickering
        COORD cursorPos = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);

        // Top wall
        for (int i = 0; i < WIDTH; i++) cout << "#";
        cout << endl;

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (x == 0 || x == WIDTH - 1) {
                    cout << "#";
                } else if (x == headX && y == headY) {
                    cout << "O";
                } else if (x == foodX && y == foodY) {
                    cout << "@";
                } else {
                    bool isTail = false;
                    for (auto& t : tail) {
                        if (t.first == x && t.second == y) {
                            cout << "o";
                            isTail = true;
                            break;
                        }
                    }
                    if (!isTail) cout << " ";
                }
            }
            cout << endl;
        }

        // Bottom wall
        for (int i = 0; i < WIDTH; i++) cout << "#";
        cout << endl;

        cout << "Score: " << score << "    \n";
        cout << "WASD / Arrow keys to move | X to quit" << "    \n";
    }

    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'a': case 'A': case 75:
                    if (dir != RIGHT) dir = LEFT;
                    break;
                case 'd': case 'D': case 77:
                    if (dir != LEFT) dir = RIGHT;
                    break;
                case 'w': case 'W': case 72:
                    if (dir != DOWN) dir = UP;
                    break;
                case 's': case 'S': case 80:
                    if (dir != UP) dir = DOWN;
                    break;
                case 'x': case 'X':
                    gameOver = true;
                    break;
            }
        }
    }

    void logic() {
        if (dir == STOP) return;

        int prevX = headX, prevY = headY;

        switch (dir) {
            case LEFT:  headX--; break;
            case RIGHT: headX++; break;
            case UP:    headY--; break;
            case DOWN:  headY++; break;
        }

        // Wall collision
        if (headX <= 0 || headX >= WIDTH - 1 || headY < 0 || headY >= HEIGHT) {
            gameOver = true;
            return;
        }

        // Self collision
        for (auto& t : tail) {
            if (t.first == headX && t.second == headY) {
                gameOver = true;
                return;
            }
        }

        // Check food
        if (headX == foodX && headY == foodY) {
            // Grow: add segment at previous head position
            tail.insert(tail.begin(), {prevX, prevY});
            score += 10;
            spawnFood();
        } else if (!tail.empty()) {
            // Move tail forward
            for (size_t i = tail.size() - 1; i > 0; i--) {
                tail[i] = tail[i - 1];
            }
            tail[0] = {prevX, prevY};
        }
    }

    void reset() {
        gameOver = false;
        score = 0;
        dir = STOP;
        tail.clear();
        headX = WIDTH / 2;
        headY = HEIGHT / 2;
        spawnFood();
    }

    void run() {
        // Hide cursor
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

        char ch;
        do {
            reset();

            while (!gameOver) {
                draw();
                input();
                logic();
                Sleep(150);
            }

            // Game over screen
            system("cls");
            cout << "####################\n";
            cout << "#                  #\n";
            cout << "#    GAME OVER!    #\n";
            cout << "#                  #\n";
            cout << "#   Score: " << score << "    #\n";
            cout << "#                  #\n";
            cout << "####################\n";
            cout << "\nPress R to restart or any other key to quit...\n";

            ch = _getch();
        } while (ch == 'r' || ch == 'R');

        // Show cursor again
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}