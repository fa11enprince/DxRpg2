#include "Battle/BattleProcess.h"
#include "StdCommon.h"
#include "Util/ResourceLoader.h"
#include "Util/GameController.h"
#include "Battle/AbstractBattleCharacter.h"
#include "Battle/BattleCharacter.h"
#include "Battle/BattleMonster.h"
#include "Battle/BattleAnimation.h"
#include "Battle/BattleGraph.h"
#include "Battle/Board.h"

namespace Battle
{
	BattleProcess::BattleProcess(BattleCharacter *chr, BattleMonster *mon)
		:isWin_(false), isGameover_(false),
		 isWinMoveNext_(false), isGameoverMoveNext_(false),
		 gc_(Util::GameController::getInstance()),
		 rl_(Util::ResourceLoader::getInstance()),
		ba_(std::make_unique<BattleAnimation>()),
		bg_(std::make_unique<BattleGraph>()),
		board_(std::make_unique<Battle::Board>())
	{
		chr_ = chr;
		mon_ = mon;
	}

	BattleProcess::~BattleProcess()
	{
	}

	void BattleProcess::main()
	{
		calculateAtb();  // �o�g���p�̃J�E���^(=ATB)���グ��
 
		if (!isGameover_ && !isWin_)
		{
			// ����
			if (!chr_->getMotionFlag() && chr_->getAtbCnt() == AtbCntMax)
			{
				selectMenu();
			}
			// �G
			if (!mon_->getMotionFlag() && mon_->getAtbCnt() == AtbCntMax)
			{
				mon_->setMotionFlag(true);
			}
		}
		// �ȉ��̏����͐퓬�I����(Win or Gameover)�ł��ꕔ�s��
		// �U���̃��[�V����
		if (chr_->getMotionFlag())
		{
			doAction(chr_, mon_, CharacterEnemyKind::Character);
		}
		if (mon_->getMotionFlag())
		{
			doAction(mon_, chr_, CharacterEnemyKind::Enemy);
		}

		decideGameover();
		decideWin();

		drawBattle();
		
		decreaseHpAction(mon_);
		decreaseHpAction(chr_);
		displayMeter();

		ba_->drawAnimation(chr_, mon_);

		board_->draw();

		// ���[�V�������ĂȂ���ATB�J�E���^��MAX�Ȃ�I�����ڂ�`��
		if (!chr_->getMotionFlag() && chr_->getAtbCnt() == AtbCntMax)
		{
			bg_->drawMenu(chr_);
		}

		if (isGameover_)
		{
			// �Q�[���I�[�o�[����
			drawGameover();
		}
		else if (isWin_)
		{
			// �������Ƃ��̏���
			drawWin();
		}
	}

	bool BattleProcess::getWinMoveNext() const
	{
		return isWinMoveNext_;
	}

	bool BattleProcess::getGameoverMoveNext() const
	{
		return isGameoverMoveNext_;
	}

	void BattleProcess::calculateAtb()
	{
		if (mon_->getAtbCntFlag() && !isGameover_ && !isWin_)
		{
			mon_->increaseAtbCnt();
		}
		if (chr_->getAtbCntFlag() && !isGameover_ && !isWin_)
		{
			chr_->increaseAtbCnt();
		}
	}

	void BattleProcess::selectMenu()
	{
		if (gc_.upPressedNow())
		{
			chr_->pressedKeyUp();
		}
		else if (gc_.downPressedNow())
		{
			chr_->pressedKeyDown();
		}
		if (gc_.xPressedNow())
		{
			chr_->pressedKeyX();
			if (chr_->getMenuSelect() < 10000)   // �I�������ׂĊ������Ă��Ȃ����
			{
				// NOP
			}
			else
			{
				chr_->resetSelectState();
				chr_->setMotionFlag(true);
			}
		}
		else if (gc_.zPressedNow())
		{
			chr_->pressedKeyZ();
		}
	}

