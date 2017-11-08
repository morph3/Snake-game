#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define FALSE 0
#define TRUE 1
const int SIZE=16;
enum direction{
	STOP=0,
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
enum direction dir;
int game_over();
void draw(int snake[], int fruitArray[]);
int score_board();
void generate_fruit(int SIZE,int fruit[]);
void input();
void play(int snake[]);
int score();
// TODO : add a menu function that configures the game .
// TODO : add a scoreboard .

int main(){
	// TODO : change variable and function names .
	int fruitArray[2];
	//int board[SIZE][SIZE];
	// snake has two coordinates x and y
	int snake[2]={5,5}; // and initialized its coordinates
	
	dir = STOP;
	srand(time(NULL));
	generate_fruit(SIZE,fruitArray);
	while(!game_over()){
		fflush(stdin);
		if(score()){
			
			generate_fruit(SIZE , fruitArray);
		}
		draw(snake,fruitArray);
		input();
		play(snake);
		//screen[fruitArray[0],fruitArray[1]]="X";
		printf("Fruit coordinate : %d x %d y \n", fruitArray[0], fruitArray[1]);
		Sleep(90);
	}
	return 0;
}

void draw(int snake[], int fruitArray[]){

	// TODO : change the coordinate increment , it is confusing .

	system("cls");
	int snakeX, snakeY;
	snakeX= snake[0];
	snakeY= snake[1];	
	/*
				i,j 
	### 	 0,0 | 0,1 | 0,2  
	# # 	 1,0 | 1,1 | 1,2
	###  	 2,0 | 2,1 | 2,2
	*/

	int x,y; // coordinates of table

	for(x=0 ; x<=SIZE ; x++){
		for(y=0 ; y<=SIZE; y++){
			if( x == snakeX && y == snakeY ){
				printf("S");
				continue;
			}
			if(x==fruitArray[0] && y==fruitArray[1]){
				printf("X");
				continue;
			}
			if(x==0 || y==0 || y==SIZE || x==SIZE){
				printf("#");
			}
			else{
				printf(" ");
			}		
		}
		printf("\n");
	}
}


int game_over(){
	// TODO : if snake crushes with wall , gameover.
	// if snake eats itself , gameover.
	return FALSE;
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
		default:
			break;
	}
	printf("Snake coordiante : %d x %d y \n", snake[0], snake[1]);
	return;
}


int score(){
	return FALSE;
}