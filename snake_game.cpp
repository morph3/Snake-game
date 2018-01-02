#include <iostream>
#include <conio.h>
#include <list>
#include <windows.h>
#include <time.h>
#include <string>
using namespace::std;


struct sSnakeStruct{
    int x;
    int y;
};

enum eDirection{
    STOP = 0,
    NORTH,
    EAST,
    SOUTH,
    WEST,
}dir;

void input();
void play(list<sSnakeStruct> &snake);
int main(){

    srand(time(NULL));

    list<sSnakeStruct> snake = {{4,5},{5,5},{6,5},{7,5},{8,5},{9,5}};
    const int nScreenWidth = 120;
    const int nScreenHeight = 25;
    int nFruitX = 1;
    int nFruitY = 10;

    int nScore = 0;

    bool isDead = false;
    // snake is heading towards
    dir = WEST;

    char *screen = new char[nScreenWidth*nScreenHeight];
    WORD *screenAttr = new WORD[nScreenWidth*nScreenHeight];
    for(int j = 0 ; j< nScreenWidth*nScreenHeight ; j++){
        screenAttr[j] = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
    }
    for (int i = 0; i < nScreenWidth*nScreenHeight; i++) {
        screen[i] = ' ';
    }

    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;
    DWORD dwAttrBytesWritten = 0;


    string Title = "Snake Game  ---- Score :  ";
    string Score;

    unsigned int TIME_SLEEP = 100;


    while(!isDead){
        Score = to_string(nScore);

        SetConsoleTitle((Title+Score).c_str());


        //printf(&screen[1 + nScreenWidth],"Score : %d" , nScore );


        Sleep(TIME_SLEEP);
        //emptying the pixels each time
        for (int i = 0; i < nScreenWidth*nScreenHeight; i++){
            screen[i] = ' ';
            screenAttr[i] = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
        }

        // draw border
        for (int i = 0; i < nScreenWidth; i++) {
            screen[i] = '=';
            screen[(nScreenHeight-1) * nScreenWidth +i] = '=';

            screenAttr[i] = FOREGROUND_RED;
            screenAttr[(nScreenHeight-1) * nScreenWidth +i] = FOREGROUND_RED;

        }
        //snake body drawing
        int j = 0;
        for (auto item : snake) {
            if(isDead){
                screen[item.y * nScreenWidth + item.x] = '+';
            }
            else{
                // snakes head
                if(j == 0 ){
                    screen[item.y * nScreenWidth + item.x] = 'O';
                    screenAttr[item.y * nScreenWidth + item.x ] = FOREGROUND_GREEN;
                    j++;
                    continue;
                }
                screenAttr[item.y * nScreenWidth + item.x ] = FOREGROUND_GREEN;
                screen[item.y * nScreenWidth + item.x] = 'o';
            }

        }

        // user input
        input();

        //snake movement
        play(snake);

        //fruit display
        screen[(nFruitY ) * nScreenWidth  + (nFruitX)] = 'F';
        screenAttr[(nFruitY ) * nScreenWidth  + (nFruitX)] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

        //fruit snake collision
        if(snake.front().x == nFruitX && snake.front().y == nFruitY){
            // each fruit makes snakes grow 5 box
            for(int i = 0 ; i < 5 ; i++){
                snake.push_back({nFruitX,nFruitY});
            }
            nFruitX = rand()%nScreenWidth;
            nFruitY = (rand() % (nScreenHeight-3))+2;
            nScore ++;
            if(TIME_SLEEP < 70){
                TIME_SLEEP = TIME_SLEEP - 1;
            }
            else{
                TIME_SLEEP = TIME_SLEEP - 2;
            }
        }

        // snake wall collision

        // cover the length of snake
        snake.pop_back();

        WriteConsoleOutputAttribute(hConsole, screenAttr, nScreenWidth*nScreenHeight , {0,0}, &dwAttrBytesWritten);
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }

    system("pause");
    return 0;
}

void input(){
    fflush(stdin);
    if(_kbhit()){
        switch(_getch()){
            case 'W':
            case 'w':
                dir = NORTH;
                break;
            case 'D':
            case 'd':
                dir = EAST;
                break;
            case 'A':
            case 'a':
                dir = WEST;
                break;
            case 'S':
            case 's':
                dir = SOUTH;
                break;
            case 'x':
                dir = STOP;

            default:
                break;
        }
    }
}

void play(list<sSnakeStruct> &snake){
    switch (dir) {
        case NORTH:
            snake.push_front({snake.front().x, snake.front().y-1});
            break;
        case EAST:
            snake.push_front({snake.front().x+1, snake.front().y});
            break;
        case SOUTH:
            snake.push_front({snake.front().x,snake.front().y+1});
            break;
        case WEST:
            snake.push_front({snake.front().x-1,snake.front().y});
            break;
        case STOP:
            break;
        default:
            break;
    }
}
