#include<iostream>
#include<windows.h>
#include<conio.h>
#include<ctime>

using namespace std;

#define MAX_LENGTH 1000

const char DIR_UP = 'U';
const char DIR_DOWN  = 'D';
const char DIR_LEFT  = 'L';
const char DIR_RIGHT  = 'R';

int consoleWidth, consoleHEight;

void initScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHEight = csbi.srWindow.Bottom - csbi.srWindow.Top;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left;

}

struct Point{
    int xCord;
    int yCord;
    Point(){}
    Point(int x, int y){
        xCord = x;
        yCord = y;
    }
};

class Snake{
    int length;
    char direction;
public:
    Point body [MAX_LENGTH];
    Snake(int x, int y){
        length = 1;
        body[0] = Point(x, y);
        direction = DIR_RIGHT;
    }

    ~Snake(){}
    
    int getLength(){
        return length;
    }

    void changeDirection(char newDirection){
        if(newDirection ==  DIR_UP && direction != DIR_DOWN){
            direction = newDirection;
        }
        else if(newDirection == DIR_DOWN && direction != DIR_UP){
            direction = newDirection;
        }
        else if(newDirection == DIR_LEFT && direction != DIR_RIGHT){
            direction = newDirection;
        }
        else if(newDirection == DIR_RIGHT && direction != DIR_LEFT){
            direction = newDirection;
        }
    }

    bool move(Point food){
        for(int i=length-1; i>0; i--)
        {
            body[i] = body[i-1];
        }
        switch (direction)
        {
            int val;
            case DIR_UP:
                val = body[0].yCord;
                body[0].yCord = val - 1;
                break;
            case DIR_DOWN:
                val  = body[0].yCord;
                body[0].yCord = val + 1;
                break;
            case DIR_RIGHT:
                val = body[0].xCord;
                body[0].xCord = val + 1;
                break;
            case DIR_LEFT:
                val = body[0].xCord;
                body[0].xCord = val - 1;
                break;
        }

        
        if(body[0].xCord < 0 || body[0].xCord >= consoleWidth || body[0].yCord < 0 || body[0].yCord >= consoleHEight)
        {
            return false;
        }
        
        for(int i=1; i<length; i++){
            if(body[0].xCord == body[i].xCord && body[0].yCord == body[i].yCord)
            {
                return false;
            }
        }

        return true;
    }

    void growSnake(){
        body[length] = Point(body[length-1].xCord, body[length-1].yCord);
        length++;
    }

};

class Board{
    Snake *snake;
    const char SNAKE_BODY = 'O';
    Point food;
    const char FOOD = 'o';
    int score;
public:
    Board(){
        snake = new Snake(10,10);
        score = 0;
        spawnfood();
    }
    ~Board(){
        delete snake;
    }

    int getScore(){
        return score;
    }
    
    void spawnfood(){
        bool onSnake = true;
        while(onSnake){
            int x = rand() % consoleWidth;
            int y = rand() % consoleHEight;
            food = Point(x, y);
        
            onSnake = false;
            for(int i = 0; i < snake->getLength(); i++){
                if(snake->body[i].xCord == food.xCord && snake->body[i].yCord == food.yCord){
                    onSnake = true; 
                    break;
                }
            }
        }
    }

    void gotoxy(int x, int y){
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    }

    void draw(){
        system("cls");
        for(int i=0; i<snake->getLength(); i++){
            gotoxy(snake->body[i].xCord, snake->body[i].yCord);
            cout<<SNAKE_BODY;
        }

        gotoxy(food.xCord, food.yCord);
        cout<<FOOD;
    }

    bool update(){
        bool isalive = snake->move(food);
        if(isalive == false) return false;

        if(food.xCord == snake->body[0].xCord && food.yCord == snake->body[0].yCord){
            score++;
            snake->growSnake();
            spawnfood();
        }

        return true;
    }

    void getInput(){
        if(kbhit()){
            int key = getch();
            if(key == 'w' || key == 'W')
            {
                snake->changeDirection(DIR_UP);
            }
            else if(key == 'a' || key == 'A'){
                snake->changeDirection(DIR_LEFT);
            }
            else if(key == 's' || key == 'S'){
                snake->changeDirection(DIR_DOWN);
            }
            else if(key == 'd' || key == 'D'){
                snake->changeDirection(DIR_RIGHT);
            }
        }
    }
};

int main()
{
    initScreen();
    srand(time(0));
    Board *board = new Board();
    while(true){
        board->getInput();
        if(!board->update()) break;
        board->draw();
        Sleep(max(50, 100 - board->getScore() * 5));
    }
    cout<<"Game Over"<<endl;
    cout<<"final Score: "<<board->getScore();
    return 0;
}
