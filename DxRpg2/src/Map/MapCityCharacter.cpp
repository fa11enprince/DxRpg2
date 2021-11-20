#include "Map/MapCityCharacter.h"

namespace Map
{
	void MapCityCharacter::initMapState(int tx, int ty, Direction dir, int kind)
	{
		x_ = tx;
		y_ = ty;
		flag_ = true; // �L��������
		dir_ = dir;  // �L�����̂ނ��Ă�����������ɃZ�b�g D:0,L:1,R:2,U:3
		imgFlag_ = true;  // �`�悷��
		kind_ = kind;  // ���̐l�̎��
		walkPixel_ = 0;
		walkFlag_ = false;
		img_ = 0;
		walkSpeed_ = DefWalkSpeed;
		animePicPos_ = 1;  // �^��
		stop();  // ����0
	}

	bool MapCityCharacter::move(PassFlag *isPassable)
	{
		setPosition();  // �L�����N�^�ʒu�̌���
		// x��y�����W���P���̔{���̂Ƃ�
		if (x_ % BlockLen == 0 && y_ % BlockLen == 0)
		{
			stop();  // �����J�E���^��߂�
			if (GetRand(WalkRand) == 0)  // �����n�߂�
			{
				dir_ = static_cast<Direction>(DxLib::GetRand(3));  // �����_���ɕ��������� 0-3
				// current position
				int cy = y_ / BlockLen;
				int cx = x_ / BlockLen;
				// next position
				int ny = y_ / BlockLen + OffsetY[static_cast<int>(dir_)];
				int nx = x_ / BlockLen + OffsetX[static_cast<int>(dir_)];

				if (canMove(isPassable[ny * XBlock + nx]))
				{
					// ���݈ʒu��ʂ����̂ɂ��āA�������ʒu��ʂ��Ȃ����̂Ƃ���
					walkFlag_ = true;
					isPassable[cy * XBlock + cx] = PassFlag::Through;
					isPassable[ny * XBlock + nx] = PassFlag::NoThrough;
				}
				else  // �����Ă�������ʂ�Ȃ��ꏊ�Ȃ�
				{
					walkFlag_ = false;
				}
			}
		}
		else  // ��y�����W���P���̔{���łȂ��Ƃ�
		{
			walk();  // ���dir�̕����֕���
		}
		animePicPos_ = decideAnimePic(walkPixel_, dir_);
		return false;  // ���false��Ԃ�(�G���J�E���g���Ȃ�)
	}
}
