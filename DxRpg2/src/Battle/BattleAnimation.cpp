#include "StdCommon.h"
#include "Battle/BattleAnimation.h"
#include "Battle/BattleCharacter.h"
#include "Battle/BattleMonster.h"
#include "Util/ResourceLoader.h"

namespace Battle
{

	// C++�̃R���X�g���N�^�ŎQ�Ɠn��������Ɓu�Q�Ƃ����������ꂸ�ɐ錾����Ă��܂��v�Əo��
	// https://www.hamayanhamayan.com/entry/2017/11/27/200917
	BattleAnimation::BattleAnimation():
		rl_(Util::ResourceLoader::getInstance())
	{
	}

	BattleAnimation::~BattleAnimation()
	{
	}

	// �U���A�j���[�V�����̊J�n
	void BattleAnimation::startAnimation(int selected, int target, int actor)
	{
		// �g���Ă��Ȃ��A�j���[�V������T��
		int i = 0;
		for (i = 0; i < AnimationMax - 1; i++)   // 1���炷
		{
			// �t���O�������Ă��Ȃ�
			if (!animation_[i].flag)
			{
				break;
			}
		}
		animation_[i].flag = true;
		animation_[i].target = target;   // ���� 100:����,  0:�G
		animation_[i].kind = selected;   // menuSelect()�̒l
		animation_[i].cnt = 0;
		animation_[i].actor = actor;     // ���� 100:����,  0:�G

	}

	void BattleAnimation::drawAnimation(BattleCharacter *chr, BattleMonster *mon)
	{
		for (int i = 0; i < AnimationMax; i++)
		{
			if (animation_[i].flag)
			{
				int k = animation_[i].kind / 10000;
				switch (k)  // �A�j���[�V�����̎�ނɂ���ď����𕪊�
				{
					case 1:  // �U��
						// �^�[�Q�b�g���G
						if (animation_[i].target == 0)	// �^�[�Q�b�g���Gno.0��������
						{
							animation0(i, chr, mon->getX() + 5, mon->getY() + 5);
						}
						// �^�[�Q�b�g������
						if (animation_[i].target == 100)  // �^�[�Q�b�g������no.100�Ȃ�
						{
							animation0(i, mon, chr->getX() - 45, chr->getY() - 30);
						}
						break;
					case 2:  // ���@
						// �񕜖��@
						if (animation_[i].kind == 20000)
						{
							// �^�[�Q�b�g���G
							if (animation_[i].actor == 0)
							{
								animation1(mon, i, 0, 0);
							}
							// �^�[�Q�b�g������
							else if (animation_[i].actor == 100)
							{
								animation1(chr, i, -30, 20);
							}
						}
						// �U�����@
						else
						{
							// �G
							if (animation_[i].target == 0)	// �^�[�Q�b�g���Gno.0��������
							{
								animation0(i, chr, mon->getX() + 5, mon->getY() + 5);
							}
							// ����
							if (animation_[i].target == 100)  // �^�[�Q�b�g������no.0�Ȃ�
							{
								animation0(i, mon, chr->getX() - 32, chr->getY() - 16);
							}
						}
						break;
					default:
						exit(9999);
						break;
				}
			}
		}
		drawDamage();   // Animation�̌��Damage
	}

	void BattleAnimation::drawDamage()
	{
		// �r�������ł��Ă��悢 break���Ȃ��B
		for (int i = 0; i < PopDamMax; i++)
		{
			if (popDamage_[i].flag)
			{
				calcDamage(i);
				popDamage_[i].cnt++;
				// �����Ă���120(2�b)��������t���O�𗎂Ƃ�
				if (popDamage_[i].cnt == 120)
				{
					popDamage_[i].flag = false;
				}
			}
		}
	}

