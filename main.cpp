#include<iostream>
#include<thread>
#include<conio.h>		 //to use _getch
#include<cstdlib>		 //to use rand
#include<ctime>			 //to use time() in srand

#define up 72
#define left 75
#define right 77
#define down 80

using namespace std;

struct position
{
	int spaceship_height_plusminus;
	int spaceshipe_leftright_limit;
	int length_minus_bulletSTART;
	int BULLET_TYPE;
};
position pos1 = { 2,4,5,1 };
position pos2 = { 3,6,6,2 };
position currentPOSITION = pos2;
const short length = 23; //this controls the dimension of game
const short breath = length + (int)(length * 0.6);
static bool gameCONTINUITY = 1;		//this keeps track of state of game ,ended or started and going on
int score = 0;		//as name suggest bro ; )
int continuous_score = 0;		//get power after every 100 continuous hits and lose after every negativity
auto bullet_SPEED = 0.2s, enemy_SPEED = 0.5s;
char a[length][breath];		//main array that containes the main game to display
char name[17];
short PAUSE = 0;	//to add pause and play feature in game
short random;		// to add invert and upside down feature in game
short spaceship_length = length - currentPOSITION.spaceship_height_plusminus;
short spaceship_breath = breath / 2;
short POWER = 1;		//this can be 1 or 3 only......................................................................................
char BULLET = '*';
char ENEMY = 'O';
short GodMode = 0;		//this makes pullet power and speed to maximum forever and start with high scores; )

struct BULLETlist
{
	BULLETlist* next = NULL;
	short power = POWER;
	bool state = 1;
	bool state_left = 1;
	bool state_right = 1;
	short y = spaceship_breath;		//breath
};


inline int input();
void initialize();
void display();		//it does more than just display, cuz of optimization
void moveleft();
void moveright();
void run();			//this updates the game as per user input

inline int input()
{
	int ch;
	ch = _getch();
	if (ch == 224)
		return _getch();
	else return ch;
}

void initialize()
{
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < breath - 1; j++)
		{
			if (i == length - 1 || j == 0 || j == breath - 2)
				a[i][j] = '#';
			else
				a[i][j] = ' ';
		}
		a[i][breath - 1] = '\0';
	}
	{
		a[spaceship_length][spaceship_breath] = '|';
		a[spaceship_length][spaceship_breath - 1] = '-';
		a[spaceship_length][spaceship_breath - 2] = '<';
		a[spaceship_length][spaceship_breath - 3] = '<';
		a[spaceship_length][spaceship_breath + 1] = '-';
		a[spaceship_length][spaceship_breath + 2] = '>';
		a[spaceship_length][spaceship_breath + 3] = '>';

		a[spaceship_length - 1][spaceship_breath] = 'X';
		a[spaceship_length - 1][spaceship_breath - 2] = 'A';
		a[spaceship_length - 1][spaceship_breath + 2] = 'A';

		a[spaceship_length - 2][spaceship_breath] = 'A';
	}
}

void display()
{
	while (gameCONTINUITY)
	{
		while (gameCONTINUITY && !PAUSE)
		{
			system("CLS");	//clear screen
			//modify the shape before
			cout << "\n    -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-    ITS  < -:" << name;
			cout << ":- >  YOU BIT** " << "    -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-\n\n";
			for (int i = 0; i < length; i++)
				printf("					%s\n", &a[i]);
			cout << "\n\n\t\t\t\t     'p' :- pause(any key to continue later...)";
			cout << "\n\t\t\t\t\t\t    SCORE :- " << score;
		}
	}
}

void setXYspace()
{
	a[spaceship_length][spaceship_breath] = '|';
	a[spaceship_length][spaceship_breath - 1] = '-';
	a[spaceship_length][spaceship_breath - 2] = '<';
	a[spaceship_length][spaceship_breath - 3] = '<';
	a[spaceship_length][spaceship_breath + 1] = '-';
	a[spaceship_length][spaceship_breath + 2] = '>';
	a[spaceship_length][spaceship_breath + 3] = '>';

	a[spaceship_length - 1][spaceship_breath] = 'X';
	a[spaceship_length - 1][spaceship_breath - 2] = 'A';
	a[spaceship_length - 1][spaceship_breath + 2] = 'A';

	a[spaceship_length - 2][spaceship_breath] = 'A';
}

void moveleft()
{
	if (a[spaceship_length][spaceship_breath - currentPOSITION.spaceshipe_leftright_limit] == '#')
		return;
	a[spaceship_length][spaceship_breath + 3] = ' ';
	a[spaceship_length - 1][spaceship_breath] = ' ';
	a[spaceship_length - 1][spaceship_breath - 2] = ' ';
	a[spaceship_length - 1][spaceship_breath + 2] = ' ';
	a[spaceship_length - 2][spaceship_breath] = ' ';
	spaceship_breath--;
	setXYspace();
}

void moveright()
{
	if (a[spaceship_length][spaceship_breath + currentPOSITION.spaceshipe_leftright_limit] == '#')
		return;
	a[spaceship_length][spaceship_breath - 3] = ' ';
	a[spaceship_length - 1][spaceship_breath] = ' ';
	a[spaceship_length - 1][spaceship_breath + 2] = ' ';
	a[spaceship_length - 1][spaceship_breath - 2] = ' ';
	a[spaceship_length - 2][spaceship_breath] = ' ';
	spaceship_breath++;
	setXYspace();
}

