/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

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
		AddTextureResource(L"NEXTSTAGE_TX", texturePath + L"White.png");
		AddTextureResource(L"CLEAR_TITLEBACK_TX", texturePath + L"White.png");
		AddTextureResource(L"RAIL_LINE_TX", texturePath + L"RailLine.tga");

		// �n�ʂ̉��e�N�X�`��
		AddTextureResource(L"GROUND_TX", texturePath + L"ForestGround.png");

		// �Q�[������BGM
		AddAudioResource(L"FIRST_BGM", soundPath + L"FirstBGM");
		AddAudioResource(L"SECOND_BGM", soundPath + L"SecondBGM");
		AddAudioResource(L"THIRD_BGM", soundPath + L"ThirdBGM");
		AddAudioResource(L"FOURTH_BGM", soundPath + L"FourthBGM");
		AddAudioResource(L"FIFTH_BGM", soundPath + L"FifthBGM");

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
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		Vec3 defEye = Vec3(3.0f + m_stageDistanceX, 20.0f, -23.5f);
		Vec3 defAt = Vec3(3.0f, 1.0f, -8.5f);

		auto PtrCamera = ObjectFactory::Create<MainCamera>(MainCamera::State::Follow, defEye, defAt);
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
				if (num == eStageID::DeRail)
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

		AddGameObject<RailGuide>();
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
		m_fadeSprite->SetPosition(m_fadeSprite->GetPosition() + Vec3(0.0f, 0.0f, 0.1f));
		m_fadeSprite->SetDiffuseColor(COL_WHITE);
		m_gameSprite = AddGameObject<Sprite>(L"GAMECLEAR_TX", Vec2(500.0f), Vec3(0.0f, 200.0f, 0.0f));

		// �R���e�B�j���[���Ɉ����X�v���C�g
		m_continueSprite = AddGameObject<Sprite>(L"CONTINUE_TX", m_defScale, m_leftPos);
		m_titleBackSprite = AddGameObject<Sprite>(L"CONTINUE_TITLEBACK_TX", m_defScale, m_rightPos);
		m_selectMap.emplace(eContinueSelect::Continue, m_continueSprite);
		m_selectMap.emplace(eContinueSelect::TitleBack, m_titleBackSprite);
		m_continueSprite->SetDiffuseColor(COL_ALPHA);
		m_titleBackSprite->SetDiffuseColor(COL_ALPHA);

		m_clearState.reset(new GameClearState(GetThis<GameStage>()));
		m_clearState->CreateState();
	}

	// UI�̐���
	void GameStage::CreateUIObject()
	{
		// �p�����[�^
		const float scale = 60.0f;
		const Vec3 startPos = Vec3(-900.0f, 500.0f, 0.2f);
		const Vec3 distance = Vec3(0.0f, -scale * 1.75f, 0.0f);

		// �A�C�e����UI
		AddGameObject<ItemCountUI>(scale, startPos, L"UI_WOOD_TX", eItemType::Wood);
		AddGameObject<ItemCountUI>(scale, startPos + distance, L"UI_STONE_TX", eItemType::Stone);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 2.0), L"UI_GEAR_TX", eItemType::Gear);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 4.0), L"UI_RAIL_TX", eItemType::Rail);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 5.0), L"UI_BRIDGE_TX", eItemType::WoodBridge);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 6.0), L"UI_CROSSING_TX", eItemType::Crossing);

		// �A�C�e���擾���̒���ł��G�t�F�N�g�̐ݒ�
		const auto& itemFly = GetSharedGameObject<FlyItemManager>(L"FlyItemManager");
		itemFly->SetTargetUIData(eItemType::Wood, L"UI_WOOD_TX", startPos);
		itemFly->SetTargetUIData(eItemType::Stone, L"UI_STONE_TX", startPos + distance);
		itemFly->SetTargetUIData(eItemType::Gear, L"UI_GEAR_TX", startPos + (distance * 2.0));
		itemFly->SetTargetUIData(eItemType::Rail, L"UI_RAIL_TX", startPos + (distance * 4.0));
		itemFly->SetTargetUIData(eItemType::WoodBridge, L"UI_BRIDGE_TX", startPos + (distance * 5.0));
		itemFly->SetTargetUIData(eItemType::Crossing, L"UI_CROSSING_TX", startPos + (distance * 6.0));

		// �|�[�Y���j���[�̍쐬
		auto& pauseMenu = AddGameObject<PauseMenu>();
		SetSharedGameObject(L"PAUSE", pauseMenu);
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
		auto train = GetSharedGameObject<GameTrain>(L"Train");

		Vec3 defEye = Vec3(3.0f + m_stageDistanceX, 20.0f, -23.5f);
		Vec3 defAt = Vec3(3.0f, 1.0f, -8.5f);

		auto newCamera = ObjectFactory::Create<MainCamera>(MainCamera::State::Follow, defEye, defAt);
		newCamera->SetTargetObject(train);
		newCamera->SetAt(train->GetDefaultPosition());

		auto& view = dynamic_pointer_cast<SingleView>(GetView());
		view->SetCamera(newCamera);
	}

	// �X�v���C�g�̕\��
	void GameStage::LogoActive()
	{
		m_gameSprite->SetDrawActive(false);

		switch (m_gameProgress)
		{
		case GameClear:
			m_gameSprite->SetTexture(L"GAMECLEAR_TX");
			break;

		case GameOver:
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
			m_gameProgress = eGameProgress::Playing;
		}

		float volume = Utility::Lerp(0.5f, 0.0f, m_fadeSprite->GetDiffuseColor().w);
		m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
	}

	void GameStage::ToClearSelectStage()
	{
		if (m_clearState->GetClearState() != eGameClearState::StandBy)
		{
			m_clearState->UpdateState();
			return;
		}

		auto select = m_clearState->GetSelectStage();
		m_gameProgress = select != eSelectStage::TitleBack ? eGameProgress::ToNext : eGameProgress::ToTitle;
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
			m_clearState->ResetState();
			m_gameProgress = eGameProgress::FadeIn;
		}

		float volume = Utility::Lerp(0.5f, 0.0f, m_fadeSprite->GetDiffuseColor().w);
		m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
	}

	void GameStage::ToTitleStage()
	{
		// �t�F�[�h�C���J�n�̏����𖞂����Ă����ꍇ�̏���
		if (m_countTime >= m_defermentTransition) {
			// �t�F�[�h�p�X�v���C�g�̃G���[�`�F�b�N
			if (!m_fadeSprite) return;

			// �X�v���C�g�̃t�F�[�h�C���������I�����Ă����ꍇ�̏���
			if (m_fadeSprite->FadeInColor(2.0f))
			{
				// �^�C�g���X�e�[�W�֑J��
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"TitleStage");
				m_countTime = 0.0f;
			}

			float volume = Utility::Lerp(0.5f, 0.0f, m_fadeSprite->GetDiffuseColor().w);
			m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
		}

		m_countTime += DELTA_TIME;
	}

	void GameStage::OnPauseMenu()
	{
		auto& menu = GetSharedGameObject<PauseMenu>(L"PAUSE");
		
	}

	// �R���e�B�j���[����
	void GameStage::ToContinueStage()
	{
		// �t�F�[�h�C���J�n�̏����𖞂����Ă����ꍇ�̏���
		if (m_countTime >= m_defermentTransition) 
		{
			// �t�F�[�h�p�X�v���C�g�̃G���[�`�F�b�N
			if (!m_fadeSprite) return;

			// �R���e�B�j���[�����𑗂�
			if (m_continueFunc.find(m_continueState) == m_continueFunc.end()) return;
			m_continueFunc.at(m_continueState)();
		}
		else
		{
			m_countTime += DELTA_TIME;
		}
	}

	// �R���e�B�j���[���̃t�F�[�h�C������
	void GameStage::ContinueFadeInState()
	{
		if (m_fadeSprite->FadeInColor(2.0f))
		{
			m_continueState = eContinueState::SelectFade;
		}
	}

	void GameStage::ContinueSelectFadeState()
	{
		m_titleBackSprite->FadeInColor(0.5f);
		if (m_continueSprite->FadeInColor(0.5f))
		{
			m_continueState = eContinueState::Selecting;
		}
	}

	void GameStage::ContinueSelectingState()
	{
		float stickVal = Input::GetLStickValue().x;
		
		m_totalTime += DELTA_TIME * 2.0f;
		if (m_totalTime >= XM_2PI) m_totalTime = 0.0f;

		if (stickVal && !m_pastStickVal)
		{
			m_totalTime = 0.0f;
			m_pastState = m_currentState;
			switch (m_currentState)
			{
			case eContinueSelect::Continue:
				m_currentState = eContinueSelect::TitleBack;
				break;

			case eContinueSelect::TitleBack:
				m_currentState = eContinueSelect::Continue;
				break;

			default:
				break;
			}
		}

		m_pastStickVal = stickVal;
		float scale = Utility::SinCurve(m_totalTime, 1.0f, 1.25f);
		m_selectMap.at(m_pastState).lock()->SetScale(m_defScale);
		m_selectMap.at(m_currentState).lock()->SetScale(m_defScale * scale);

		if (Input::GetPushB()) m_continueState = eContinueState::SelectState;
	}

	// �R���e�B�j���[���̑I�������Ăяo��
	void GameStage::ContinueSelectState()
	{
		if (m_selectFunc.find(m_currentState) == m_selectFunc.end()) return;

		m_selectFunc.at(m_currentState)();
	}

	// �R���e�B�j���[���̃��Z�b�g����
	void GameStage::ResetState()
	{
		ResetCreateStage();

		m_continueState = eContinueState::FadeOut;
		m_gameProgress = eGameProgress::ContinueFade;
	}

	// �^�C�g���ɖ߂�
	void GameStage::TitleBackState()
	{
		// �X�v���C�g���t�F�[�h�A�E�g
		m_titleBackSprite->FadeOutColor(1.0f);
		m_continueSprite->FadeOutColor(1.0f);
		if (m_gameSprite->FadeOutColor(2.5f))
		{
			// �^�C�g���X�e�[�W�ɑJ��
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"TitleStage");
		}

		float volume = Utility::Lerp(0.0f, 0.5f, m_gameSprite->GetDiffuseColor().w);
		m_bgmItem.lock()->m_SourceVoice->SetVolume(volume);
	}

	// �R���e�B�j���[���̃t�F�[�h�A�E�g
	void GameStage::ContinueFadeOutState()
	{
		m_titleBackSprite->FadeOutColor(0.5f);
		m_continueSprite->FadeOutColor(0.5f);

		if (m_fadeSprite->FadeOutColor(2.0f))
		{
			m_gameProgress = eGameProgress::Playing;
			m_continueState = eContinueState::FadeIn;
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