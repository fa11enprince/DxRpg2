#define _CRT_SECURE_NO_WARNINGS
#include "BattleGraph.h"
#include "Battle/BattleCharacter.h"
#include "Battle/BattleMonster.h"
#include "Util/GameController.h"
#include "Util/ResourceLoader.h"
#include "StdCommon.h"

namespace Battle
{
	BattleGraph::BattleGraph():
		gc_(Util::GameController::getInstance()),
		rl_(Util::ResourceLoader::getInstance())
	{
	}

	BattleGraph::~BattleGraph()
	{
	}

	void BattleGraph::drawBackground() const
	{
		DxLib::DrawGraph(0, 0, rl_.getHdlImgBackGround(0), FALSE);
	}

	void BattleGraph::drawBottomWindow() const
	{
		DxLib::DrawGraph(0, WindHeight - 134, rl_.getHdlImgBackGround(10), FALSE);
	}


	void BattleGraph::drawMonsterImage(BattleMonster *mon) const
	{
		DxLib::DrawGraph(
			mon->getX(),
			mon->getY(),
			rl_.getHdlImgMonster(mon->getKind()),
			TRUE);
	}

	void BattleGraph::drawCharacterImage(BattleCharacter *chr) const
	{
		DxLib::DrawGraph(
			chr->getX(),
			chr->getY(),
			rl_.getHdlImgChar(chr->getKind(), 5),
			TRUE);
	}

	void BattleGraph::drawMonsterBar(BattleMonster *mon)
	{
		// ���[�^�̊J�n�ʒu�̍��W
		const int MonMtX = 15;
		const int MonMtY = 370;
		drawBar(MonMtX, MonMtY,
			mon->getHp(), mon->getHpMax(),
			mon->getMp(), mon->getMpMax(),
			mon->getAtbCnt(), -1);
	}

	void BattleGraph::drawCharacterBar(BattleCharacter *chr)
	{
		// ���[�^�̊J�n�ʒu�̍��W
		const int ChrMtX = 250;
		const int ChrMtY = 370;
		drawBar(ChrMtX, ChrMtY,
			chr->getHp(), chr->getHpMax(),
			chr->getMp(), chr->getMpMax(),
			chr->getAtbCnt(), chr->getDamagePoint()
		);
	}

