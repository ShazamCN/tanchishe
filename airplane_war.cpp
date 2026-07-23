#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 30;
const int HEIGHT = 25;
const int MAX_ENEMIES = 8;
const int MAX_BULLETS = 20;

// 游戏元素符号
const char PLAYER_CHAR = '^';
const char BULLET_CHAR = '|';
const char ENEMY_CHAR = 'V';
const char ENEMY_BULLET_CHAR = '*';

class AirplaneWar {
private:
    bool gameOver;
    int score;
    int lives;
    int level;
    int frameCount;

    // 玩家坐标
    int playerX, playerY;

    // 子弹
    struct Bullet {
        int x, y;
        bool active;
    };
    Bullet bullets[MAX_BULLETS];

    // 敌人
    struct Enemy {
        int x, y;
        bool active;
        int speed;       // 移动速度计数器
        int counter;
        char symbol;     // 不同敌人不同外观
    };
    Enemy enemies[MAX_ENEMIES];

    // 敌机子弹（降低难度的惩罚射击）
    struct EnemyBullet {
        int x, y;
        bool active;
    };
    EnemyBullet enemyBullets[10];

    int enemySpawnCounter;
    int enemySpawnRate;   // 越小生成越快

public:
    AirplaneWar() {
        reset();
    }

    void reset() {
        gameOver = false;
        score = 0;
        lives = 3;
        level = 1;
        frameCount = 0;
        playerX = WIDTH / 2;
        playerY = HEIGHT - 2;
        enemySpawnCounter = 0;
        enemySpawnRate = 30;

        // 初始化子弹
        for (int i = 0; i < MAX_BULLETS; i++) {
            bullets[i].active = false;
        }

        // 初始化敌人
        for (int i = 0; i < MAX_ENEMIES; i++) {
            enemies[i].active = false;
        }

        // 初始化敌机子弹
        for (int i = 0; i < 10; i++) {
            enemyBullets[i].active = false;
        }

        srand(static_cast<unsigned>(time(nullptr)));
    }

