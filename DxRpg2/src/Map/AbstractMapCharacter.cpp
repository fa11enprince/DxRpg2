#include "Map/AbstractMapCharacter.h"

namespace Map
{
	AbstractMapCharacter::AbstractMapCharacter()
		: x_(0), y_(0), walkPixel_(0), walkFlag_(false), dir_(Direction::Down),
		flag_(0), img_(0), imgFlag_(0), kind_(0),
		animePicPos_(0), walkSpeed_(DefWalkSpeed)
	{
	}

	int AbstractMapCharacter::getX() const
	{
		return x_;
	}

	int AbstractMapCharacter::getY() const
	{
		return y_;
	}

	int AbstractMapCharacter::getImg() const
	{
		return img_;
	}

	bool AbstractMapCharacter::getImgFlag() const
	{
		return imgFlag_;
	}

	Direction AbstractMapCharacter::getDir() const
	{
		return dir_;
	}

	int AbstractMapCharacter::getKind() const
	{
		return kind_;
	}

	int AbstractMapCharacter::getAnimePicPos() const
	{
		return animePicPos_;
	}

	void AbstractMapCharacter::stop()
	{
		walkPixel_ = 0;
		walkFlag_ = false;
	}
	void AbstractMapCharacter::walk()
	{
		walkPixel_++;
	}

	void AbstractMapCharacter::setImg(int img)
	{
		img_ = img;
	}
	void AbstractMapCharacter::setImgFlag(bool imgFlag)
	{
		imgFlag_ = imgFlag;
	}

	void AbstractMapCharacter::setPosition()
	{
		if (walkFlag_)
		{
			// walkSpeed�����������ɑ΂��ăL�����̍��W�𑝌�����B
			x_ = x_ + OffsetX[static_cast<int>(dir_)] * walkSpeed_;
			y_ = y_ + OffsetY[static_cast<int>(dir_)] * walkSpeed_;
		}
	}

	int AbstractMapCharacter::decideAnimePic(int walkPixel, Direction dir)
	{
		int axisX;
		int axisY;
		// walkPixel��1�s�N�Z��������1����
		// 1���̂ǂ̂�����ɂ���̂��Ƃ����̂��A
		// 1����4���������l�ł���Ă��Ƃ܂�̂����̊G�ƂȂ�B
		// �A�j���[�V�����̂ǂꂩ�����i�摜��x����[1�����z��]�j
		// 0-31��z�肵�Ă���32px / 8�������0-3�ɋϓ��ɂ΂��
		// 8�ł���0-3�Ɏ��܂�
		axisX = (walkPixel % BlockLen) / 8;
		// �����Ńc�N�[��MV�̎d�l�ɍ��킹�ĕύX(��3x��4)
		//�^�񒆂��Î~�摜�Ȃ̂ŕ␳����(������0�͐Î~�����ł���)
		axisX = axisX == 0 ? 1 : axisX - 1;
		// �A�j���[�V�����̌��������(�摜��y����[1�����z��])
		axisY = static_cast<int>(dir) * 3;
		// 0�`2�͉������摜�A3�`5�͍������摜�A
		// 6�`8�͉E�����摜�A9�`11�͏�����摜����Ɛ��l��ϊ�
		return axisX + axisY;
	}

	bool AbstractMapCharacter::canMove(PassFlag isPassable)
	{
		// ��l��
		if (kind_ == MainCharIndex)
		{
			return !(isPassable == PassFlag::NoThrough);
		}
		// ���L����
		else
		{
			return !(isPassable >= PassFlag::MainCharOnly);
		}
	}

	void AbstractMapCharacter::setX(const int x)
	{
		x_ = x;
	}
	void AbstractMapCharacter::setY(const int y)
	{
		y_ = y;
	}
}