#include "mapgenerator.h"

MapGenerator::MapGenerator(int wX, int wY)
{
	WorldX = wX;
	WorldY = wY;
	Map = new int*[WorldX];
	for (int i = 0; i < WorldX; i++) {
		Map[i] = new int[WorldY];
	}
}

void MapGenerator::Generate()
{
	for (int i=0; i<WorldX; i++)
	{
		for (int j=0; j<WorldY; j++)
		{
			Map[i][j] = 0;
			if(j==0)
			{
				Map[i][j]=1;
			}
		}
	}

	for (int i=0; i<WorldX/2; i++)
	{
		for (int j=0; j<WorldY; j++)
		{
			if(Map[i][j-1] == 1)
			{
				Map[i][j]=qrand()%2;		// Generates half the map
				Map[WorldX-i-1][j]=Map[i][j];	// Mirrors the generated map
			}
		}
	}
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
//
//if (i == 0|| i == (WorldX-1) || j == 0 || j == (WorldY-1))
//{
//	continue;
//}
