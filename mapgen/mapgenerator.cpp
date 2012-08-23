#include "mapgenerator.h"


MapGenerator::MapGenerator(int wX, int wY)
{
	WorldX = wX;
	WorldY = wY;
	Map = new int*[WorldX];
	for (int x = 0; x < WorldX; x++) {
		Map[x] = new int[WorldY];
	}
	//Create a blank board.
	for (int x=0; x<WorldX ; x++)
	{
		for (int y=0; y<WorldY; y++)
		{
			Map[x][y]=0;
		}
	}
}

void MapGenerator::Generate()
{
	for (int x=0; x<WorldX ; x++)
	{
		for (int y=0; y<WorldY; y++)
		{
			Map[x][y]=qrand()%2;
			if (Map[x][y]==1)
			{
				Map[x][y]=qrand()%2;
				if (Map[x][y]==1)
				{
					Map[x][y]=qrand()%2;
				}
			}
		}
	}

	for (int x=0; x<WorldX ; x++)
	{
		for (int y=0; y<WorldY; y++)
		{
			if (x > 0 && x < WorldX-1)
			{
				if (Map[x-1][y]==1 && Map[x+1][y]==1)
				{
					Map[x][y]=1;
				}
			}
		}
	}
	for (int x=0; x<WorldX ; x++)
	{
		for (int y=0; y<WorldY; y++)
		{
			if (y > 0 && y < WorldY-1)
			{
				if (Map[x][y-1]==1 && Map[x][y+1]==1)
				{
					Map[x][y]=1;
				}
			}
		}
	}
}

void MapGenerator::SetTile(int x, int y, int val)
{
	Map[x-1][y-1]=val;
}

void MapGenerator::PrintBoard()
{
	for (int i = 0; i<WorldY; i++)
	{
		QString tmp = "";
		for (int j=0; j<WorldX; j++)
		{
			tmp+=QString::number(Map[j][WorldY-i-1]);
		}
		qDebug() << tmp.simplified();
	}
}
