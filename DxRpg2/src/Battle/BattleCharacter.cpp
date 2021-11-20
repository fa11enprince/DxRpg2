#include "Battle/BattleCharacter.h"
#include "StdCommon.h"
#include "SharedCharacterStatus.h"
#include <cstdlib>

namespace Battle
{
	BattleCharacter::BattleCharacter(std::shared_ptr<::SharedCharacterStatus> sharedStatus)
	{
		sharedStatus_ = std::move(sharedStatus);
		initializeParameter();
	}

	BattleCharacter::~BattleCharacter()
	{
	}

	void BattleCharacter::initializeParameter()
	{
		x_ = BattleCharPosX1;
		y_ = BattleCharPosY1;

		damagePoint_ = 0;
		cntSpeed_ = 1.0;
		for (int i = 0; i < sizeof(abiFlag_); i++)
		{
			abiFlag_[i] = 0;
		}
		atbCnt_ = DxLib::GetRand(AtbCntMax - 5);  // �J�E���^�[240�ōs����
		atbCntFlag_ = true;
		motionFlag_ = false;
		menuSelect_ = 1;
		menuSelectY_ = 0;
		actionCnt_ = 0;
		damageActionCnt_ = 0;
		actionFlag_ = false;

		animationFlag_ = false;
		damageFlag_ = false;
	}

	void BattleCharacter::pressedKeyUp()
	{
		menuSelect_--;
		menuSelectY_--;
		if (menuSelectY_ == -1) // �I���ʒu����ԏ�𒴂�����
		{
			menuSelect_ += 5;    // ��ԉ��� 
			menuSelectY_ = 4;   // ��ԉ��� 4�͑I��\�����ڂ̍ő吔-1
		}
	}

	void BattleCharacter::pressedKeyDown()
	{
		menuSelect_++;
		menuSelectY_++;
		if (menuSelectY_ == 5) // �I���ʒu����ԏ�𒴂�����
		{
			menuSelect_ -= 5;   // ��ԏ��
			menuSelectY_ = 0;   // ��ԏ��
		}
	}

	void BattleCharacter::pressedKeyX()
	{
		menuSelect_ = menuSelect_ * 100;   // ����{�^����������邽�т�100�{��
		menuSelectY_ = 0;   // �I���ʒu��������Ԃ�
	}

	void BattleCharacter::pressedKeyZ()
	{
		menuSelect_ = 1;    // �I����Ԃ�߂� 
		menuSelectY_ = 0;   // �I���ʒu��������Ԃ�
	}

	void BattleCharacter::resetSelectState()
	{
		switch (menuSelect_)
		{
			case 10200:
			case 10300:
			case 10400:
		menuSelect_ = 10000;
				break;
			case 30000:
				menuSelect_ = 10000;
				break;
		}
	}
}