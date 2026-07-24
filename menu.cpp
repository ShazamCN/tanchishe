#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

// 设置控制台编码为UTF-8，支持中文显示
void setConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

// 设置控制台颜色
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 隐藏光标
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// 设置光标位置
void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 打印艺术字标题
void printTitle() {
    setColor(11); // 青色
    gotoxy(15, 2);  cout << "╔══════════════════════════════════╗";
    gotoxy(15, 3);  cout << "║                                  ║";
    gotoxy(15, 4);  cout << "║      游  戏  选  择  菜  单       ║";
    gotoxy(15, 5);  cout << "║                                  ║";
    gotoxy(15, 6);  cout << "╚══════════════════════════════════╝";
    setColor(7); // 恢复默认颜色
}

// 打印菜单选项
void printMenu(int selected) {
    const char* options[] = { "🐍  贪吃蛇 (Snake Game)", "✈️  飞机大战 (Airplane War)", "🚪  退出程序" };
    const char* colors[] = { "10", "13", "12" }; // 选项对应颜色

    for (int i = 0; i < 3; i++) {
        gotoxy(18, 10 + i * 3);

        if (i == selected) {
            // 选中项高亮显示
            setColor(14); // 黄色背景+白色文字的效果
            cout << " ▶ ";
            setColor(14);
            cout << options[i] << " ◀ ";
            setColor(7);
        } else {
            setColor(8); // 灰色
            cout << "    ";
            setColor(7);
            cout << options[i];
            if (i < 2) cout << "  ";
            else cout << "    ";
        }
        cout << endl;
    }
}

// 打印装饰边框
void printBorder() {
    setColor(8);
    for (int i = 0; i < 50; i++) {
        gotoxy(i, 0); cout << "─";
        gotoxy(i, 23); cout << "─";
    }
    for (int i = 0; i < 24; i++) {
        gotoxy(0, i); cout << "│";
        gotoxy(49, i); cout << "│";
    }
    gotoxy(0, 0); cout << "┌";
    gotoxy(49, 0); cout << "┐";
    gotoxy(0, 23); cout << "└";
    gotoxy(49, 23); cout << "┘";
    setColor(7);
}

// 底部提示信息
void printFooter() {
    setColor(7);
    gotoxy(10, 20); cout << "使用 ↑ ↓ 方向键移动选择，按 Enter 确认";
    setColor(8);
    gotoxy(14, 21); cout << "按 ESC 可随时退出菜单";
    setColor(7);
}

// 运行选中的游戏
void launchGame(int choice) {
    system("cls");
    switch (choice) {
        case 0:
            setColor(10);
            cout << "=== 启动贪吃蛇 ===" << endl;
            setColor(7);
            system("hello.exe");
            break;
        case 1:
            setColor(13);
            cout << "=== 启动飞机大战 ===" << endl;
            setColor(7);
            system("airplane_war.exe");
            break;
        case 2:
            // 退出程序
            break;
    }
}

int main() {
    // 设置UTF-8编码以支持中文
    setConsoleUTF8();

    // 设置控制台标题
    SetConsoleTitle(TEXT("游戏选择菜单 - 贪吃蛇 & 飞机大战"));

    // 设置控制台大小
    system("mode con cols=50 lines=24");

    hideCursor();

    int selected = 0;
    bool running = true;
    int key;

    while (running) {
        // 绘制界面
        printBorder();
        printTitle();
        printMenu(selected);
        printFooter();

        // 获取按键
        key = _getch();

        // 处理方向键 (_getch() 返回两次: 224 和键码)
        if (key == 224) {
            key = _getch();
            switch (key) {
                case 72: // 上箭头
                    selected = (selected - 1 + 3) % 3;
                    break;
                case 80: // 下箭头
                    selected = (selected + 1) % 3;
                    break;
            }
        } else if (key == 13) { // Enter
            if (selected == 2) {
                running = false;
            } else {
                launchGame(selected);
                // 游戏结束后回到菜单
                system("cls");
                // 恢复隐藏光标
                hideCursor();
            }
        } else if (key == 27) { // ESC
            running = false;
        }
    }

    // 退出动画
    system("cls");
    setColor(11);
    gotoxy(12, 10);
    cout << "感谢游玩，再见！";
    setColor(7);
    gotoxy(12, 12);
    cout << "按任意键退出...";
    _getch();

    return 0;
}
