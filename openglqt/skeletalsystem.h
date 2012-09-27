#ifndef SKELETALSYSTEM_H
#define SKELETALSYSTEM_H
#include <QDebug>
#include <QList>
#include <QQuaternion>
#include <QMatrix4x4>

class Bone {
	public:
		Bone *mPrevious;
		QQuaternion mOrigin, rotOrigin, rot;
		Bone(Bone *p, QQuaternion o) {
			mPrevious = p;
			mOrigin = rotOrigin = QQuaternion();
			rot = o;
		}
		void calcMatrix() {
			rotOrigin = mOrigin;
			Bone *tmp = mPrevious;
			if (tmp != NULL) {
				rot *= tmp->rot;
				rotOrigin = tmp->rot * rotOrigin;
				qDebug() << rotOrigin;
				qDebug() << "debug";
			}
		}

		void setRotation(QQuaternion in) {
			rot = in;
		}
};

class Skeleton {
	public:
		QList<Bone*> bones;
		Skeleton() {}
		int addBone(QQuaternion boneOrigin, int pos) {
			if (pos < 0) {
				bones.push_back(new Bone(NULL, boneOrigin));
			} else {
				bones.push_back(new Bone(bones.at(pos), boneOrigin));
			}
			return bones.size()-1;
		}
		void rotateBone(int bone, QQuaternion rot) {
			bones.at(bone)->rot = rot;
		}
		void calculateRotations() {
			for (int i = 0; i < bones.size(); i++) {
				bones.at(i)->calcMatrix();
			}
		}
};

class SkeletalSystem {
	public:
		QList<Skeleton *> skeletons;
		int addSkeleton() {
			skeletons.push_back(new Skeleton());
			return skeletons.size()-1;
		}
		int addBoneToSkeleton(int s, QQuaternion origin, int bone) {
			return skeletons.at(s)->addBone(origin, bone);
		}
		void rotateBone(int s, int b, QQuaternion rot) {
			skeletons.at(s)->rotateBone(b, rot);
		}
		void calculateRotations() {
			for (int i = 0; i < skeletons.size(); i++) {
				skeletons.at(i)->calculateRotations();
			}
		}
		QMatrix4x4 getMatrixForSkeleton(int s, int b) {
			QMatrix4x4 ret;
			ret.rotate(skeletons.at(s)->bones.at(b)->rot);
			return ret;
		}
};


#endif // SKELETALSYSTEM_H
