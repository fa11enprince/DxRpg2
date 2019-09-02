#pragma once
#include "StdCommon.h"

namespace Battle
{
	const int BoardMax = 100;

	class Board
	{
	private:
		struct Board_
		{
			bool flag;
			int cnt;
			char message[MessageLenMax];

			Board_()
				: flag(false), cnt(0)
			{
				for (int i = 0; i < sizeof(message); i++)
				{
					message[i] = 0;
				}
			}
		};
		Board_ board_[BoardMax];

	public:
		Board();
		~Board();

		void graph();
		void add(char *message);
	};
}
