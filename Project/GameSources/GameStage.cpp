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

		// �t�F�[�h�p�̃e�N�X�`��
		AddTextureResource(L"FADE_TX", texturePath + L"Blue.png");

		// �Q�[������BGM
		AddAudioResource(L"GAME_BGM", soundPath + L"GameBGM");

		// �ǉ��������\�[�X���������ɒǉ�
		AddedTextureResources();
		AddedAudioResources();
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
		// ���{�b�N�X�I�u�W�F�N�g�̒ǉ�
		auto g = AddGameObject<GroundBox>(Vec3(49.5f, 0.0f, -7.0f) ,Vec3(100.0f, 2.0f, 15.0f));
		g->SetDrawActive(false);

		AddGameObject<GroundManager>();	// �n�ʂ̕`�搶��
		AddGameObject<UnBreakRock>();	// �󂹂Ȃ���̐���
	}

	// �v���C���[�̐���
	void GameStage::CreatePlayer()
	{
		// �v���C���[�I�u�W�F�N�g�̒ǉ�
		const auto& player = AddGameObject<GamePlayer>();

		// �V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"Player", player);
	}

	// csv�ł̃X�e�[�W����
	void GameStage::CreateStageCSV(string csvPath)
	{
		// CSV�f�[�^(int�^�̓񎟌��z��)
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
		AddGameObject<GearManager>();
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

		// �Q�[����ʂ̃T�C�Y���擾
		const float m_width = static_cast<float>(App::GetApp()->GetGameWidth());
		const float m_height = static_cast<float>(App::GetApp()->GetGameHeight());

		// �t�F�[�h�C���p�X�v���C�g�̐���
		auto& sprite = AddGameObject<Sprite>(L"FADE_TX", Vec2(m_width, m_height));
		sprite->SetDiffuseColor(COL_ALPHA);
		SetSharedGameObject(L"FadeSprite", sprite);
	}

	// UI�̐���
	void GameStage::CreateUIObject()
	{
		// �p�����[�^
		const float scale = 60.0f;
		const Vec3 startPos = Vec3(-910.0f, 500.0f, 0.0f);
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
			// �t�F�[�h�C���p�X�v���C�g���擾
			auto sprite = GetSharedGameObject<Sprite>(L"FadeSprite", true);

			// �X�v���C�g�̃t�F�[�h�C���������I�����Ă����ꍇ�̏���
			if (sprite->FadeInColor(2.0f))
			{
				// �^�C�g���X�e�[�W�֑J��
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"TitleStage");
				m_countTime = 0.0f;
			}
		}

		m_countTime += DELTA_TIME;
	}


	// �������̏���
	void GameStage::OnCreate() 
	{
		try 
		{
			// �p�����̐������̏���
			BaseStage::OnCreate();

			// ���\�[�X�̓ǂݍ���
			CreateResourses();

			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			// �v���C���[�̐���
			CreatePlayer();

			// CSV�ŃX�e�[�W�𐶐�
			CreateStageCSV(m_stagePath);

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

			// BGM�̍Đ�
			m_soundManager->StartBGM(L"GAME_BGM", XAUDIO2_LOOP_INFINITE, 0.5f, ThisPtr);

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

			// �Q�[���̏󋵂�GameClear�ȊO�̏ꍇ�͏������s��Ȃ�
			if (m_gameProgress != Playing) return;

			// �^�C�g���X�e�[�W�J�ڗp�֐�
			ToTitleStage();
		}
		catch (...)
		{
			throw;
		}
	}
}