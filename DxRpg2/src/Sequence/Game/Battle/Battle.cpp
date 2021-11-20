#include "Sequence/Game/Battle/Battle.h"
#include "Util/GameController.h"
#include "Util/ResourceLoader.h"
#include "Sequence/Parent.h"
#include "Battle/BattleProcess.h"
#include "Battle/BattleCharacter.h"
#include "Battle/BattleMonster.h"

namespace Sequence
{
	namespace Game
	{
		namespace Battle
		{

			Battle::Battle(std::shared_ptr<::SharedCharacterStatus> sharedStatus) :
				gc_(Util::GameController::getInstance()),
				rl_(Util::ResourceLoader::getInstance()),
				chr_(std::make_unique<::Battle::BattleCharacter>(sharedStatus)),
				mon_(std::make_unique<::Battle::BattleMonster>()),
				randX_{}, randY_{}, startBattleX_{}, startBattleY_{}
			{
				// �L������ʂ��O����ݒ�
				chr_->setKind(0);
				mon_->setKind(0);

				// ���S���O����ݒ�
				int axisChrX, axisChrY;
				DxLib::GetGraphSize(
					rl_.getHdlImgChar(chr_->getKind(), 5),
					&axisChrX, &axisChrY);
				chr_->setCenterX(chr_->getX() + axisChrX / 2);
				chr_->setCenterY(chr_->getY() + axisChrY / 2);

				int axisMonX, axisMonY;
				DxLib::GetGraphSize(
					rl_.getHdlImgMonster(mon_->getKind()),
					&axisMonX, &axisMonY);
				mon_->setCenterX(mon_->getX() + axisMonX / 2);
				mon_->setCenterY(mon_->getY() + axisMonY / 2);

				bp_ = std::make_unique<::Battle::BattleProcess>(chr_.get(), mon_.get());

				for (int i = 0; i < ImgCharDivNum; i++)
				{
					startBattleX_[i] = 0;
					startBattleY_[i] = 0;
					randX_[i] = 0;
					randY_[i] = 0;
				}

				nextSequence_ = NextSequence::NextBattleShatter;   // �ŏ��̓R��
			}

			Battle::~Battle()
			{
			}

			// �o�g���ɂ����āA�ǂ̊֐��ɏ�����n�����𐧌�
			void Battle::update(Sequence::Game::Parent *parent)
			{
				switch (nextSequence_)
				{
				case NextSequence::NextBattleShatter:  // ��ʂ�����ė�����V�[��
					startBattleShatter();
					break;
				case NextSequence::NextBattleEnter:  // �퓬�˓��V�[��
					startBattleEnter();
					break;
				case NextSequence::NextBattleMain:  // �퓬�V�[��
					bp_->main();    // �퓬���C������
					if (bp_->getWinMoveNext())
					{
						nextSequence_ = NextSequence::NextMap;
					}
					if (bp_->getGameoverMoveNext())
					{
						nextSequence_ = NextSequence::NextRestart;
					}
					break;
				case NextSequence::NextBattleDebug:  // DEBUG�p
					//debugBattleMode();
					break;
				case NextSequence::NextMap:
					parent->moveTo(Parent::NextSequence::NextMapMain);
					break;
				case NextSequence::NextRestart:
					parent->moveTo(Parent::NextSequence::NextRestart);
					break;
				default:
					exit(EXIT_FAILURE);
					break;
				}
			}

