#pragma once
#include "AbstractMapCharacter.h"
#include "SharedCharacterStatus.h"  // �s���S�N���X�^�ւ̃|�C���^�[���Q�Ƃł��Ȃ������Ȃ̂�
#include "Util/GameController.h"

namespace Map
{
	constexpr int	EncRand = 4;  // �o�g���ɓ˓�����m��
	constexpr int	EncMinWalkCnt = 4;  // �o�g���ɓ˓�����Œ����

	class MapMainCharacter : public AbstractMapCharacter
	{
	public:
		MapMainCharacter();
		~MapMainCharacter() override;
		void initMapState(int tx, int ty, Direction dir, int kind) override;
		bool move(PassFlag *isPassable) override;
		// ����������SharedCharacterStatus���Q�Ƃł���悤�ɂ���

	private:
		int  prevX_;  // �O�񂢂�X���W
		int  prevY_;  // �O�񂢂�Y���W
		int  encCnt_;  // �G�����J�E���g
		bool encountersEnemy();
		void updatePassable(Util::GameController &gc, PassFlag *isPassable);
	};
}