	void BattleGraph::drawBar(int x, int y, int hp, int hpMax,
		int mp, int mpMax, int atbCnt, int damagePoint)
	{
		const int MeterHeight = 5;
		const int OffsetY = 25;

		// HP���[�^�̘g
		DxLib::DrawBox(
			x,
			y,
			x + 150,
			y + MeterHeight,
			DxLib::GetColor(0, 255, 255), FALSE);
		// HP���[�^�̒��g
		DxLib::DrawBox(
			x,
			y,
			x + 150 * hp / hpMax,
			y + MeterHeight,
			DxLib::GetColor(0, 255, 255), TRUE);
		// HP���[�^�[�̒��g
		DxLib::DrawBox(
			x + 150 + hp / hpMax,
			y,
			x + 150 * hp / hpMax,
			y + MeterHeight,
			DxLib::GetColor(200, 0, 0), TRUE);
		decorateMeter(x, y, MeterHeight, 150 * hp / hpMax);

		// MP���[�^�̘g
		DxLib::DrawBox(
			x,
			y + OffsetY,
			x + 150,
			y + OffsetY + MeterHeight,
			DxLib::GetColor(200, 200, 0), FALSE);
		// MP���[�^�̒��g
		DxLib::DrawBox(
			x,
			y + OffsetY,
			x + 150 * mp / mpMax,
			y + OffsetY + MeterHeight,
			DxLib::GetColor(200, 0, 0), TRUE);
		decorateMeter(x, y + OffsetY, MeterHeight, 150 * mp / mpMax);

		// ATB���[�^�[
		int barColorAtb = DxLib::GetColor(50, 150, 150);  // �D�F
		// ATB���[�^�[�̘g��`��
		DxLib::DrawBox(
			x,
			y + (OffsetY * 2),
			x + 150,
			y + (OffsetY * 2) + MeterHeight,
			barColorAtb, FALSE);
		// ATB���[�^�[�̒��g�`��
		DxLib::DrawBox(
			x,
			y + (OffsetY * 2),
			x + 150 * atbCnt / AtbCntMax,
			y + (OffsetY * 2) + MeterHeight,
			barColorAtb, TRUE);

		// DP(�_���[�W�|�C���g)
		int barColorDp = DxLib::GetColor(100, 100, 100);
		if (damagePoint == DamagePointMax)
		{
			// ���C���{�[
			barColorDp = DxLib::GetColor(
				(gc_.getGCount() * 3) % 256,
				(gc_.getGCount() * 3 + 85) % 256,
				(gc_.getGCount() * 3 + 170) % 256);
		}
		if (damagePoint != -1) {
			// DP���[�^�[�̘g��`��
			DxLib::DrawBox(
				x,
				y + (OffsetY * 3),
				x + 150,
				y + (OffsetY * 3) + MeterHeight,
				DxLib::GetColor(100, 100, 100), FALSE);

			// DP���[�^�[�̒��g��`��
			if (damagePoint < DamagePointMax)
			{
				DxLib::DrawBox(
					x,
					y + (OffsetY * 3),
					x + 150 * damagePoint / DamagePointMax,
					y + (OffsetY * 3) + MeterHeight,
					barColorDp, TRUE);
			}
			else
			{
				DxLib::DrawBox(
					x,
					y + (OffsetY * 3),
					x + 150 * damagePoint / DamagePointMax,
					y + (OffsetY * 3) + MeterHeight,
					barColorDp, TRUE);
			}
		}

		DxLib::DrawFormatStringToHandle(x, y + 4, DxLib::GetColor(255, 255, 255),
			rl_.getHdlFont(0), "HP %4d/%4d", hp, hpMax);  // HP���l�\��
		DxLib::DrawFormatStringToHandle(x, y + OffsetY + 4, DxLib::GetColor(255, 255, 255),
			rl_.getHdlFont(0), "MP %4d/%4d", mp, mpMax);  // MP���l�\��
		DxLib::DrawFormatStringToHandle(x, y + (OffsetY * 2) + 4, DxLib::GetColor(255, 255, 255),
			rl_.getHdlFont(0), "ATB");
		if (damagePoint == DamagePointMax) {
			DxLib::DrawFormatStringToHandle(x, y + (OffsetY * 3) + 8, barColorDp,
				rl_.getHdlFont(1), "���~�b�g�u���C�N");  // DP
		}
	}