void fillBULLET(BULLETlist* start)
{
	int pos = 0;
	while (start)
	{
		if (start->state)
		{
			if (a[pos][start->y] == ENEMY)
			{
				score += 10;
				continuous_score += 10;
				start->state = 0;
				a[pos][start->y] = ' ';
			}
			else
				a[pos][start->y] = BULLET;
		}
		if (start->power == 3 && start->state_left)
		{
			if (a[pos][(start->y) - currentPOSITION.BULLET_TYPE] == ENEMY)
			{
				score += 10;
				continuous_score += 10;
				start->state_left = 0;
				a[pos][(start->y) - currentPOSITION.BULLET_TYPE] = ' ';
			}
			else
				a[pos][(start->y) - currentPOSITION.BULLET_TYPE] = BULLET;
		}
		if (start->power == 3 && start->state_right)
		{
			if (a[pos][(start->y) + currentPOSITION.BULLET_TYPE] == ENEMY)
			{
				score += 10;
				continuous_score += 10;
				start->state_right = 0;
				a[pos][(start->y) + currentPOSITION.BULLET_TYPE] = ' ';
			}
			else
				a[pos][(start->y) + currentPOSITION.BULLET_TYPE] = BULLET;
		}
		start = start->next;
		pos++;
	}
}

void emptyBULLET(BULLETlist* start)
{
	int pos = 0;
	while (start)
	{
		if (start->state)
		{
			a[pos][start->y] = ' ';
		}
		if (start->power == 3 && start->state_left)
		{
			a[pos][(start->y) - currentPOSITION.BULLET_TYPE] = ' ';
		}
		if (start->power == 3 && start->state_right)
		{
			a[pos][(start->y) + currentPOSITION.BULLET_TYPE] = ' ';
		}
		start = start->next;
		pos++;
	}
}

void bullet()
{
	BULLETlist* start = new BULLETlist, * last, * temp, * make_new = start;
	short listLENGTH = length - currentPOSITION.length_minus_bulletSTART;
	while (listLENGTH)
	{
		start->next = new BULLETlist;
		start = start->next;
		listLENGTH--;
	}
	last = start;
	start = make_new;
	emptyBULLET(start);
	while (gameCONTINUITY)
	{
		emptyBULLET(start);
		temp = start;
		start = start->next;
		delete(temp);
		make_new = new BULLETlist;
		make_new->y = spaceship_breath;
		last->next = make_new;
		last = last->next;
		fillBULLET(start);
		//this is used here to improve efficiency and reduce bullet speed : 0
		//this checks if the bullet is at last line ,so to reduce score and decrease power if recquired...
		for (int i = currentPOSITION.spaceshipe_leftright_limit; i < breath - currentPOSITION.spaceshipe_leftright_limit - 1; i++)
		{
			if (a[spaceship_length - 4][i] == ENEMY)
			{
				score -= 10;
				continuous_score = 0;
				a[spaceship_length - 4][i] = ' ';
				POWER = 1;
				bullet_SPEED = 0.2s;
				if (score < (-50) && !GodMode)
				{
					gameCONTINUITY = 0;
				}
			}
			if (continuous_score > 50 && !GodMode)
			{
				POWER = 3;
				bullet_SPEED = 0.2s;
			}
		}
		this_thread::sleep_for(bullet_SPEED);
	}
}

void movedown()
{
	for (int i = spaceship_length - 4; i >= 0; i--)
	{
		for (int j = currentPOSITION.spaceshipe_leftright_limit; j < breath - currentPOSITION.spaceshipe_leftright_limit - 1; j++)
		{
			if (a[i][j] == ENEMY)
			{
				a[i][j] = ' ';
				a[i + 1][j] = ENEMY;
			}
		}
	}
}


void enemy_make()
{
	while (gameCONTINUITY)
	{
		srand(time(0));
		random = (rand() % (breath - (currentPOSITION.spaceshipe_leftright_limit << 1) - 1));
		random += currentPOSITION.spaceshipe_leftright_limit;
		a[0][random] = ENEMY;
		movedown();
		this_thread::sleep_for(enemy_SPEED);
	}
}

void run()
{
	static short i, check, counter;
	char ch;
	counter = 0;
	while (gameCONTINUITY)
	{
		int INPUT = input();
		switch (INPUT)
		{
		case 75:	moveleft();	 PAUSE = 0;								break;
		case 77:	moveright(); PAUSE = 0;								break;
		case 'p':	PAUSE == 0 ? PAUSE = 1 : PAUSE = 0;					break;
		default:	PAUSE = 0;
		}
	}
}

int main()
{
	if (GodMode)
	{
		POWER = 3;
		bullet_SPEED = 0.0s;
		score = INT16_MAX;
	}
	initialize();	//it should be initialize before so, that new shape can be added with all the default shapes updated
	while (gameCONTINUITY)
	{
		thread displayTHREAD(display); //display part and continuos moving down of parts
		thread runTHREAD(run); //this continuously takes input and update direction once runs the game
		thread bulletTHREAD(bullet);
		thread enemyTHREAD(enemy_make);
		enemyTHREAD.join();
		bulletTHREAD.join();
		runTHREAD.join();
		displayTHREAD.join();
	}
	char caps = 'A';
	char low = 'a';
	for (int i = 0; i < 27; i++)
	{
		system("CLS");
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t" << caps << (char)(1 + caps) << (char)(2 + caps) << (char)(3 + caps) << " " << (char)(2 + caps) << (char)(1 + caps) << " :- ";
		cout << (char)(caps + 1) << low << (char)(low + 2) << (char)(low + 3) << low << (char)(low + 1) << (char)(low + 2) << " " << caps << (char)(low + 2) << low << (char)(low + 3) << (char)(low + 1);
		if (caps == 'w')
			caps = 'A';
		else
			caps++;
		if (low == 'w')
			low = 'a';
		else
			low++;
	}
	system("CLS");
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\tMADE BY :- Bharat Patel\n\n\n\n\n\n\n\n\n\n";
}
