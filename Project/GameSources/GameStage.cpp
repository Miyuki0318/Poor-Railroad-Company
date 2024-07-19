/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "BridgeManager.h"
#include "Crossing.h"
#include "GameTrain.h"
#include "GameStage.h"
#include "GamePlayer.h"
#include "GatheringObject.h"
#include "GatherTreasure.h"
#include "Gear.h"
#include "GoalGuide.h"
#include "GroundManager.h"
#include "ItemCountUI.h"
#include "MainCamera.h"
#include "PauseMenu.h"
#include "RailGuideBlinking.h"
#include "RailGuideIcon.h"
#include "RailManager.h"
#include "SkyBox.h"
#include "UnBreakRock.h"
#include "CraftPosshibleGuide.h"
#include "Instruction.h"
#include "Station.h"

namespace basecross
{
	// ���\�[�X�̓ǂݍ���
	void GameStage::CreateResourses()
	{
		BaseStage::CreateResourses();

		// �A�v���P�[�V�����I�u�W�F�N�g�̎擾
		const auto& app = App::GetApp();

		// �f�B���N�g���p�X�̒�`
		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";
		wstring soundPath = mediaPath + L"Sounds/";

		// �Q�[�����ʃe�N�X�`��
		AddTextureResource(L"GAMECLEAR_TX", texturePath + L"Win.png");
		AddTextureResource(L"GAMEOVER_TX", texturePath + L"Lose.png");
		AddTextureResource(L"CONTINUE_TX", texturePath + L"Continue.png");
		AddTextureResource(L"STAGESELECT_TX", texturePath + L"StageSelect.png");
		AddTextureResource(L"CONTINUE_TITLEBACK_TX", texturePath + L"TitleBack.png");
		AddTextureResource(L"NEXTSTAGE_TX", texturePath + L"NextStage.png");
		AddTextureResource(L"CLEAR_TITLEBACK_TX", texturePath + L"ClearTitleBack.png");
		AddTextureResource(L"OVER_CONTINUE_TX", texturePath + L"ContinueTrain.png");
		AddTextureResource(L"OVER_TITLEBACK_TX", texturePath + L"TitleBackTrain.png");
		AddTextureResource(L"RAIL_LINE_TX", texturePath + L"RailLine.tga");
		AddTextureResource(L"PAYMENTS_MENU_TX", texturePath + L"PaymentsMenu.tga");

		// �S�[���K�C�h�e�N�X�`��
		AddTextureResource(L"GOAL_GUIDE_TX", texturePath + L"GoalGuide.png");
		AddTextureResource(L"GOAL_ARROW_TX", texturePath + L"GoalGuideArrow.png");

		// ���[���K�C�h�e�N�X�`��
		AddTextureResource(L"GUIDE_RAIL_TX", texturePath + L"RailGuide.png");

		// ������@�e�N�X�`��
		AddTextureResource(L"INST_CRAFT_K", texturePath + L"CraftInstruction_K.png");
		AddTextureResource(L"INST_NORMAL_K", texturePath + L"NormalInstruction_K.png");
		AddTextureResource(L"INST_CRAFT_X", texturePath + L"CraftInstruction_X.png");
		AddTextureResource(L"INST_NORMAL_X", texturePath + L"NormalInstruction_X.png");

		// �N���t�g�E�B���h�E�̃e�N�X�`��
		AddTextureResource(L"C_WINDOW_TX", texturePath + L"CraftWindow.png");

		// QTE�o�[�̃t���[���̃e�N�X�`��
		AddTextureResource(L"C_QTE_FLAME_TX", texturePath + L"BarFlame.png");
		AddTextureResource(L"C_QTE_POINT_TX", texturePath + L"QTEPoint.png");

		// �N���t�g�A�C�e���A�C�R���e�N�X�`��
		AddTextureResource(L"PAD_RAIL_TX", texturePath + L"CraftRailIcon.png");
		AddTextureResource(L"PAD_BRIDGE_TX", texturePath + L"CraftBridgeIcon.png");
		AddTextureResource(L"PAD_CROSSING_TX", texturePath + L"CraftCrossingIcon.png");

		AddTextureResource(L"MOUSE_RAIL_TX", texturePath + L"Mouse_CraftRailIcon.png");
		AddTextureResource(L"MOUSE_BRIDGE_TX", texturePath + L"Mouse_CraftBridgeIcon.png");
		AddTextureResource(L"MOUSE_CROSSING_TX", texturePath + L"Mouse_CraftCrossingIcon.png");

		// �Q�[������BGM
		AddAudioResource(L"LV1_BGM", soundPath + L"FirstBGM");
		AddAudioResource(L"LV2_BGM", soundPath + L"SecondBGM");
		AddAudioResource(L"LV3_BGM", soundPath + L"ThirdBGM");
		AddAudioResource(L"FOURTH_BGM", soundPath + L"FourthBGM");
		AddAudioResource(L"FIFTH_BGM", soundPath + L"FifthBGM");

		// �Q�[������SE
		AddAudioResource(L"PAUSE_OPEN_SE", soundPath + L"PauseOpen");
		AddAudioResource(L"PAUSE_CLOSE_SE", soundPath + L"PauseClose");
		AddAudioResource(L"TRAIN_DERAIL_SE", soundPath + L"DeRailed");
		AddAudioResource(L"POSSHIBLE_SE", soundPath + L"CraftPosshible");

		// �ǉ��������\�[�X���������ɒǉ�
		AddedTextureResources();
		AddedAudioResources();
	}

