#include <QDebug>
#include <QTime>
#include "mapgenerator.h"

int main(int argc, char *argv[])
{
	int x;
	int y;
	switch(1)
	{
		case 1: x = 100; y = 20; break;
		case 2: x = 75; y = 15; break;
		case 3: x = 50; y = 10; break;
		default: x = 25; y = 5;
	}
	MapGenerator mg(x,y);
	mg.Generate();
	mg.PrintBoard();
}