    void spawnBullet() {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) {
                bullets[i].x = playerX;
                bullets[i].y = playerY - 1;
                bullets[i].active = true;
                break;
            }
        }
    }

    void spawnEnemy() {
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemies[i].active) {
                enemies[i].x = rand() % (WIDTH - 4) + 2;
                enemies[i].y = 1;
                enemies[i].active = true;
                enemies[i].speed = 1 + rand() % 3;
                enemies[i].counter = 0;
                // 随机敌人外观
                int r = rand() % 3;
                enemies[i].symbol = (r == 0) ? 'V' : (r == 1) ? 'W' : 'M';
                break;
            }
        }
    }

    void spawnEnemyBullet(int x, int y) {
        for (int i = 0; i < 10; i++) {
            if (!enemyBullets[i].active) {
                enemyBullets[i].x = x;
                enemyBullets[i].y = y + 1;
                enemyBullets[i].active = true;
                break;
            }
        }
    }

    void draw() {
        // 隐藏光标已在 run() 中设置
        COORD cursorPos = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);

        // 顶部墙壁
        cout << "+";
        for (int i = 0; i < WIDTH - 2; i++) cout << "-";
        cout << "+\n";

        // 绘制游戏区域
        for (int y = 0; y < HEIGHT; y++) {
            cout << "|"; // 左墙
            for (int x = 0; x < WIDTH - 2; x++) {
                char ch = ' '; // 默认空格

                // 绘制玩家
                if (x == playerX && y == playerY) {
                    ch = PLAYER_CHAR;
                }
                // 绘制子弹
                else {
                    for (auto& b : bullets) {
                        if (b.active && b.x == x && b.y == y) {
                            ch = BULLET_CHAR;
                            break;
                        }
                    }
                }
                // 绘制敌人（覆盖优先级高于子弹）
                if (ch != PLAYER_CHAR) {
                    for (auto& e : enemies) {
                        if (e.active && e.x == x && e.y == y) {
                            ch = e.symbol;
                            break;
                        }
                    }
                }
                // 绘制敌机子弹
                if (ch == ' ') {
                    for (auto& eb : enemyBullets) {
                        if (eb.active && eb.x == x && eb.y == y) {
                            ch = ENEMY_BULLET_CHAR;
                            break;
                        }
                    }
                }

                cout << ch;
            }
            cout << "|\n"; // 右墙
        }

        // 底部墙壁
        cout << "+";
        for (int i = 0; i < WIDTH - 2; i++) cout << "-";
        cout << "+\n";

        // 信息面板
        cout << "Score: " << score << "  Lives: ";
        for (int i = 0; i < lives; i++) cout << (char)3 << " "; // 心形符号
        cout << "  Level: " << level << "    \n";
        cout << "A/D or Left/Right: Move  Space: Shoot  X: Quit    \n";
    }

    void input() {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) { // 方向键前缀
                key = _getch();
                switch (key) {
                    case 75: // Left
                        if (playerX > 1) playerX--;
                        break;
                    case 77: // Right
                        if (playerX < WIDTH - 3) playerX++;
                        break;
                }
            } else {
                switch (key) {
                    case 'a': case 'A':
                        if (playerX > 1) playerX--;
                        break;
                    case 'd': case 'D':
                        if (playerX < WIDTH - 3) playerX++;
                        break;
                    case ' ': // Space
                        spawnBullet();
                        break;
                    case 'x': case 'X':
                        gameOver = true;
                        break;
                }
            }
        }
    }

    void updateBullets() {
        for (auto& b : bullets) {
            if (b.active) {
                b.y--;
                if (b.y < 0) {
                    b.active = false;
                }
            }
        }
    }

    void updateEnemies() {
        for (auto& e : enemies) {
            if (e.active) {
                e.counter++;
                if (e.counter >= e.speed) {
                    e.counter = 0;
                    e.y++;
                }
                // 敌人到达底部
                if (e.y >= HEIGHT - 1) {
                    e.active = false;
                }
            }
        }
    }

    void updateEnemyBullets() {
        for (auto& eb : enemyBullets) {
            if (eb.active) {
                eb.y++;
                if (eb.y >= HEIGHT) {
                    eb.active = false;
                }
            }
        }
    }

    void checkCollisions() {
        // 子弹击中敌人
        for (auto& b : bullets) {
            if (!b.active) continue;
            for (auto& e : enemies) {
                if (!e.active) continue;
                if (b.x == e.x && b.y == e.y) {
                    b.active = false;
                    e.active = false;
                    score += 10;

                    // 每击毁5个敌机升级
                    if (score % 50 == 0) {
                        level++;
                        if (enemySpawnRate > 10) enemySpawnRate -= 2;
                    }
                    break;
                }
            }
        }

        // 玩家被敌人撞到
        for (auto& e : enemies) {
            if (!e.active) continue;
            if (e.x == playerX && e.y == playerY) {
                e.active = false;
                lives--;
                if (lives <= 0) {
                    gameOver = true;
                }
                break;
            }
        }

        // 玩家被敌机子弹击中
        for (auto& eb : enemyBullets) {
            if (!eb.active) continue;
            if (eb.x == playerX && eb.y == playerY) {
                eb.active = false;
                lives--;
                if (lives <= 0) {
                    gameOver = true;
                }
                break;
            }
        }

        // 敌人到达底部 -> 扣命
        for (auto& e : enemies) {
            if (e.active && e.y >= HEIGHT - 1) {
                e.active = false;
                lives--;
                if (lives <= 0) {
                    gameOver = true;
                }
            }
        }
    }

    void enemyShoot() {
        // 随机让敌机开火
        for (auto& e : enemies) {
            if (!e.active) continue;
            // 敌人有概率开火，随等级提升概率增加
            int chance = max(1, 8 - level);
            if (rand() % chance == 0) {
                spawnEnemyBullet(e.x, e.y);
            }
        }
    }

    void logic() {
        if (gameOver) return;

        frameCount++;

        // 更新子弹
        updateBullets();

        // 更新敌人
        updateEnemies();

        // 更新敌机子弹
        updateEnemyBullets();

        // 生成敌人
        enemySpawnCounter++;
        int spawnRate = max(10, enemySpawnRate - level * 2);
        if (enemySpawnCounter >= spawnRate) {
            enemySpawnCounter = 0;
            // 数一下当前活跃敌人数量
            int activeCount = 0;
            for (auto& e : enemies) {
                if (e.active) activeCount++;
            }
            if (activeCount < MAX_ENEMIES) {
                spawnEnemy();
            }
        }

        // 敌机开火
        if (frameCount % 15 == 0) {
            enemyShoot();
        }

        // 碰撞检测
        checkCollisions();
    }

    void run() {
        // 隐藏光标
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

        // 设置控制台标题
        SetConsoleTitle(TEXT("经典飞机大战"));

        char ch;
        do {
            reset();

            while (!gameOver) {
                draw();
                input();
                logic();
                Sleep(80);
            }

            // Game over 画面
            system("cls");
            cout << "+------------------------------+\n";
            cout << "|                              |\n";
            cout << "|         GAME OVER!           |\n";
            cout << "|                              |\n";
            cout << "|      Score: " << score;
            // 对齐格式
            int scoreLen = to_string(score).length();
            for (int i = 0; i < 6 - scoreLen; i++) cout << " ";
            cout << "       |\n";
            cout << "|      Level: " << level;
            int levelLen = to_string(level).length();
            for (int i = 0; i < 6 - levelLen; i++) cout << " ";
            cout << "       |\n";
            cout << "|                              |\n";
            cout << "+------------------------------+\n";
            cout << "\nPress R to restart or any other key to quit...\n";

            ch = _getch();
        } while (ch == 'r' || ch == 'R');

        // 显示光标
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

        system("cls");
        cout << "Thanks for playing! Final Score: " << score << endl;
    }
};

int main() {
    AirplaneWar game;
    game.run();
    return 0;
}
