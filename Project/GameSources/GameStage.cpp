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

		// �n�ʂ̉��e�N�X�`��
		AddTextureResource(L"GROUND_TX", texturePath + L"ForestGround.png");

		// �Q�[������BGM
		AddAudioResource(L"FIRST_BGM", soundPath + L"FirstBGM");
		AddAudioResource(L"SECOND_BGM", soundPath + L"SecondBGM");
		AddAudioResource(L"THIRD_BGM", soundPath + L"ThirdBGM");

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
		m_soundManager->StartBGM(Utility::ToUpperString(bgmKey) + L"_BGM", XAUDIO2_LOOP_INFINITE, 0.5f, ThisPtr);
	}

	//�r���[�ƃ��C�g�̐���
	void GameStage::CreateViewLight()
	{
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<MainCamera>(MainCamera::State::Follow);
		PtrView->SetCamera(PtrCamera);
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	// ���{�b�N�X�̐���
	void GameStage::CreateGroundBox()
	{
		AddGameObject<GroundManager>();	// �n�ʂ̕`�搶��
		AddGameObject<UnBreakRock>();	// �󂹂Ȃ���̐���
	}

	// �v���C���[�̐���
	void GameStage::CreatePlayer()
	{
		// �J�n���̍��W���擾
		Vec3 startPos = ROWCOL2POS(m_startLine.x, m_startLine.y);

		// �v���C���[�I�u�W�F�N�g�̒ǉ�
		const auto& player = AddGameObject<GamePlayer>();
		player->SetPosition(startPos.x, 2.0f, startPos.z);

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
					m_startLine = Point2D<size_t>(i, j);
					id = 0;
				}
				if (num == eStageID::PlayerGoal) 
				{
					m_goalLine = Point2D<size_t>(i, j);
					id = 0;
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
		const auto& train = AddGameObject<GameTrain>(Vec3(0.0f, 1.0f, -7.0f));

		// �V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"Train", train);
	}

	// �X�v���C�g�̐���
	void GameStage::CreateSpriteObject()
	{
		m_gameSprite = AddGameObject<Sprite>(L"GAMECLEAR_TX", Vec2(500.0f), Vec3(0.0f));
	}

	// UI�̐���
	void GameStage::CreateUIObject()
	{
		// �p�����[�^
		const float scale = 60.0f;
		const Vec3 startPos = Vec3(-910.0f, 500.0f, 0.1f);
		const Vec3 distance = Vec3(0.0f, -scale * 1.75f, 0.0f);

		// �A�C�e����UI
		AddGameObject<ItemCountUI>(scale, startPos, L"UI_WOOD_TX", eItemType::Wood);
		AddGameObject<ItemCountUI>(scale, startPos + distance, L"UI_STONE_TX", eItemType::Stone);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 2.0), L"UI_GEAR_TX", eItemType::Gear);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 4.0), L"UI_RAIL_TX", eItemType::Rail);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 5.0), L"UI_BRIDGE_TX", eItemType::WoodBridge);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 6.0), L"UI_CROSSING_TX", eItemType::Crossing);
	}

	// �X�v���C�g�̕\��
	void GameStage::LogoActive()
	{
		switch (m_gameProgress)
		{
		case Playing :
		case ContinueFade:
			m_gameSprite->SetDrawActive(false);
			break;

		case GameClear:
			m_gameSprite->SetTexture(L"GAMECLEAR_TX");
			m_gameSprite->SetDrawActive(true);
			break;

		case GameOver:
			m_gameSprite->SetTexture(L"GAMEOVER_TX");
			m_gameSprite->SetDrawActive(true);
			break;
		}
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
		}

		m_countTime += DELTA_TIME;
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

		m_countTime += DELTA_TIME;
	}

	// �R���e�B�j���[���̃t�F�[�h�C������
	void GameStage::ContinueFadeInState()
	{
		if (m_fadeSprite->FadeInColor(2.0f))
		{
			m_continueState = eContinueState::Reset;
		}
	}

	// �R���e�B�j���[���̃��Z�b�g����
	void GameStage::ContinueResetState()
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
		player->ResetPlayer();

		m_continueState = eContinueState::FadeOut;
		m_gameProgress = eGameProgress::ContinueFade;
	}

	// �R���e�B�j���[���̃t�F�[�h�A�E�g
	void GameStage::ContinueFadeOutState()
	{
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

			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

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