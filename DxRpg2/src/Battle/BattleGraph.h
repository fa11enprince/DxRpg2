#pragma once

namespace Util
{
	class GameController;
	class ResourceLoader;
}
namespace Battle
{
	constexpr int  BattleStringLenMax = 32;

	constexpr int  MenuNum = 3;
	constexpr int  SubMenuNum = 5;

	constexpr char Main[MenuNum][BattleStringLenMax] =
	{ "�U��", "���@", "������" };

	constexpr char Sub[MenuNum][SubMenuNum][BattleStringLenMax] =
	{
		{ "�ʏ�U��", "�e�N�j�J���A�^�b�N", "", "", "" },
		{ "�L���A", "�G�A�[", "�t�@�C�A", "�t���[�U", "�T���_�[" },
		{ "������", "", "", "", "" }
	};

	class BattleCharacter;
	class BattleMonster;

	/**
	 * �`��n�̃N���X
	 * �U���E���@�������A�L�����N�^�[�A�����X�^�[�̃C���[�W
	 * �o�[�̕`��Ȃǂ��s��
	 */
	class BattleGraph
	{
	public:
		BattleGraph();
		~BattleGraph();
		void drawBackground() const;
		void drawBottomWindow() const;
		void drawMonsterImage(BattleMonster *mon) const;
		void drawCharacterImage(BattleCharacter *chr) const;
		void graphMonsterBar(BattleMonster *mon);
		void graphCharacterBar(BattleCharacter *chr);

		void graphMenu(BattleCharacter *chr);
		void graphGameover() const;
		void graphWin() const;

	private:
		Util::GameController &gc_;
		Util::ResourceLoader &rl_;

		void graphBar(int x, int y, int hp, int hpMax, int mp, int mpMax, int atbCnt, int dp);
		void decorateMeter(int x, int y, int h, int len);
		char *decideBoardString(char *message, BattleCharacter *chr);
	};
}
