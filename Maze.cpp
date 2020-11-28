// Tries to solve a maze using a recursive solution
// Shows path through the maze if one exists

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <string>

#include <Windows.h>
using namespace std;

const int FILENAMESIZE = 255;
const int MAXROWS = 22;
const int MAXCOLS = 81;

// ASCII characters to use for display
// https://theasciicode.com.ar/extended-ascii-code/block-graphic-character-ascii-code-219.html
const char WALL = (char)219;
const char WALLINFILE = '0';
const char START = 'S';
const char DEST = 'D';
const char EMPTY = ' ';
const char PATH = '*';
const char DEAD = 'X';

/*
* getFile will get a filename from the user. It then attmpts
* to open the file, read its contents, and store the maze
* structure into the maze array, based on the contents of the file. 
* A WALLINFILE character in the file represents a maze wall, while
* an EMPTY character represents a free pathway. The file should also
* contain a single START character and a single DEST character.
* The function stops reading and exits the program if > MAXCOLS-1
* characters are found on one row, or if > MAXROWS rows are
* found in the file.  If the function is successful, then 
* startX and startY will be set to the row and column number of 
* where the START character is located in the maze.
*/
void getMaze(char maze[][MAXCOLS], int& startX, int& startY);

/*
* display is used to draw the maze on the screen.
*/
void display(const char maze[][MAXCOLS]);

//recursion function to solve maze
bool mazeSolver(char maze[][MAXCOLS], int X, int Y, int& Counter, bool& mazeSolved);

//checks neighboring nodes for a char we pass through the variable checker
bool checkChar(char maze[][MAXCOLS], int X, int Y, char checker);

void initMaze(char maze[][MAXCOLS]);

void finalMaze(char maze[][MAXCOLS], int startX, int startY);

int main()
{
	char maze[MAXROWS][MAXCOLS];

	//Count the recursion
	int startX, startY = 0;
	int recursionCounter = 0;
	bool solvedMaze = false;

	initMaze(maze);
	getMaze(maze, startX, startY);
	display(maze);

	do
	{
		cout << '\n' << "Press enter to solve the maze.";
	} while (cin.get() != '\n');
	cin.ignore();

	solvedMaze = mazeSolver(maze, startX, startY, recursionCounter, solvedMaze);
	finalMaze(maze, startX, startY);
	display(maze);

	//outputs if we solved the maze or not and the amount of recursion calls there were
	if (solvedMaze) 
	{
		cout << "Solution Found! -- " << recursionCounter << " recursion calls.";
	}
	else 
	{
		cout << "No solution to this maze -- " << recursionCounter << " recursion calls.";
	}

	do
	{
		cout << '\n' << "Press enter to exit.";
	} while (cin.get() != '\n');

	return 0;
}

void display(const char maze[][MAXCOLS])
{
	// system("cls") clears the screen; only works on Windows machines
	// You will probably need to comment this out if working on
	// a non-Windows machine
	system("cls");

	//displays the maze into the console
	for (int i = 0; i < MAXROWS; i++) 
	{
		for (int j = 0; j < MAXCOLS; j++)							
		{
			cout << maze[i][j];
		}
		cout << endl;
	}
	cout << endl;

}

void getMaze(char maze[][MAXCOLS], int& startX, int& startY)
{
	ifstream inFile;
	int row = 1;

	//gets user input for file
	char fileName[FILENAMESIZE];
	cout << "What file name would you like to generate? (Don't type .txt)\n";
	cin >> fileName;
	strcat_s(fileName, ".txt");

	inFile.open(fileName);

	//checks if file is valid
	if (!inFile)
	{
		cerr << "\nERROR: Input file cannot be opened. \n";
		inFile.close();
		exit(1);
	}

	char line[MAXCOLS];

	//accounts for walls, rows, and columns within maze(#).txt
	for (row; row <= MAXROWS && inFile.getline(line, MAXCOLS); row++)
	{
		for (int i = 0; i < MAXCOLS; i++)
		{
			if (line[i] == WALLINFILE) 
			{
				maze[row-1][i] = WALL;
			}
			else if (line[i] == START)
			{
				maze[row-1][i] = START;
				startX = row-1;
				startY = i;
			}
			else if (line[i] == DEST) {
				maze[row-1][i] = DEST;
			}
			else {
				maze[row-1][i] = EMPTY;
			}
		}
	}

	// After file reading is finished, this code checks to see if the reading stopped
	// because of too many characters on a line in the file, or too many rows in the file
	// It is possible that you might have to change the conditions here depending on
	// exactly how you choose to read the file
	if (!inFile.eof())
	{
		if (row <= MAXROWS)
		{
			cerr << "\nERROR: Line " << row << " in input file contains more than " << MAXCOLS - 1 << " chars\n";
		}
		else
		{
			cerr << "\nERROR: There are more than " << MAXROWS << " lines in the file\n";
		}
		inFile.close();
		exit(1);
	}
	inFile.close();
}

bool mazeSolver (char maze[][MAXCOLS], int X, int Y, int& counter, bool& mazeSolved)
{
	//Order: right, down, left, up
	//dont check if point is a wall
	//dont recheck points that are already checked
	counter++;
	maze[X][Y] = PATH;

	//if destination is one away, mazesolved = true and recursion stops
	if (checkChar(maze, X, Y, DEST)) 
	{
		mazeSolved = true;
		return mazeSolved;
	}
	else {
		//goes through each neighboring node till maze is solved
		if (maze[X + 1][Y] == EMPTY && !mazeSolved)
		{
			mazeSolver(maze, X + 1, Y, counter, mazeSolved);
		}
		if (maze[X][Y - 1] == EMPTY && !mazeSolved)
		{
			mazeSolver(maze, X, Y - 1, counter, mazeSolved);
		}
		if (maze[X - 1][Y] == EMPTY && !mazeSolved)
		{
			mazeSolver(maze, X - 1, Y, counter, mazeSolved);
		}
		if (maze[X][Y + 1] == EMPTY && !mazeSolved)
		{
			mazeSolver(maze, X, Y + 1, counter, mazeSolved);
		}
		//if node goes through all paths and doesn't find exit, change it to dead
		if (!checkChar(maze, X, Y, EMPTY) && !mazeSolved)
		{
			maze[X][Y] = DEAD;
		}
		return mazeSolved;
	}
}

bool checkChar(char maze[][MAXCOLS], int X, int Y, char checker) 
{
	//checks neighboring nodes for the char we pass through the variable checker
	if (maze[X + 1][Y] == checker) 
	{
		return true;
	}
	else if (maze[X][Y - 1] == checker) 
	{
		return true;
	}
	else if (maze[X - 1][Y] == checker) 
	{
		return true;
	}
	else if (maze[X][Y + 1] == checker) 
	{
		return true;
	}
	return false;
}

void initMaze(char maze[][MAXCOLS]) 
{
	for (int x = 0; x < MAXROWS; x++) 
	{
		for (int y = 0; y < MAXCOLS; y++) 
		{
			maze[x][y] = EMPTY;
		}
	}

}

void finalMaze(char maze[][MAXCOLS], int startX, int startY) 
{
	for (int x = 0; x < MAXROWS; x++) 
	{
		for (int y = 0; y < MAXCOLS; y++) 
		{
			if (maze[x][y] == DEAD) 
			{
				maze[x][y] = EMPTY;
			}
		}
	}

	maze[startX][startY] = START;
}