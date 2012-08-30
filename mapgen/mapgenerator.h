#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H
#include <QDebug>
#include <QString>

class MapGenerator
{
public:
	MapGenerator(int, int);
	void Generate();
	void SetTile(int , int, int);
	void CreateSpawnProtection();
	void PrintBoard();
	int **Map;
	int WorldX, WorldY;
};

#endif // MAPGENERATOR_H