	void BattleProcess::doAction(AbstractBattleCharacter *self,
		AbstractBattleCharacter *opponent,
		CharacterEnemyKind k)
	{
		if (isGameover_ || isWin_)
		{
			self->setActionCnt(0);
			return;
		}
		// �O�ɂ����A�N�V����
		if (20 < self->getActionCnt() && self->getActionCnt() <= 40)
		{
			//�@����
			if (k == CharacterEnemyKind::Character)
			{
				self->decreaseX();
			}
			// �G
			else
			{
				self->increaseX();
			}
		}
		// �U�����[�V�������J�n
		if (self->getActionCnt() == 50)
		{
			// ����
			if (k == CharacterEnemyKind::Character)
			{
				ba_->startAnimation(self->getMenuSelect(), 0, 100);
			}
			// �G
			else
			{
				// 10000�͒ʏ�U��
				ba_->startAnimation(10000, 100, 0);
			}
		}

		// ���̏u�ԃ_���[�W�A�N�V����������t���O�������Ă����� START
		int damage = 0;
		if (self->getActionFlag())	// BattleAnimation�N���X��ON�ɂȂ�
		{
			// �G�̏ꍇ
			if (k == CharacterEnemyKind::Enemy)
			{
				// �ʏ�U���I��
				self->setMenuSelect(10000);
			}

			switch (self->getMenuSelect())
			{
				case 10000:
				case 10100:
				case 20100:
				case 20200:
				case 20300:
				case 20400:
					damage = attackAction(self, opponent, k);
					ba_->setPopDamage(damage, opponent->getCenterX() - 20, opponent->getCenterY() - 20, 0);
					break;

				case 20000:
					damage = cureAction(self);
					ba_->setPopDamage(damage, self->getCenterX() - 20, self->getCenterY() - 20, 1);
					break;

				case 30000:
					// ������
					break;

				default:
					break;
			}
			self->setActionFlag(false);
			self->increaseDamageActionCnt();
		}

		// ���̏u�ԃ_���[�W�A�N�V����������t���O�������Ă����� END

		// ���ɉ�����
		if (0 < self->getDamageActionCnt() && self->getDamageActionCnt() <= 20)
		{
			// ���ɓ���
			if (k == CharacterEnemyKind::Character)
			{
				self->increaseX();
			}
			else
			{
				self->decreaseX();
			}
		}

		self->increaseActionCnt();  // �����͕K��������s
		if (self->getDamageActionCnt() > 0)
		{
			self->increaseDamageActionCnt();
		}
		if (self->getDamageActionCnt() == 30)
		{
			// �e���ڂ�������
			self->setActionCnt(0);
			self->setDamageActionCnt(0);

			self->setMenuSelect(1);
			self->setMenuSelectY(0);
			self->setMotionFlag(false);
			self->setAtbCntFlag(true);
			self->setAtbCnt(0);
		}

		// �Q�[���I�[�o�[������
		if (isGameover_ || isWin_)
		{
			self->setActionCnt(0);
			self->setDamageActionCnt(0);
		}
	}

	int BattleProcess::attackAction(
		AbstractBattleCharacter *self,
		AbstractBattleCharacter *opponent,
		CharacterEnemyKind k)
	{
		int damage = decideDamage(self, ((k == CharacterEnemyKind::Character) ? 0 : 1), self->getMenuSelect());
		if (self->getAttackMagic().flag)
		{
			damage *= 2;
		}

		opponent->setDamage(damage);  // �_���[�W���Z�b�g

		// ����̃_���[�W�|�C���g���Z�b�g
		opponent->setDamagePoint();

		// ���@�Ȃ�
		if (20000 <= self->getMenuSelect() && self->getMenuSelect() < 30000)
		{
			self->useMp();
		}
		self->changeAttackMagic();
		return damage;
	}


	int BattleProcess::cureAction(AbstractBattleCharacter *c)
	{
		int damage = decideDamage(c, 0, c->getMenuSelect());
		c->decreaseMp(9);
		c->increaseHp(damage);

		// �񕜂��������Ԃ�߂�
		if (c->getHp() > c->getHpMax())
		{
			c->setHp(c->getHpMax());
		}
		c->increaseHp(0);   // �񕜗ʂ�߂�
		c->changeCureMagic();
		return damage;
	}

