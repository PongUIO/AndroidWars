#include "../../skeletalsystem.h"

int main(int argc, char *argv[])
{
	SkeletalSystem s;
	int skel = s.addSkeleton();
	qDebug() << skel;
	int b = s.addBoneToSkeleton(skel, QVector3D(3,2,1), -1);
	qDebug() << b;
	int b2 = s.addBoneToSkeleton(skel, QVector3D(5,4,3), b);
	qDebug() << b2;
	s.rotateBone(0, 1, QMatrix4x4(0,2,0,0,2,0,0,0,0,0,2,0,0,0,0,2));
	s.rotateBone(0, 0, QMatrix4x4(2,0,0,0,0,2,0,0,0,0,2,0,0,0,0,2));
	s.calculateRotations();
	qDebug() << s.skeletons.at(0)->bones.at(0)->rot;
	qDebug() << s.skeletons.at(0)->bones.at(0)->rotOrigin;
	qDebug() << s.skeletons.at(0)->bones.at(1)->rot;
	qDebug() << s.skeletons.at(0)->bones.at(1)->rotOrigin;
}
