#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define FALSE 0
#define TRUE 1
const int SIZE=20;
enum direction{
	STOP=0,
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
enum direction dir;
void draw(int *gameOver, int *score ,int *tailLength,int snake[], int fruitArray[], int tailX[], int tailY[]);
int score_board();
void generate_fruit(int SIZE,int fruit[]);
void input();
void play(int snake[]);
void tail(int snakeX , int snakeY, int tailLength , int tailX[] , int tailY[]);
void display(int array[], int length);
// TODO : add a menu function that configures the game .
// TODO : add a scoreboard .

int main(){
	// TODO : change variable and function names .
	int fruitArray[2];
	int snake[2]={5,5}; // initializing snake
	int tailX[100], tailY[100];
	tailX[0] = snake[0];// tail initilazing 
	tailY[0] = snake[1];// tail initilazing 
	

	int tailLength = 1;
	int score=0;
	int gameOver = FALSE;
	srand(time(NULL));
	generate_fruit(SIZE,fruitArray);
	
	while(!gameOver){
		fflush(stdin);
		draw(&gameOver ,&score ,&tailLength,snake,fruitArray,tailX , tailY);
		input();
		play(snake);
		printf("Score : %d ", score);
		Sleep(70);
	}
	printf("Game Over");
	return 0;
}

void draw(int *gameOver, int *score ,int *tailLength,int snake[], int fruitArray[], int tailX[], int tailY[]){
	system("cls");
	int snakeX, snakeY;
	snakeX= snake[0];
	snakeY= snake[1];	
	
	// refreshing tail everytime
	tail(snakeX , snakeY ,*tailLength,tailX,tailY);
	int x,y; // coordinates of table
	for(x=0 ; x<=SIZE ; x++){
		for(y=0 ; y<=SIZE; y++){
			
			//snake head drawing
			if( x == snakeX && y == snakeY ){
				printf("S");
				continue;
			}
			// fruit drawing
			if(x==fruitArray[0] && y==fruitArray[1]){
				printf("X");
				continue;
			}
			// if snake eats the fruit
			if(snakeX == fruitArray[0] && snakeY == fruitArray[1]){
				generate_fruit(SIZE , fruitArray);
				(*score)++ ;
				(*tailLength)++;
			}
			
			// snake hits the wall
			if(snakeX == 0 || snakeY ==0 || snakeX == SIZE || snakeY == SIZE){
				*gameOver = TRUE;
			}
			if(x==0 || y==0 || y==SIZE || x==SIZE){
				printf("#");
			}

			else{
				int j = 0;
				int printed = 0;
			// tail printing 
				for(j ; j < *(tailLength); j++ ){
					if(x == tailX[j] && y == tailY[j]){
						printf("s");
						printed = 1;
					}
				}
			//to avoid excess blanks
			if(!printed){
				printf(" ");	
			}
				
			}	
		}
		printf("\n");
	}
}


void generate_fruit(int SIZE, int fruit[]){
	// TODO : make sure that fruit does not appear under snake .
	//fruit[0] == fruitX
	//fruit[1] == fruitY
	fruit[0] = rand()% SIZE;
	fruit[1] = rand()% SIZE;
	if(fruit[0]== 0){
		fruit[0]++;
	}
	if(fruit[1] == 0){
		fruit[1]++;
	}

}


void input(){
	fflush(stdin);
	//kbhit in c
	//kbhit in c: kbhit function is used to determine if a key has been pressed or not. 
	//To use kbhit function in your program you should include the header file "conio.h". 
	//If a key has been pressed then it returns a non zero value otherwise returns zero.
	if(_kbhit()){
		switch(_getch()){
			case 'w':
				dir = UP;
				break;
			case 'a':	
				dir = LEFT;
				break;
			case 'd':	
				dir = RIGHT;
				break;	
			case 's':
				dir = DOWN; 
				break;
			case 'x':
				dir = STOP;
			default:
				break;
		}
	}	
	return;
}

void play(int snake[]){
	/*int snakeX, snakeY;
	snakeX = snake[0];
	snakeY = snake[1];*/
	switch(dir){
		case UP:
			snake[0]--;
			break;
		case DOWN:
			snake[0]++;
			break;
		case LEFT:
			snake[1]--;
			break;
		case RIGHT:
			snake[1]++;
			break;
		case STOP:
			break;
		default:
			break;
	}
	printf("Snake coordiante : %d x %d y \n", snake[0], snake[1]);
	return;
}

void tail(int snakeX , int snakeY, int tailLength , int tailX[] , int tailY[]){
	int i,j;
	int tempArrayX[(tailLength)];
	int tempArrayY[(tailLength)];

	// refreshing tail 
	// basically tail has previous snake coordinates everytime 
	// tailX part
	for(i =0; i <(tailLength) ; i++){
		tempArrayX[i] = tailX[i];
	}
	tailX[0] = snakeX;
	for(j = 1; j <tailLength ; j++){
		tailX[j] = tempArrayX[j-1];
	}

	////////////////////////////////////////////////////////////////////
	// tailY part
	for(i =0; i <(tailLength) ; i++){
		tempArrayY[i] = tailY[i];
	}
	tailY[0] = snakeY;
	for(j = 1; j <(tailLength) ; j++){
		tailY[j] = tempArrayY[j-1];
	}		
	
	
}
