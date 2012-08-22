#include "../../skeletalsystem.h"

int main(int argc, char *argv[])
{
	SkeletalSystem s;
	int skel = s.addSkeleton();
	QQuaternion q1 = QQuaternion();
	q1.setVector(1,0,0);
	QQuaternion q2 = QQuaternion();
	q2.setVector(0,0,1);
	qDebug() << q1.normalized();
	qDebug() << q2.normalized();
	q1.normalize();
	q2.normalize();

	int b = s.addBoneToSkeleton(skel, q1, -1);
	qDebug() << b;
	int b2 = s.addBoneToSkeleton(skel, q2, b);

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
