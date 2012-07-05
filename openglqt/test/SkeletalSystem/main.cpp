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
	s.rotateBone(0, 0, QMatrix4x4(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1));

}