	// BGM�̍Đ�
	void GameStage::CreateStartBGM()
	{
		// BGM�̃L�[���X�e�[�W�p�X����ݒ�
		wstring bgmKey;
		Util::ConvertUtf8toWstring(m_stagePath, bgmKey);

		// BGM�̍Đ�
		m_bgmItem = m_soundManager->StartBGM(Utility::ToUpperString(bgmKey) + L"_BGM", XAUDIO2_LOOP_INFINITE, 0.0f, ThisPtr);
	}

	//�r���[�ƃ��C�g�̐���
	void GameStage::CreateViewLight()
	{
		Vec3 stationPos = GetSharedGameObject<Station>(L"Station")->GetPosition();
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		Vec3 startEye = Vec3(m_DefStartEye.x + m_stageDistanceX, m_DefStartEye.y, m_DefStartEye.z);

		auto PtrCamera = ObjectFactory::Create<MainCamera>(MainCamera::State::Follow, stationPos, startEye, m_DefStartAt);
		PtrView->SetCamera(PtrCamera);
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	// ���{�b�N�X�̐���
	void GameStage::CreateGroundBox()
	{
		const float scale = 1.0f;
		const auto& ground = AddGameObject<GroundManager>(scale);	// �n�ʂ̕`�搶��
		SetSharedGameObject(L"GroundManager", ground);

		const auto& treasure = AddGameObject<GatherTreasure>();
		SetSharedGameObject(L"GatherTreasure", treasure);

		const auto& unbreak = AddGameObject<UnBreakRock>();	// �󂹂Ȃ���̐���
		SetSharedGameObject(L"UnBreakRock", unbreak);
	}

	// �v���C���[�̐���
	void GameStage::CreatePlayer()
	{
		// �v���C���[�I�u�W�F�N�g�̒ǉ�
		const auto& player = AddGameObject<GamePlayer>(m_startPosition, m_goalStagingPosition);

		// �V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"Player", player);
	}

	// csv�ł̃X�e�[�W����
	void GameStage::CreateStageCSV(string csvPath)
	{
		// CSV�f�[�^(int�^�̓񎟌��z��)
		m_positionMap.clear();
		m_stageMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Stage"));
		m_groundMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Ground"));

		// ��d���[�v
		for (int i = 0; i < m_stageMap.size(); i++)
		{
			vector<Vec3> tempVec;
			for (int j = 0; j < m_stageMap.at(i).size(); j++)
			{
				// ID���擾
				int& id = m_stageMap.at(i).at(j);
				eStageID num = STAGE_ID(id);

				// ���W(����+��ԍ��~�X�P�[��,����+�s��-�s�ԍ��~�X�P�[��)
				tempVec.push_back(ROWCOL2POS(i, j));

				// �΂�ID�Ȃ�
				if (num == eStageID::Stone1)
				{
					int random = Utility::RangeRand(3, 0);
					id = id + random;
				}

				// �؂�ID�Ȃ�
				if (num == eStageID::Tree1)
				{
					int random = Utility::RangeRand(2, 0);
					id = id + random;
				}

				// �v���C���[�̊J�n�ʒu�ƃN���A���o���̈ړ����ێ�
				if (num == eStageID::PlayerStart)
				{
					m_startPosition = Vec3(float(j), 2.0f, -float(i));
					id = 0;
				}
				if (num == eStageID::PlayerGoal)
				{
					m_goalStagingPosition = Vec3(float(j), 2.0f, -float(i));
					id = 0;
				}

				// ��[���[���Ȃ�z��̍s�ԍ����f�B�X�^���X�ɂ���
				if (num == eStageID::StartRail)
				{
					m_stageDistanceX = float(j);
				}
			}
			m_positionMap.push_back(tempVec);
		}
	}

	void GameStage::CreateRailManager()
	{
		const auto& railManager = AddGameObject<RailManager>();
		SetSharedGameObject(L"RailManager", railManager);

		AddGameObject<RailGuideIcon>();
		AddGameObject<RailGuideBlinking>();
	}

	void GameStage::CreateBridgeManager()
	{
		const auto& bridgeManager = AddGameObject<BridgeManager>();
		SetSharedGameObject(L"BridgeManager", bridgeManager);
	}

	void GameStage::CreateCrossingManager()
	{
		const auto& crossingManager = AddGameObject<CrossingManager>();
		SetSharedGameObject(L"CrossingManager", crossingManager);
	}

	void GameStage::CreateGatheringManager()
	{
		const auto& gatheringManager = AddGameObject<GatheringManager>();
		SetSharedGameObject(L"GatheringManager", gatheringManager);
	}

	void GameStage::CreateGearManager()
	{
		const auto& gearManager = AddGameObject<GearManager>();
		SetSharedGameObject(L"GearManager", gearManager);
	}

	// ��Ԃ̐���
	void GameStage::CreateTrain()
	{
		// ��ԃI�u�W�F�N�g�̒ǉ�
		Vec3 startPos = GetSharedGameObject<RailManager>(L"RailManager")->GetStartRailPos();
		const auto& train = AddGameObject<GameTrain>(startPos);

		// �V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"Train", train);
	}

	// �X�v���C�g�̐���
	void GameStage::CreateSpriteObject()
	{
		m_fadeSprite->SetDiffuseColor(COL_WHITE);
		m_fadeSprite->SetDrawLayer(10);
		m_gameSprite = AddGameObject<Sprite>(L"GAMECLEAR_TX", Vec2(500.0f, 300.0f), Vec3(0.0f, 375.0f, 0.3f));
		m_gameSprite->SetDrawLayer(7);

		// ���̃G�t�F�N�g�I�u�W�F�N�g�̐���
		auto& smoke = AddGameObject<SpriteParticle>(L"SMOKE_TX");
		SetSharedGameObject(L"SmokeEffect", smoke);

		// ���^�G�t�F�N�g�I�u�W�F�N�g�̐���
		auto& star = AddGameObject<SpriteParticle>(L"STAR_TX");
		SetSharedGameObject(L"StarEffect", star);

		// �N���t�g�\�G�t�F�N�g�I�u�W�F�N�g�̐���
		auto& craft = AddGameObject<SpriteParticle>(L"RED_CIRCLE_EFEC_TX", 3);
		SetSharedGameObject(L"CraftEffect", craft);

		m_gameClearState.reset(new GameClearState(GetThis<GameStage>()));
		m_gameClearState->CreateState();

		m_gameOverState.reset(new GameOverState(GetThis<GameStage>()));
		m_gameOverState->CreateState();

		m_paymentsState.reset(new PaymentsState(GetThis<GameStage>()));
		m_paymentsState->CreateState();
	}

	// UI�̐���
	void GameStage::CreateUIObject()
	{
		// �p�����[�^
		const float scale = 60.0f;
		const Vec3 startPos = Vec3(-900.0f, 485.0f, 0.4f);
		const Vec3 distance = Vec3(0.0f, -scale * 1.75f, 0.0f);

		// �A�C�e����UI
		AddGameObject<ItemCountUI>(scale, startPos, L"UI_WOOD_TX", eItemType::Wood);
		AddGameObject<ItemCountUI>(scale, startPos + distance, L"UI_STONE_TX", eItemType::Stone);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 2.0), L"UI_GEAR_TX", eItemType::Gear);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 4.0), L"UI_RAIL_TX", eItemType::Rail);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 5.0), L"UI_BRIDGE_TX", eItemType::WoodBridge);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 6.0), L"UI_CROSSING_TX", eItemType::Crossing);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 8.0), L"UI_GOLDBAR_TX", eItemType::GoldBar);

		// �A�C�e���擾���̒���ł��G�t�F�N�g�̐ݒ�
		const auto& itemFly = GetSharedGameObject<FlyItemManager>(L"FlyItemManager");
		itemFly->SetTargetUIData(eItemType::Wood, L"UI_WOOD_TX", startPos);
		itemFly->SetTargetUIData(eItemType::Stone, L"UI_STONE_TX", startPos + distance);
		itemFly->SetTargetUIData(eItemType::Gear, L"UI_GEAR_TX", startPos + (distance * 2.0));
		itemFly->SetTargetUIData(eItemType::Rail, L"UI_RAIL_TX", startPos + (distance * 4.0));
		itemFly->SetTargetUIData(eItemType::WoodBridge, L"UI_BRIDGE_TX", startPos + (distance * 5.0));
		itemFly->SetTargetUIData(eItemType::Crossing, L"UI_CROSSING_TX", startPos + (distance * 6.0));
		itemFly->SetTargetUIData(eItemType::GoldBar, L"UI_GOLDBAR_TX", startPos + (distance * 8.0));
		itemFly->SetTargetUIData(eItemType::Money, L"UI_COIN_TX", Vec3(650.0f, 460.0f, 0.0f));

		// �N���t�g�\�ɂȂ������ɒm�点��G�t�F�N�g�̐����Ɛݒ�
		const auto& craftGuide = AddGameObject<CraftPosshibleGuide>();
		Vec3 scaleDiff = Vec3(scale * 2.5f, 0.0f, 0.0f);
		craftGuide->SetUIPosition(eCraftItem::Rail, startPos + scaleDiff + (distance * 4.0));
		craftGuide->SetUIPosition(eCraftItem::WoodBridge, startPos + scaleDiff + (distance * 5.0));
		craftGuide->SetUIPosition(eCraftItem::Crossing, startPos + scaleDiff + (distance * 6.0));

		// �|�[�Y���j���[�̍쐬
		auto& pauseMenu = AddGameObject<PauseMenu>();
		SetSharedGameObject(L"PAUSE", pauseMenu);

		// ������@�摜�̍쐬
		auto& inst = AddGameObject<Instruction>();
		SetSharedGameObject(L"INSTRUCTION", inst);

		// �S�[���K�C�h�̐���
		AddGameObject<GoalGuide>();
	}

	// ���Z�b�g����
	void GameStage::ResetCreateStage()
	{
		// CSV�ŃX�e�[�W�𐶐�
		CreateStageCSV(m_stagePath);

		// �e�탊�Z�b�g�����𑗂�
		const auto& railManager = GetSharedGameObject<RailManager>(L"RailManager");
		railManager->ResetInstanceRail();

		const auto& gameTrain = GetSharedGameObject<GameTrain>(L"Train");
		gameTrain->ResetTrain();

		const auto& bridgeManager = GetSharedGameObject<BridgeManager>(L"BridgeManager");
		bridgeManager->ResetBridge();

		const auto& gatheringManager = GetSharedGameObject<GatheringManager>(L"GatheringManager");
		gatheringManager->ResetGathering();

		const auto& crossingManager = GetSharedGameObject<CrossingManager>(L"CrossingManager");
		crossingManager->ResetCrossing();

		const auto& gearManager = GetSharedGameObject<GearManager>(L"GearManager");
		gearManager->ResetGears();

		const auto& player = GetSharedGameObject<GamePlayer>(L"Player");
		player->ResetPlayer(m_startPosition, m_goalStagingPosition);

		const auto& flyEffect = GetSharedGameObject<FlyItemManager>(L"FlyItemManager");
		flyEffect->ResetFly();

		const auto& starEffect = GetSharedGameObject<SpriteParticle>(L"StarEffect");
		starEffect->ResetParticle();

		const auto& craftEffect = GetSharedGameObject<SpriteParticle>(L"CraftEffect");
		craftEffect->ResetParticle();
	}

	// �n�ʂ̍Đ�������
	void GameStage::ResetGroundStage()
	{
		const auto& groundManager = GetSharedGameObject<GroundManager>(L"GroundManager");
		groundManager->ClearInstanceGround();
		groundManager->CreateInstanceGround();

		const auto& unbreakRock = GetSharedGameObject<UnBreakRock>(L"UnBreakRock");
		unbreakRock->CreateUnBreakRock();
	}

	void GameStage::ResetCameraObject()
	{
		Vec3 stationPos = GetSharedGameObject<Station>(L"Station")->GetPosition();
		Vec3 startEye = Vec3(m_DefStartEye.x + m_stageDistanceX, m_DefStartEye.y, m_DefStartEye.z);

		auto& camera = GetView()->GetTargetCamera();
		auto mainCamera = dynamic_pointer_cast<MainCamera>(camera);
		mainCamera->ResetCamera(stationPos, startEye, m_DefStartAt);
	}

	// �X�v���C�g�̕\��
	void GameStage::LogoActive()
	{
		m_gameSprite->SetDrawActive(false);

		switch (m_gameProgress)
		{
		case GameClear:
		case ClearSelect:
		case ToNext:
		case ToTitleClear:
		case MoneyCalculation:
		case MoneyCountDown:
			m_gameSprite->SetTexture(L"GAMECLEAR_TX");
			m_gameSprite->SetDrawActive(true);
			break;

		case GameOver:
		case ToTitleOver:
		case ContinueFadeIn:
			m_gameSprite->SetTexture(L"GAMEOVER_TX");
			m_gameSprite->SetDrawActive(true);
			break;

		default:
			break;
		}
	}

	void GameStage::ToFadeInState()
	{
		if (m_fadeSprite->FadeOutColor(2.0f))
		{
			m_gameProgress = eGameProgress::Opening;
		}

		float volume = 0.0f;
		m_bgmItem.lock()->m_SourceVoice->GetVolume(&volume);

		if (volume >= m_bgmVolume) return;
		volume = Utility::Lerp(m_bgmVolume, 0.0f, m_fadeSprite->GetDiffuseColor().w);
		m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
	}

	void GameStage::ToOpeningState()
	{
		// ��ԂƃJ�������擾
		auto& camera = GetView()->GetTargetCamera();
		auto mainCamera = dynamic_pointer_cast<MainCamera>(camera);
		auto station = GetSharedGameObject<Station>(L"Station");

		if (mainCamera->m_cameraState != MainCamera::State::Scroll)
		{
			mainCamera->ScrollStart();
		}
		if (mainCamera->GetScrollEnd())
		{
			m_gameProgress = eGameProgress::Playing;
		}
	}

	void GameStage::ToPlayingState()
	{
		if (m_fadeSprite->GetDiffuseColor().w > 0.0f)
		{
			m_fadeSprite->FadeOutColor(2.0f);
		}
	}

	void GameStage::ToMoneyCalculationState()
	{
		const auto& player = GetSharedGameObject<GamePlayer>(L"Player");
		const auto& railManager = GetSharedGameObject<RailManager>(L"RailManager");
		const auto reward = CSVLoader::LoadFile("StageClearReward");
		vector<int> moneyOperator = CSVLoader::ReadDataToInt(CSVLoader::LoadFile("MoneyOperator")).at(0);

		int addMoney = 0;
		int count = 0;

		count = int(railManager->GetAddRailNum()) * moneyOperator.at(0);
		m_paymentsState->SetNumberGoal(eGamePaymentsState::RailsInstallations, count * -1);
		addMoney += count;

		count = player->GetItemCount(eItemType::Wood) * moneyOperator.at(1);
		count += player->GetItemCount(eItemType::Stone) * moneyOperator.at(1);
		m_paymentsState->SetNumberGoal(eGamePaymentsState::ResourceSales, count);
		addMoney += count;

		count = player->GetItemCount(eItemType::Rail) * moneyOperator.at(2);
		m_paymentsState->SetNumberGoal(eGamePaymentsState::RailsSales, count);
		addMoney += count;

		count = player->GetItemCount(eItemType::GoldBar) * moneyOperator.at(3);
		m_paymentsState->SetNumberGoal(eGamePaymentsState::GoldBarSales, count);
		addMoney += count;

		for (size_t i = 0; i < reward.size(); i++)
		{
			if (reward.at(i).at(0) == m_stagePath)
			{
				count = stoi(reward.at(i).at(1));
				break;
			}
		}

		m_paymentsState->SetNumberGoal(eGamePaymentsState::RewardCount, count);
		addMoney += count;

		addMoney = max(addMoney, 0);
		m_paymentsState->SetNumberGoal(eGamePaymentsState::TotalIncome, addMoney);
		m_gameProgress = eGameProgress::MoneyCountDown;
	}

	void GameStage::ToMoneyCountDownState()
	{
		if (m_paymentsState->GetPaymentsState() != eGamePaymentsState::StandBy)
		{
			m_paymentsState->UpdateState();
			return;
		}

		m_paymentsState->ResetState();
		m_gameProgress = eGameProgress::ClearSelect;
	}

	void GameStage::ToClearSelectStage()
	{
		if (m_gameClearState->GetClearState() != eGameClearState::StandBy)
		{
			m_gameClearState->UpdateState();
			return;
		}

		auto select = m_gameClearState->GetSelectStage();
		m_gameProgress = select != eSelectGameClear::TitleBack ? eGameProgress::ToNext : eGameProgress::ToTitleClear;
	}

	void GameStage::ToNextStage()
	{
		// �t�F�[�h�p�X�v���C�g�̃G���[�`�F�b�N
		if (!m_fadeSprite) return;

		if (m_fadeSprite->FadeInColor(2.0f))
		{
			m_stagePath = App::GetApp()->GetScene<Scene>()->ToNextStage();
			ResetCreateStage();
			ResetGroundStage();
			ResetCameraObject();
			CreateStartBGM();
			m_gameClearState->ResetState();
			m_gameProgress = eGameProgress::FadeIn;
		}

		float volume = Utility::Lerp(0.5f, 0.0f, m_fadeSprite->GetDiffuseColor().w);
		m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
	}

	void GameStage::ToTitleStage()
	{
		// �t�F�[�h�p�X�v���C�g�̃G���[�`�F�b�N
		if (!m_fadeSprite) return;

		// �X�v���C�g�̃t�F�[�h�C���������I�����Ă����ꍇ�̏���
		if (m_fadeSprite->FadeInColor(2.0f))
		{
			// �^�C�g���X�e�[�W�֑J��
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"TitleStage");
		}

		float volume = Utility::Lerp(m_bgmVolume, 0.0f, m_fadeSprite->GetDiffuseColor().w);
		m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
	}

	void GameStage::PushButtonStart()
	{
		// QTE���̓|�[�Y�ł��Ȃ��悤�ɂ���
		if (GetSharedGameObject<Player>(L"Player")->GetStatus(ePlayerStatus::IsCraftQTE)) return;
		auto& menu = GetSharedGameObject<PauseMenu>(L"PAUSE"); // �|�[�Y���j���[�I�u�W�F�N�g���擾

		if (m_gameProgress == Pause) // �|�[�Y���Ȃ�
		{
			// �|�[�Y��ʂ����
			if (!menu->OnClose()) return; // �|�[�Y��ʂ����Ȃ�������SE��炳�Ȃ�
			m_soundManager->StartBGM(L"PAUSE_CLOSE_SE", 0, 1.0f, ThisPtr);
			return;
		}
		if (m_gameProgress == Playing) // �v���C���Ȃ�
		{
			// �|�[�Y��ʂ�\������
			if (!menu->OnOpen()) return; // �|�[�Y��ʂ�\���ł��Ȃ�������SE��炳�Ȃ�
			m_soundManager->StartBGM(L"PAUSE_OPEN_SE", 0, 1.0f, ThisPtr);
		}
	}

	// �R���e�B�j���[����
	void GameStage::ToGameOverStage()
	{
		// �t�F�[�h�C���J�n�̏����𖞂����Ă����ꍇ�̏���
		if (m_countTime >= m_defermentTransition)
		{
			if (m_gameOverState->GetState() != eGameOverState::StandBy)
			{
				m_gameOverState->UpdateState();
				return;
			}

			auto select = m_gameOverState->GetSelectStage();
			m_gameProgress = select != eSelectGameOver::TitleBack ? eGameProgress::ContinueFadeIn : eGameProgress::ToTitleOver;
		}
		else
		{
			m_countTime += DELTA_TIME;
		}
	}

	void GameStage::ToContinueFadeIn()
	{
		if (m_fadeSprite->FadeInColor(1.0f))
		{
			ResetCreateStage();
			m_gameOverState->ResetState();
			m_gameProgress = eGameProgress::ContinueFadeOut;
		}
	}

	void GameStage::ToContinueFadeOut()
	{
		if (m_fadeSprite->FadeOutColor(1.0f))
		{
			m_gameProgress = eGameProgress::Playing;
		}
	}

	// �������̏���
	void GameStage::OnCreate()
	{
		try
		{
			// �p�����̐������̏���
			BaseStage::OnCreate();

			// BGM�̍Đ�
			CreateStartBGM();

			// CSV�ŃX�e�[�W�𐶐�
			CreateStageCSV(m_stagePath);

			// �v���C���[�̐���
			CreatePlayer();

			// ���H�̐���
			CreateRailManager();

			// �؂̑���̐���
			CreateBridgeManager();

			// ���{�b�N�X�̐���
			CreateGroundBox();

			// ��Ԃ̐���
			CreateTrain();

			// ���؂̐���
			CreateCrossingManager();

			// �̏W�I�u�W�F�N�g�̐���
			CreateGatheringManager();

			// ���ԃI�u�W�F�N�g�̐���
			CreateGearManager();

			// �X�v���C�g�̐���
			CreateSpriteObject();

			// UI�̐���
			CreateUIObject();

			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			// �X�J�C�{�b�N�X�̐���
			auto& camera = GetView()->GetTargetCamera();
			auto mainCamera = dynamic_pointer_cast<MainCamera>(camera);
			AddGameObject<SkyBox>(mainCamera);

			// ��ԂƃJ�������擾
			auto train = GetSharedGameObject<GameTrain>(L"Train");
			// ���C���J�����̃^�[�Q�b�g�ɗ�Ԃ��Z�b�g����
			mainCamera->SetTargetObject(train);
			mainCamera->SetAt(train->GetDefaultPosition());
		}
		catch (...)
		{
			throw;
		}
	}

	// �j������鎞�̏���
	void GameStage::OnDestroy()
	{
		try
		{
			// �p�����̔j�����̏���
			BaseStage::OnDestroy();
			ReleasedResourses();
		}
		catch (...)
		{
			throw;
		}
	}

	// ���t���[���X�V����
	void GameStage::OnUpdate()
	{
		try
		{
			// �p�����̍X�V����
			BaseStage::OnUpdate();

			// �X�v���C�g�̕\��
			LogoActive();

			// START�{�^������������
			if (Input::GetStartPush())
			{
				PushButtonStart();
			}
			// ���o�����̐^�U���v���C�����ǂ����ŗ��Ă�
			m_isStaging = !Utility::ORS(m_gameProgress, eGameProgress::FadeIn, eGameProgress::Playing, eGameProgress::CraftPause);

			// �Q�[���̌��ʂɉ����ď��������s
			if (m_progressFunc.find(m_gameProgress) == m_progressFunc.end()) return;
			m_progressFunc.at(m_gameProgress)();
		}
		catch (...)
		{
			throw;
		}
	}
}