	// to: 0:����->�G�A1:�G->����
	int BattleProcess::decideDamage(AbstractBattleCharacter *c, int to, int actKind)
	{
		int power = 0;
		int strength = 0;
		int weapon = 0;
		int level = 0;
		int defense = 0;
		int equip = 0;
		switch (to)
		{
			case 0:	 // �����́u�ʏ�U���v
				chr_->getOffenseParam(&power, &strength, &weapon, &level);
				mon_->getDefenseParam(&defense, &equip);
				if (actKind % 10000 == 2)	  // ���͂̐�̎�(��1�P�^��2)
				{
					power += 100 * (actKind % 10000) / 100;
				}
				if (actKind % 10000 == 100)   // ����U��
				{
					power *= 4;
				}
				break;
			case 1:	 // �G�́u�ʏ�U���v
				mon_->getOffenseParam(&power, &strength, &weapon, &level);
				chr_->getDefenseParam(&defense, &equip);
				break;
			case 2:
				break;
			case 3:
				break;
		}
		if (actKind < 40000 && actKind != 20000)  // �U��
		{
			return static_cast<int>(level * 2
				+ ( (power + static_cast<double>(weapon)) - (defense + static_cast<double>(equip)) )
				* static_cast<double>(strength) * genRandom(0.01, 0.05) + static_cast<double>(strength) * genRandom(0.2, 1.0));
		}
		if (actKind == 20000)	// ��
		{
			return static_cast<int>(c->getMagic() * 10.0 * genRandom(0.2, 1.0));
		}
		return -1;
	}

	void BattleProcess::decideGameover()
	{
		if (chr_->getHp() <= 0)
		{
			isGameover_ = true; // �Q�[���I�[�o�[
		}
	}

	void BattleProcess::decideWin()
	{

		if (mon_->getHp() <= 0)
		{
			isWin_ = true; // ����
		}
	}

	void BattleProcess::drawBattle()
	{
		bg_->drawBackground();
		bg_->drawBottomWindow();

		bg_->drawMonsterImage(mon_);
		bg_->drawCharacterImage(chr_);
	}

	void BattleProcess::displayMeter()
	{
		bg_->drawMonsterBar(mon_);
		bg_->drawCharacterBar(chr_);
	}

	// HP���ۂ̃��[�V���� ��u�Ō���悤�ɉ���
	void BattleProcess::decreaseHpAction(AbstractBattleCharacter *c)
	{
		if (isGameover_ && isWin_)
		{
			return;
		}

		if (c->getHp() - c->getDamage() < 0)
		{
			c->setHp(0);
		}
		else
		{
			int newHp = c->getHp() - c->getDamage();
			c->setHp(newHp);
		}
		c->setDamage(0);
	}

	/**
	 * @param sigma
	 * @param average
	 * @return �U��΂�x�����Ƃ��Đ��K���z����������Ԃ�
	 */
	double BattleProcess::genRandom(double sigma, double average)
	{
		static double r1, r2, s, r;
		static int valid = 0;
		double c;
		if (!valid)
		{
			r1 = DxLib::GetRand(RAND_MAX) / static_cast<double>(RAND_MAX + 1);
			r2 = DxLib::GetRand(RAND_MAX) / static_cast<double>(RAND_MAX + 1);
			s = sqrt(-2.0 * log(1.0 - r2));
		}
		valid = !valid;
		c = 2.0 * PI * r1;
		r = valid ? s * sin(c) : s + cos(c);
		return r * sigma + average;
	}

	void BattleProcess::drawGameover()
	{
		static int cnt = 0;
		bg_->drawGameover();

		cnt++;
		if (cnt == 120)
		{
			cnt = 0;
			isGameoverMoveNext_ = true;
		}
	}

	void BattleProcess::drawWin()
	{
		static int cnt = 0;

		bg_->drawWin();

		cnt++;
		if (cnt == 120)
		{
			cnt = 0;
			isWinMoveNext_ = true;
		}
	}

}
