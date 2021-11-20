#pragma once
#include "StdCommon.h"

namespace Map
{
	constexpr int	DefWalkSpeed = 1;
	// ���̐l�Ԃ��������x���� -> �����قǂ��܂ɂ��������Ȃ�
	constexpr int	WalkRand	= 100;
	// int�^�udir�v�̒l����x,y���ꂼ��̕����̑��Έʒu�Ƃ��Ăɕ�����������
	// x,y�͂��ꂼ��A���A���A�E�A��
	constexpr int OffsetX[] = {  0, -1,  1,  0 };
	constexpr int OffsetY[] = {  1,  0,  0, -1 }; 
	constexpr int DirNum = 4;

	class AbstractMapCharacter
	{
	public:
		AbstractMapCharacter();
		virtual ~AbstractMapCharacter(){};

		int getX() const;
		int getY() const;
		int getImg() const;
		bool getImgFlag() const;
		Direction getDir() const;
		int getKind() const;
		int getAnimePicPos() const;
		void setImg(int img);
		void setImgFlag(bool imgFlag);
		void setX(const int x);
		void setY(const int y);

		virtual void initMapState(int tx, int ty, Direction dir, int kind) = 0;
		virtual bool move(PassFlag *isPassable) = 0;

	protected:
		int x_;  // �L�����̂w���W
		int y_;  // �L�����̂x���W
		int walkPixel_;  // �A�j���[�V�����̊G�łǂ̊G���g�����̔��f�̂��߂̕���
		bool walkFlag_;  // �����Ă��邩�����Ă��Ȃ���
		Direction dir_;  // �S�������Ǘ�
		bool flag_;  // �L�����N�^�̑��݃t���O
		int img_;  // �L�����N�^�̊G
		bool imgFlag_;  // �L�����N�^�̊G�̕`��t���O
		int kind_;  // ��l���Ƃ����̐l�̎�ނƂ�
		int animePicPos_;  // 16�����摜�̃L�����G�ʒu  0-15
		int walkSpeed_;  // �L�����̓�������

		void stop();
		void walk();
		void setPosition();
		int decideAnimePic(int WalkPixel, Direction dir);
		bool canMove(PassFlag isPassable);
	};
}