	void BattleAnimation::calcDamage(int t)
	{
		// �ʒu���� �h��Đ������o��l�q��\��
		const int ArraySize = 20;
		int y[ArraySize] =
		{
			-1, -3, -4, -5, -4, -3, -1, 1, 3, 7,
			11, 15, 22, 16, 13, 12, 13, 16, 22, 21
		};
		char damage[6];
		sprintf(damage, "%d", popDamage_[t].damage);
		int cy;
		// �_���[�W�̌����������[�v
		for (int i = 0; i < static_cast<int>(strlen(damage)); i++)
		{
			if (popDamage_[t].cnt - 4 * i < 0)
			{
				return;
			}
			if (popDamage_[t].cnt - 4 * i < ArraySize)
			{
				cy = y[popDamage_[t].cnt - 4 * i];
			}
			else
			{
				cy = 22;
			}
			// �������G�Ɩ����Ŕ����Ɉʒu���Ⴄ�ς���ׂ�
			// �|�b�v�_���[�W�`��
			DxLib::DrawGraph(popDamage_[t].x + 15 * i,
				popDamage_[t].y + cy - popDamage_[t].type * 14,
				rl_.getHdlImgNumber(popDamage_[t].type, damage[i] - '0'),
				TRUE);
		}
	}

	void  BattleAnimation::animation0(int idx, AbstractBattleCharacter *c, int x, int y)
	{
		int attackKind = animation_[idx].kind / 10000;

		// ����A�j���[�V�����`��
		if (animation_[idx].cnt < 64)
		{
			if (attackKind == 1)  // �����U��
			{
				int kind = animation_[idx].cnt / 13;  // 64��12�Ŋ����0-4�ɂ΂炯��̂ł������Ă���
				DxLib::DrawGraph(x, y, rl_.getSlashImage(kind), TRUE);
			}
			else if (attackKind == 2)  // �U�����@
			{
				int subKind = animation_[idx].kind % 10000 / 100;
				int kind = animation_[idx].cnt / 8;  // 64��8�Ŋ����0-7�ɂ΂炯��̂ł������Ă���
				switch (subKind) {
				case 1:  // wind
					DxLib::DrawGraph(x, y, rl_.getWindImage(kind), TRUE);
					break;
				case 2:  // fire
					DxLib::DrawGraph(x, y, rl_.getFireImage(kind), TRUE);
					break;
				case 3:  // ice
					DxLib::DrawGraph(x, y, rl_.getIceImage(kind), TRUE);
					break;
				case 4:  // thunder
					DxLib::DrawGraph(x, y, rl_.getThunderImage(kind), TRUE);
					break;
				}
			}
		}
		animation_[idx].cnt++;

		if (animation_[idx].cnt == 64)  // ���� -> �����
		{
			animation_[idx].flag = false;
			c->setActionFlag(true);  // �d�v
			animation_[idx].cnt = 0;
		}

	}

	// �L���A
	void  BattleAnimation::animation1(AbstractBattleCharacter *c, int idx, int x, int y)
	{
		if (animation_[idx].cnt < 64)
		{
			DxLib::DrawGraph(c->getX() + x,
				c->getY() + y - animation_[idx].cnt,
				rl_.getCureImage(animation_[idx].cnt/ 8),
				TRUE);
		}
		animation_[idx].cnt++;

		if (animation_[idx].cnt == 64)
		{
			animation_[idx].flag = false;
			c->setActionFlag(true);
			animation_[idx].cnt = 0;
		}
	}

	void BattleAnimation::setPopDamage(int damage, int x, int y, int type)
	{
		int i = 0;
		for (i = 0; i < PopDamMax - 1; i++)   // 1���炷
		{
			if (!popDamage_[i].flag) { break; }
		}
		popDamage_[i].flag = true;
		popDamage_[i].cnt = 0;
		popDamage_[i].damage = damage;
		popDamage_[i].x = x;
		popDamage_[i].y = y;
		popDamage_[i].type = type;  // 1: cure, 0: others
	}

}