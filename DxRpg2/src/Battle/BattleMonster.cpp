#include "Battle/BattleMonster.h"
#include "DxLib.h"
#include "StdCommon.h"
#include "SharedCharacterStatus.h"

namespace Battle
{
	BattleMonster::BattleMonster()
	{
		sharedStatus_ = std::make_shared<::SharedCharacterStatus>();  // �G�̏ꍇ�͖��xHP�������ō쐬����
		initializeParameter();
	}

	BattleMonster::~BattleMonster()
	{
	}

	void BattleMonster::initializeParameter()
	{
		x_ = BattleMonPosX1;
		y_ = BattleMonPosY1;
#ifdef _DEBUG
		sharedStatus_->hp_ = 200;
#else
		sharedStatus_->hp_ = 200 + DxLib::GetRand(20);
#endif
		sharedStatus_->hpMax_ = sharedStatus_->hp_;
		sharedStatus_->mp_ = 136;
		sharedStatus_->mpMax_ = 136;
		sharedStatus_->power_ = 20;    // �U����
		sharedStatus_->defense_ = 20;    // �h���
		sharedStatus_->strength_ = 20;    // ��
		sharedStatus_->magic_ = 20;
		sharedStatus_->weapon_ = 20;    // ����̍U����
		sharedStatus_->equip_ = 20;    // �����̖h���
		sharedStatus_->level_ = 10;
	
		damagePoint_ = 0;
		cntSpeed_ = 1.0;    // �J�E���^�[����������X�s�[�h(double)
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
		animationFlag_ = false;    // �A�j���[�V���������邩�̃t���O
		damageFlag_ = false;    // �_���[�W�\�����邩�̃t���O
	}
}