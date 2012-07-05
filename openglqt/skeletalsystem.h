#ifndef SKELETALSYSTEM_H
#define SKELETALSYSTEM_H
#include <QMatrix4x4>
#include <QVector3D>
#include <QList>
class Bone {
	public:
		Bone *previous;
		QMatrix4x4 rot;
		QVector3D origin, rotOrigin;
		Bone(Bone *p, QVector3D o) {
			previous = p;
			origin = rotOrigin = o;
			rot.setToIdentity();
		}
		void calcMatrix() {
			rotOrigin = origin;
			Bone *tmp = previous;
			QList<Bone*> order;
			while (tmp != NULL) {
				order.push_back(tmp);
				tmp = tmp->previous;
			}
			while (order.size() != 0) {
				tmp = order.last();
				order.pop_back();
				rot *= tmp->rot;
				rotOrigin = tmp->rot * rotOrigin;
				qDebug() << "debug";
				qDebug() << tmp->rot * rotOrigin;
			}
		}

		void setRotation(QMatrix4x4 in) {
			rot = in;
		}
};

class Skeleton {
	public:
		QList<Bone*> bones;
		Skeleton() {}
		int addBone(QVector3D boneOrigin, int pos) {
			if (pos < 0) {
				bones.push_back(new Bone(NULL, boneOrigin));
			} else {
				bones.push_back(new Bone(bones.at(pos), boneOrigin));
			}
			return bones.size()-1;
		}
		void rotateBone(int bone, QMatrix4x4 rot) {
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
		int addBoneToSkeleton(int s, QVector3D origin, int bone) {
			return skeletons.at(s)->addBone(origin, bone);
		}
		void rotateBone(int s, int b, QMatrix4x4 rot) {
			skeletons.at(s)->rotateBone(b, rot);
		}
		void calculateRotations() {
			for (int i = 0; i < skeletons.size(); i++) {
				skeletons.at(i)->calculateRotations();
			}
		}
};


#endif // SKELETALSYSTEM_H
