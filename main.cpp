#include <iostream>
#include <conio.h>
#include <list>
#include "colors.h"
#include <windows.h>
#include <time.h>
#define FULL_PIXEL_BLOCK 0x2588


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


void seed();
void input();
void play(list<sSnakeStruct> &snake);
void generateFruit(int *nFruitX, int *nFruitY, list<sSnakeStruct> snake , int nScreenWidth, int nScreenHeight);
int menu(int *nScreenWidth, int *nScreenHeight);
int main(){
    int nScreenWidth = 50;
    int nScreenHeight = 20;

    seed();
    while(menu(&nScreenWidth,&nScreenHeight)==1){

        list<sSnakeStruct> snake = {{17,5},{16,5},{15,5},{14,5},{13,5},{12,5}};
        int nFruitX, nFruitY;
        generateFruit(&nFruitX, &nFruitY, snake, nScreenWidth, nScreenHeight);
        float fScore = 1.0f;
        float fScoreConstant = 5.0f;
        bool isDead = false;
        unsigned int TIME_SLEEP;
        char cScore[50]="";
        string sTitle = "Snake game ----- Score : ";

        // snake is heading towards
        dir = EAST;


        auto *screen = new char[nScreenWidth*nScreenHeight];
        auto *screenAttr = new WORD[nScreenWidth*nScreenHeight];
        for(int j = 0 ; j< nScreenWidth*nScreenHeight ; j++){
            screenAttr[j] = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
        }
        for (int i = 0; i < nScreenWidth*nScreenHeight; i++) {
            screen[i] = ' ';
        }

        DWORD dwBytesWritten = 0;
        DWORD dwAttrBytesWritten = 0;

        // console size and buffer size scaling
        SMALL_RECT m_rectWindow = { 0, 0, 1, 1 };
        COORD coord = { (short)nScreenWidth, (short)nScreenHeight };

        HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        SetConsoleWindowInfo(hConsole, TRUE, &m_rectWindow);

        SetConsoleScreenBufferSize(hConsole ,coord);
        SetConsoleActiveScreenBuffer(hConsole);

        m_rectWindow = {0,0,(short)(nScreenWidth-1), (short)(nScreenHeight-1)};
        SetConsoleWindowInfo(hConsole ,1 , &m_rectWindow);

        CHAR_INFO* m_bufScreen = new CHAR_INFO[nScreenWidth*nScreenHeight];
        memset(m_bufScreen, 0, sizeof(CHAR_INFO) * nScreenWidth * nScreenHeight);

        while(!isDead){

            sprintf(cScore,"%.1f",fScore);
            SetConsoleTitle((sTitle+cScore).c_str());

            // to have a better gameplay
            (dir == NORTH || dir == SOUTH) ? TIME_SLEEP = 100 :  TIME_SLEEP = 80;

            Sleep(TIME_SLEEP);
            //emptying the pixels each time
            for (int i = 0; i < nScreenWidth*nScreenHeight; i++){
                screen[i] = ' ';
                screenAttr[i] = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
            }

            // draw borders
            for (int i = 0; i < nScreenWidth; i++) {
                screen[i] = ' ';
                screen[(nScreenHeight-1) * nScreenWidth +i] = ' ';

                screenAttr[i] = FULL_PIXEL_BLOCK ;
                screenAttr[(nScreenHeight-1) * nScreenWidth +i] = FULL_PIXEL_BLOCK ;

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
                        screen[item.y * nScreenWidth + item.x] = ' ';
                        screenAttr[item.y * nScreenWidth + item.x ] = FULL_PIXEL_BLOCK | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE ;
                        j++;
                        continue;
                    }
                    screenAttr[item.y * nScreenWidth + item.x ] =  FULL_PIXEL_BLOCK ;
                    screen[item.y * nScreenWidth + item.x] = ' ';
                }

            }

            // user input
            input();

            //snake movement
            play(snake);

            //fruit display
            screen[(nFruitY ) * nScreenWidth  + (nFruitX)] = 'X';
            screenAttr[(nFruitY ) * nScreenWidth  + (nFruitX)] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

            //fruit snake collision
            if(snake.front().x == nFruitX && snake.front().y == nFruitY){
                // each fruit makes snakes grow 5 box
                for(int i = 0 ; i < 3 ; i++){
                    snake.push_back({snake.back().x,snake.back().y});
                }

                generateFruit(&nFruitX, &nFruitY, snake, nScreenWidth, nScreenHeight);
                fScore = fScore + fScoreConstant;
                fScoreConstant = fScoreConstant * 1.2f;
                /*
                if(TIME_SLEEP < 70){
                    TIME_SLEEP = TIME_SLEEP - 1;
                }
                else{
                    TIME_SLEEP = TIME_SLEEP - 2;
                }*/
            }

            // snake wall collision
            if(snake.front().x < 0 || snake.front().x >= nScreenWidth){
                isDead = true;
            }
            if(snake.front().y <= 0 || snake.front().y >= nScreenHeight-1){
                isDead = true;
            }

            // snake snake collision
            int k = 0;
            for(auto item : snake){
                // to pass snakes head vs condition
                if(k>0){
                    if(item.x == snake.front().x && item.y == snake.front().y) isDead = true;
                }
                k++;
            }

            // cover the length of snake
            snake.pop_back();

            WriteConsoleOutputAttribute(hConsole, screenAttr, nScreenWidth*nScreenHeight , {0,0}, &dwAttrBytesWritten);
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
        }
        free(screen);
        free(screenAttr);
        free(&dwAttrBytesWritten);
        free(&dwBytesWritten);
        free(hConsole);
        free(&snake);
        menu(&nScreenWidth,&nScreenHeight);
    }
    system("pause");
    return 0;
}
void seed(){
    time_t t;
    srand((unsigned) time(&t));
}
void input(){
    fflush(stdin);
    if(_kbhit()){
        switch(_getch()){
            case 'W':
            case 'w':
                if(dir == SOUTH){
                    dir = SOUTH;
                }
                else{
                    dir = NORTH;
                }
                break;
            case 'D':
            case 'd':
                if(dir == WEST){
                    dir = WEST;
                }
                else{
                    dir = EAST;
                }
                break;
            case 'A':
            case 'a':
                if(dir == EAST){
                    dir = EAST;
                }
                else{
                    dir = WEST;
                }
                break;
            case 'S':
            case 's':
                if(dir == NORTH){
                    dir = NORTH;
                }
                else{
                    dir = SOUTH;
                }
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

void generateFruit(int *nFruitX, int *nFruitY, list<sSnakeStruct> snake , int nScreenWidth, int nScreenHeight){
    // to make sure fruit didnt appear under snake
    bool bIsUnder = true;
    while(bIsUnder){
        *nFruitX = rand()%nScreenWidth;
        *nFruitY = (rand() % (nScreenHeight-3))+2;
        for(auto item : snake){
            if(item.x == *nFruitX && item.y == *nFruitY) {
                bIsUnder = true;
                break;
            }
            else{
                bIsUnder =false;
            }
        }
    }
}

int menu(int *nScreenWidth, int *nScreenHeight){
    //HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    //flush(hConsoleOut);
    int nChoice = 0;
    while(nChoice!=1){
        system("cls");

        printf("  ____              _           ____                      \n"
                       " / ___| _ __   __ _| | _____   / ___| __ _ _ __ ___   ___ \n"
                       " \\___ \\| '_ \\ / _` | |/ / _ \\ | |  _ / _` | '_ ` _ \\ / _ \\\n"
                       "  ___) | | | | (_| |   <  __/ | |_| | (_| | | | | | |  __/\n"
                       " |____/|_| |_|\\__,_|_|\\_\\___|  \\____|\\__,_|_| |_| |_|\\___|\n"
                       "");
        printf("\n1- Play\n"
                       "2- Settings\n"
                       "3- Leader Board\n"
                       "4- Credits\n"
                       "5- Exit");
        fflush(stdin);
        scanf("%d",&nChoice);
        switch (nChoice){
            case 1:
                return 1;
            case 2: {
                int nSettingsChoice=1;
                while(nSettingsChoice != 4){
                    system("cls");
                    printf("1-Change screen width\n");
                    printf("2-Change screen height\n");
                    printf("3-Load it from a file \n");
                    printf("4-Return the main menu (Use defaults)");
                    fflush(stdin);
                    scanf("%d",&nSettingsChoice);

                    if(nSettingsChoice == 1){
                        printf("Enter the screen width :\n");
                        scanf("%d", nScreenWidth);
                    }
                    else if(nSettingsChoice == 2){
                        printf("Enter the screen height :\n");
                        scanf("%d", nScreenHeight);
                    }
                    else if(nSettingsChoice == 3){
                        char fileName[20]={""};
                        char cScreenWidth[3];
                        char cScreenHeight[3];
                        FILE *fp;

                        printf("Enter the file name :\n");
                        scanf("%s", fileName);

                        fp = fopen(fileName,"r");

                        if(fp!=nullptr){
                            fscanf(fp, "%[^;];", cScreenWidth);
                            fscanf(fp, "%[^;];", cScreenHeight);
                        }
                        else{
                            printf("Could not open the file \n");
                            break;
                        }
                        *nScreenWidth = atoi(cScreenWidth);
                        *nScreenHeight = atoi(cScreenHeight);


                    }
                    else{
                        printf("You have entered something wrong \n");
                    }

                }

            }
                break;
            case 3:
                break;
            case 4:
                printf("Melih Kaan Yildiz \n github.com/morph3\n");
                break;
            case 5:
                return 0;
        }

    }
}