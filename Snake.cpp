#include<iostream>
#include<windows.h>
#include<conio.h>
#include<ctime>

using namespace std;

#define MAX_LENGTH 1000

const char DIR_UP    = 'U';
const char DIR_DOWN  = 'D';
const char DIR_LEFT  = 'L';
const char DIR_RIGHT = 'R';

// Boundary constants (leave room for the border itself)
const int BORDER_THICKNESS = 1;

int consoleWidth, consoleHeight;

// Playable area (inside the border)
int boardLeft, boardRight, boardTop, boardBottom;

void initScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top;
    consoleWidth  = csbi.srWindow.Right  - csbi.srWindow.Left;

    // Define inner play area (1 cell inside each wall)
    boardLeft   = BORDER_THICKNESS;
    boardRight  = consoleWidth  - BORDER_THICKNESS - 1;
    boardTop    = BORDER_THICKNESS;
    boardBottom = consoleHeight - BORDER_THICKNESS - 1;
}

struct Point {
    int xCord;
    int yCord;
    Point() {}
    Point(int x, int y) { xCord = x; yCord = y; }
};

class Snake {
    int length;
    char direction;
public:
    Point body[MAX_LENGTH];

    Snake(int x, int y) {
        length = 1;
        body[0] = Point(x, y);
        direction = DIR_RIGHT;
    }

    ~Snake() {}

    int getLength() { return length; }

    void changeDirection(char newDirection) {
        if      (newDirection == DIR_UP    && direction != DIR_DOWN)  direction = newDirection;
        else if (newDirection == DIR_DOWN  && direction != DIR_UP)    direction = newDirection;
        else if (newDirection == DIR_LEFT  && direction != DIR_RIGHT) direction = newDirection;
        else if (newDirection == DIR_RIGHT && direction != DIR_LEFT)  direction = newDirection;
    }

    // Returns false when the snake hits the boundary or itself
    bool move(Point food) {
        for (int i = length - 1; i > 0; i--)
            body[i] = body[i - 1];

        switch (direction) {
            int val;
            case DIR_UP:    val = body[0].yCord; body[0].yCord = val - 1; break;
            case DIR_DOWN:  val = body[0].yCord; body[0].yCord = val + 1; break;
            case DIR_RIGHT: val = body[0].xCord; body[0].xCord = val + 1; break;
            case DIR_LEFT:  val = body[0].xCord; body[0].xCord = val - 1; break;
        }

        // Collision with boundary walls
        if (body[0].xCord < boardLeft  || body[0].xCord > boardRight ||
            body[0].yCord < boardTop   || body[0].yCord > boardBottom)
            return false;

        // Collision with self
        for (int i = 1; i < length; i++)
            if (body[0].xCord == body[i].xCord && body[0].yCord == body[i].yCord)
                return false;

        return true;
    }

    void growSnake() {
        body[length] = Point(body[length - 1].xCord, body[length - 1].yCord);
        length++;
    }
};

class Board {
    Snake *snake;
    const char SNAKE_BODY = 'O';
    Point food;
    const char FOOD = 'o';
    int score;

    // Win32 console color helpers
    HANDLE hConsole;

    void setColor(int color) {
        SetConsoleTextAttribute(hConsole, color);
    }

    void resetColor() {
        SetConsoleTextAttribute(hConsole, 7); // default white-on-black
    }

public:
    Board() {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        // Start snake near the center of the play area
        int startX = boardLeft + (boardRight  - boardLeft) / 2;
        int startY = boardTop  + (boardBottom - boardTop)  / 2;
        snake = new Snake(startX, startY);
        score = 0;
        spawnFood();
    }

    ~Board() { delete snake; }

    int getScore() { return score; }

    void spawnFood() {
        bool onSnake = true;
        while (onSnake) {
            // Clamp to play area (inside boundary)
            int x = boardLeft  + rand() % (boardRight  - boardLeft  + 1);
            int y = boardTop   + rand() % (boardBottom - boardTop   + 1);
            food = Point(x, y);

            onSnake = false;
            for (int i = 0; i < snake->getLength(); i++) {
                if (snake->body[i].xCord == food.xCord &&
                    snake->body[i].yCord == food.yCord) {
                    onSnake = true;
                    break;
                }
            }
        }
    }

    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(hConsole, coord);
    }

    void drawBoundary() {
        // Color 11 = bright cyan border
        setColor(11);

        int left   = boardLeft  - BORDER_THICKNESS;
        int right  = boardRight + BORDER_THICKNESS;
        int top    = boardTop   - BORDER_THICKNESS;
        int bottom = boardBottom + BORDER_THICKNESS;

        // Top and bottom rows
        for (int x = left; x <= right; x++) {
            gotoxy(x, top);    cout << '#';
            gotoxy(x, bottom); cout << '#';
        }
        // Left and right columns (excluding corners already drawn)
        for (int y = top + 1; y < bottom; y++) {
            gotoxy(left,  y); cout << '#';
            gotoxy(right, y); cout << '#';
        }

        resetColor();
    }

    void drawScore() {
        // Color 14 = bright yellow score text
        setColor(14);
        gotoxy(boardLeft, boardBottom + BORDER_THICKNESS + 1);
        cout << "Score: " << score;
        resetColor();
    }

    void draw() {
        system("cls");

        drawBoundary();

        // Snake body — bright green
        setColor(10);
        for (int i = 0; i < snake->getLength(); i++) {
            gotoxy(snake->body[i].xCord, snake->body[i].yCord);
            cout << SNAKE_BODY;
        }
        resetColor();

        // Food — bright red
        setColor(12);
        gotoxy(food.xCord, food.yCord);
        cout << FOOD;
        resetColor();

        drawScore();
    }

    bool update() {
        bool isAlive = snake->move(food);
        if (!isAlive) return false;

        if (food.xCord == snake->body[0].xCord &&
            food.yCord == snake->body[0].yCord) {
            score++;
            snake->growSnake();
            spawnFood();
        }

        return true;
    }

    void getInput() {
        if (kbhit()) {
            int key = getch();
            if      (key == 'w' || key == 'W') snake->changeDirection(DIR_UP);
            else if (key == 'a' || key == 'A') snake->changeDirection(DIR_LEFT);
            else if (key == 's' || key == 'S') snake->changeDirection(DIR_DOWN);
            else if (key == 'd' || key == 'D') snake->changeDirection(DIR_RIGHT);
        }
    }
};

int main()
{
    // Hide the cursor for a cleaner look
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    initScreen();
    srand(time(0));

    Board *board = new Board();

    while (true) {
        board->getInput();
        if (!board->update()) break;
        board->draw();
        Sleep(max(50, 100 - board->getScore() * 5));
    }

    SetConsoleTextAttribute(hConsole, 12); // red game-over text
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hConsole, coord);
    cout << "Game Over!" << endl;
    SetConsoleTextAttribute(hConsole, 14); // yellow score
    cout << "Final Score: " << board->getScore() << endl;
    SetConsoleTextAttribute(hConsole, 7);  // reset

    // Restore cursor
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    delete board;
    return 0;
}