			void Battle::startBattleShatter()
			{
				const double Accel = 1.5;
				const int ShatterNum = 8;
				const int DownSt = 60;
				const int DownEd = 150;
				static int cnt = 0;

				if (cnt == 0)
				{
					// �ŏ��ɏ������s��ꂽ���ADivNum*DivNum�̉摜�̏������W��ݒ�
					for (int i = 0; i < DivisionNum; i++)
					{
						for (int j = 0; j < DivisionNum; j++)
						{
							startBattleX_[i * DivisionNum + j] = WindWidth / DivisionNum * static_cast<double>(j);   // �`�悷��e�����W
							startBattleY_[i * DivisionNum + j] = WindHeight / DivisionNum * static_cast<double>(i);  // �`�悷��e�����W
							randX_[i * DivisionNum + j] = DxLib::GetRand(30);   // �΂炯�����邽�߂̗���
							randY_[i * DivisionNum + j] = DxLib::GetRand(30);   // �΂炯�����邽�߂̗���
						}
					}
				}
				// �ӂ��U�郂�[�h(�O�֎U��΂���)
				if (cnt < ShatterNum)
				{
					// ShatterNum�܂ł̊Ԓ��S����O�֎U��΂点��
					for (int i = 0; i < DivisionNum; i++)
					{
						for (int j = 0; j < DivisionNum; j++)
						{
							// �l�������ɔ�юU�点��
							startBattleX_[i * DivisionNum + j]
								+= (randX_[i * DivisionNum + j] + 30 * static_cast<double>(j) - 45) / 8.0;
							startBattleY_[i * DivisionNum + j]
								+= (randY_[i * DivisionNum + j] + 30 * static_cast<double>(i) - 45) / 8.0;
						}
					}
				}
				// �����鎞�̗�������
				if (cnt == ShatterNum)
				{
					// �����鎞�ɂ΂炯�����邽�߂̗�������
					for (int i = 0; i < DivisionNum; i++)
					{
						for (int j = 0; j < DivisionNum; j++)
						{
							randX_[i * DivisionNum + j] = GetRand(40) / 10.0 - 2.0;
							randY_[i * DivisionNum + j] = GetRand(10) / 10.0 + 1.0;
						}
					}
				}

				// �����郂�[�h
				if (cnt >= DownSt && cnt <= DownEd)
				{
					// �����郂�[�V����
					double t = (cnt - DownSt) * 17 / 1000;
					for (int i = 0; i < DivisionNum; i++)
					{
						for (int j = 0; j < DivisionNum; j++)
						{
							startBattleX_[i * DivisionNum + j]
								+= randX_[i * DivisionNum + j];
							// �����鎞�����x�������Čv�Z�BAccel�̒l�ŕύX�\
							startBattleY_[i * DivisionNum + j]
								+= t * t * Accel * randY_[i * DivisionNum + j] / 2.0 * 10;
						}
					}
				}
				// 150�ɂȂ����玟�̃V�[����
				if (cnt == DownEd)
				{
					cnt = 0;
					nextSequence_ = NextSequence::NextBattleEnter;
					return;
				}
				// �����������ꂼ��`��
				for (int i = 0; i < ImgCharDivNum; i++)
				{
					DxLib::DrawGraph(static_cast<int>(startBattleX_[i]),
						static_cast<int>(startBattleY_[i]),
						rl_.getHdlImgStartBattle(i), FALSE);
				}
				cnt++;
			}

			void Battle::startBattleEnter()
			{
				const double StartBattleTime = 120.0;
				const int Stage1 = 80;
				const int Stage2 = 170;
				const int Stage3 = 255;
				static int cnt = 0;

				if (cnt == 0)
				{
					// �I�[�v�j���O2���n�܂�u�ԁA��U�S���`�悵�āA
					// ���̉�ʂ��L���v�`�����Ĕ��f������O�ɃN���A����B
					// ���̂P���̃L���v�`�������摜�ŃI�[�v�j���O�����
					DxLib::DrawGraph(0, 0, rl_.getHdlImgBackGround(0), FALSE);

					// ��l���E�������摜�̓Ǎ� 0, 5
					DxLib::DrawGraph(::Battle::BattleCharPosX1, ::Battle::BattleCharPosY1,
						rl_.getHdlImgChar(chr_->getKind(), 5), TRUE);

					// �G�L�����Ǎ�
					DxLib::DrawGraph(::Battle::BattleMonPosX1, ::Battle::BattleMonPosY1,
						rl_.getHdlImgMonster(mon_->getKind()), TRUE);

					rl_.setImageStartBattle(0, WindWidth, WindHeight);

					// �摜�L���v�`��
					DxLib::GetDrawScreenGraph(0, 0, WindWidth, WindHeight,
						rl_.getHdlImgStartBattle(0));
					DxLib::ClearDrawScreen();
				}
				if (cnt > Stage1 && cnt <= Stage2)
				{
					DxLib::DrawRotaGraph(-50 + cnt, 100, 2.0, 0, rl_.getHdlImgStartBattle(0), FALSE);
				}
				if (cnt > Stage2 && cnt <= Stage3)
				{
					DxLib::DrawRotaGraph(500 - cnt, 400, 2.0, 0, rl_.getHdlImgStartBattle(0), FALSE);
				}
				if (cnt > Stage3 && cnt <= Stage3 + StartBattleTime)
				{
					int t = cnt - Stage3;
					DxLib::DrawExtendGraph(
						static_cast<int>((WindWidth / 2 + WindWidth / DivisionNum)
							- (WindWidth / 2 + static_cast<double>(WindWidth / DivisionNum))
							* static_cast<double>(t / StartBattleTime)),
						0,
						static_cast<int>(WindWidth * static_cast<double>(t) / StartBattleTime),
						static_cast<int>(WindHeight * static_cast<double>(t) / StartBattleTime),
						rl_.getHdlImgStartBattle(0),
						TRUE
					);
				}
				if (cnt == Stage3 + StartBattleTime)
				{	// ���ԂɂȂ�����I�[�v�j���O���I�����Đ퓬��
					cnt = 0;
					nextSequence_ = NextSequence::NextBattleMain;
					return;
				}
				cnt++;
			}
		}
	}
	
}
