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
		m_groundMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "2Ground"));

		// �I�u�W�F�N�g�O���[�v
		const auto& miningGroup = GetSharedObjectGroup(L"MiningObject");

		// ��d���[�v
		for (int i = 0; i < m_stageMap.size(); i++)
		{
			vector<Vec3> tempVec;
			for (int j = 0; j < m_stageMap.at(i).size(); j++)
			{
				// ���I�u�W�F�N�g
				shared_ptr<MiningObject> mining = nullptr;

				// ���W(����+��ԍ��~�X�P�[��,����+�s��-�s�ԍ��~�X�P�[��)
				Vec3 position = ROWCOL2POS(i, j);

				// ���l�̕ʖ�
				eStageID num = STAGE_ID(m_stageMap.at(i).at(j));
				switch (num)
				{
				case eStageID::Stone: // ��Ȃ�
					mining = AddGameObject<Rock>(position); 
					break;

				case eStageID::Tree: // �؂Ȃ�
					mining = AddGameObject<Tree>(position);
					break;

				default:
					break;
				}

				// �I�u�W�F�N�g�O���[�v�ւ̒ǉ�
				if (mining)
				{
					miningGroup->IntoGroup(mining);
				}

				tempVec.push_back(position);
			}
			m_positionMap.push_back(tempVec);
		}
	}

	// �̌@���̐���
	void GameStage::CreateStageObject()
	{
		// �V�F�A�h�I�u�W�F�N�g�O���[�v���擾
		const auto& group = GetSharedObjectGroup(L"MiningObject");

		for (int i = 0; i < 10; i++) {
			const auto& treeObj = AddGameObject<Tree>(Vec3(1.0f * i, 1.5f, 2.0f));
			const auto& rockObj = AddGameObject<Rock>(Vec3(1.0f * i, 1.5f, 5.0f));

			group->IntoGroup(treeObj);
			group->IntoGroup(rockObj);
		}
	}

	void GameStage::CreateRailManager()
	{
		const auto& railManager = AddGameObject<RailManager>();
		SetSharedGameObject(L"RailManager", railManager);

		AddGameObject<RailGuide>();
	}

	void GameStage::CreateGoalRail()
	{
		AddGameObject<GoalRail>(Vec3(9.0f, 1.0f, 0.0f));
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
		m_gameClearLogo = AddGameObject<Sprite>(L"GAMECLEAR_TX", Vec2(100.0f), Vec3(0.0f));
		m_gameOverLogo = AddGameObject<Sprite>(L"GAMEOVER_TX", Vec2(100.0f), Vec3(0.0f));
	}

	// UI�̐���
	void GameStage::CreateUIObject()
	{
		// �p�����[�^
		const float scale = 40.0f;
		const Vec3 startPos = Vec3(-910.0f, 500.0f, 0.0f);
		const Vec3 distance = Vec3(0.0f, -scale * 2.0f, 0.0f);

		// �A�C�e����UI
		AddGameObject<ItemCountUI>(scale, startPos, L"UI_WOOD_TX", eItemType::Wood);
		AddGameObject<ItemCountUI>(scale, startPos + distance, L"UI_STONE_TX", eItemType::Stone);
		AddGameObject<ItemCountUI>(scale, startPos + (distance * 2.0), L"UI_RAIL_TX", eItemType::Rail);
	}

	// �X�v���C�g�̕\��
	void GameStage::LogoActive()
	{
		switch (m_gameProgress)
		{
		case eGameProgress::Playing :
			m_gameClearLogo->SetDrawActive(false);
			m_gameOverLogo->SetDrawActive(false);
			break;

		case eGameProgress::GameClear:
			m_gameClearLogo->SetDrawActive(true);
			m_gameOverLogo->SetDrawActive(false);
			break;

		case eGameProgress::GameOver:
			m_gameClearLogo->SetDrawActive(false);
			m_gameOverLogo->SetDrawActive(true);
			break;
		}
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
			CreateStageCSV();

			// ���H�̐���
			CreateRailManager();

			// ���{�b�N�X�̐���
			CreateGroundBox();

			// ��Ԃ̐���
			CreateTrain();

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

			const auto& railManager = GetSharedGameObject<RailManager>(L"RailManager");
			Debug::Log(L"���[���̐ݒu�� : ", railManager->GetAddRailNum());
		}
		catch (...)
		{
			throw;
		}
	}
}