	// ������E�ɗ���锒�������X�[���ƃ��[�^�[�Ƀf�R���[�g
	void BattleGraph::decorateMeter(int x, int y, int h, int len)
	{
		const int MdLen = 50;
		// �[�����֎~
		if (len == 0)
		{
			return;
		}
		int x2 = x + gc_.getGCount() % len;
		for (int i = 0; i < MdLen; i++)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 / MdLen * i);
			DrawLine(x2, y, x2, y + h, DxLib::GetColor(255, 255, 255));
			x2++;
			if (x2 >= x + len)
			{
				x2 = x;
			}
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// �o�g���̃��j���[
	void BattleGraph::drawMenu(BattleCharacter *chr)
	{
		const int OffsetX = 500;
		const int OffsetY = 370;

		// �I��g
		DxLib::DrawGraph(WindWidth - 180, WindHeight - 134, rl_.getHdlImgBackGround(11), FALSE);
		// ���C�����j���[
		// �I����Ԃ��ŏ��̏��
		if (chr->getMenuSelect() < 100)
		{
			int y;
			y = chr->getMenuSelectY(); // �`�悙�ʒu�����ɃZ�b�g
			for (int i = 0; i < MenuNum; i++)
			{
				int x = 0;
				//���ݑI������Ă��鍀�ڂ͎኱���ɂ���
				if (i == y)
				{
					x = -5;
				}
				// ������\��
				DxLib::DrawFormatStringToHandle(
					OffsetX + x,
					OffsetY + 18 * i,
					DxLib::GetColor(255, 255, 255),
					rl_.getHdlFont(1), "%s", Main[i]);
			}
		}

		// �T�u���j���[
		for (int i = 0; i < MenuNum; i++)
		{
			// �키 ���@ �A�r���e�B��100���Ƃɂ킩��Ă���
			if (chr->getMenuSelect() >= 100 + 100 * i
				&& chr->getMenuSelect() < 200 + 100 * i)
			{
				int y = chr->getMenuSelectY();
				for (int j = 0; j < SubMenuNum; j++)
				{
					int x = 0;
					// �I�����Ă�����͍��ɓ�����
					if (y == j)
					{
						x = -5;
					}
					// ������\��
					DxLib::DrawFormatStringToHandle(OffsetX + x, OffsetY + 18 * j,
						DxLib::GetColor(255, 255, 255),
						rl_.getHdlFont(1), "%s", Sub[i][j]);
				}
				// �U����
				if (i == 0) {
					if (chr->getDamagePoint() == DamagePointMax) {
						int x = 0;
						const int InsertMenuPositionY = 2;
						if (y == InsertMenuPositionY)
						{
							x = -5;
						}
						DxLib::DrawFormatStringToHandle(OffsetX + x, OffsetY + 18 * InsertMenuPositionY,
							DxLib::GetColor(255, 255, 255),
							rl_.getHdlFont(1), "%s", "���~�b�g�u���C�N");
					}
				}
			}
		}

		// ���J�[�\��
		DxLib::DrawGraph(
			OffsetX - 20 + (gc_.getGCount() % 20) / 4 - 5,  // �Ԃ�Ԃ�k���鉉�o
			OffsetY + 18 * chr->getMenuSelectY() + 1,
			rl_.getHdlImgCursor(), TRUE);
		// ���b�Z�[�W��
		DxLib::DrawGraph(10, 5, rl_.getHdlImgEtc(1), TRUE);
		// ���b�Z�[�W��������
		char str[MessageLenMax] = { 0 };
		DxLib::DrawFormatString(14, 7, DxLib::GetColor(255, 255, 255),
			"%s", decideBoardString(str, chr));
	}

	char *BattleGraph::decideBoardString(char *message, BattleCharacter *chr)
	{
		if (chr->getAtbCnt() == AtbCntMax
			&& chr->getMenuSelect() < 10000)
		{
			switch (chr->getMenuSelect())
			{
			case 1:
				strcpy(message, "�U�����s���܂��B");
				break;
			case 2:
				strcpy(message, "���@�U�����s���܂��B");
				break;
			case 3:
				strcpy(message, "�퓬���瓦���܂��B");
				break;

			case 100:
				strcpy(message, "�ʏ�U�����s���܂��B");
				break;
			case 101:
				strcpy(message, "����U�����s���܂��B");
				break;

			case 200:
				strcpy(message, "MP9����ĉ񕜖��@���s���܂��B");
				break;
			case 201:
				strcpy(message, "MP8����ĕ��̖��@�U�����s���܂��B");
				break;
			case 202:
				strcpy(message, "MP16����ĉ΂̖��@�U�����s���܂��B");
				break;
			case 203:
				strcpy(message, "MP24����ĕX�̖��@�U�����s���܂��B");
				break;
			case 204:
				strcpy(message, "MP36����ė��̖��@�U�����s���܂��B");
				break;

			case 300:
				strcpy(message, "�퓬���瓦���܂��B");
				break;
			default:
				strcpy(message, "");
				break;
			}
		}
		return message;
	}

	void BattleGraph::drawGameover() const
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DxLib::DrawBox(0, 0, 640, 480, DxLib::GetColor(255, 0, 0), TRUE);
		DxLib::DrawString(300, 230, "GAME OVER", DxLib::GetColor(255, 255, 255));
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}

	void BattleGraph::drawWin() const
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DxLib::DrawBox(0, 0, 640, 480, DxLib::GetColor(0, 255, 255), TRUE);
		DxLib::DrawString(300, 230, "WON!!", DxLib::GetColor(255, 255, 255));
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
