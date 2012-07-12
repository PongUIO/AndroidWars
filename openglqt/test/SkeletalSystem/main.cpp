#include "../../skeletalsystem.h"

int main(int argc, char *argv[])
{
	SkeletalSystem s;
	int skel = s.addSkeleton();
	int b = s.addBoneToSkeleton(skel, QQuaternion(1,0,-1,1), -1);
	qDebug() << b;
	int b2 = s.addBoneToSkeleton(skel, QQuaternion(1,0,0,1), b);

	qDebug() << s.getMatrixForSkeleton(0, 0);
	qDebug() << s.skeletons.at(0)->bones.at(0)->rotOrigin;
	qDebug() << s.getMatrixForSkeleton(0, 1);
	qDebug() << s.skeletons.at(0)->bones.at(1)->rotOrigin;
	//s.rotateBone(0, 1, QQuaternion(1,0,0,1));
	//s.rotateBone(0, 0, QQuaternion(2,0,2,0));
	s.calculateRotations();
	qDebug() << s.getMatrixForSkeleton(0, 0);
	qDebug() << s.skeletons.at(0)->bones.at(0)->rotOrigin;
	qDebug() << s.getMatrixForSkeleton(0, 1);
	qDebug() << s.skeletons.at(0)->bones.at(1)->rotOrigin;
}
