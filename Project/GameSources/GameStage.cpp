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
		// �A�v���P�[�V�����I�u�W�F�N�g�̎擾
		const auto& app = App::GetApp();

		// �f�B���N�g���p�X�̒�`
		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";

		// �Q�[�����ʃe�N�X�`��
		AddTextureResource(L"GAMECLEAR_TX", texturePath + L"Win.png");
		AddTextureResource(L"GAMEOVER_TX", texturePath + L"Lose.png");

		// �N���t�g�E�B���h�E�̃e�N�X�`��
		AddTextureResource(L"C_WINDOW_TX", texturePath + L"CraftWindow.png");

		// QTE�o�[�̃t���[���̃e�N�X�`��
		AddTextureResource(L"C_QTE_FLAME_TX", texturePath + L"BarFlame.png");

		// �n�ʂ̉��e�N�X�`��
		AddTextureResource(L"GROUND_TX", texturePath + L"ForestGround.png");

		// �A�C�R���e�N�X�`��
		AddTextureResource(L"I_AXE_TX", texturePath + L"AxeIcon.png");
		AddTextureResource(L"I_PICK_TX", texturePath + L"PickIcon.png");
		AddTextureResource(L"I_RAIL_TX", texturePath + L"RailIcon.png");
		AddTextureResource(L"I_CRAFT_TX", texturePath + L"CraftIcon.png");
		AddTextureResource(L"I_BALOON_CENTER_TX", texturePath + L"BalloonCenter.png");
		AddTextureResource(L"I_BALOON_RIGHT_TX", texturePath + L"BalloonRight.png");
		AddTextureResource(L"I_BALOON_LEFT_TX", texturePath + L"BalloonLeft.png");

		// UI�e�N�X�`��
		AddTextureResource(L"UI_WOOD_TX", texturePath + L"Wood.png");
		AddTextureResource(L"UI_STONE_TX", texturePath + L"Stone.png");
		AddTextureResource(L"UI_RAIL_TX", texturePath + L"Rail.png");

		// �ǉ��������\�[�X���������ɒǉ�
		AddedTextureResources();
	}

	// ���\�[�X�̉��
	void GameStage::ReleasedResourses()
	{
		// �����ƃe�N�X�`���̉��
		ReleasedAudioResources();
		ReleasedTextureResources();
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

		AddGameObject<GroundManager>();
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
				Vec3 position = LINE2POS(i, j);

				// ���l�̕ʖ�
				const int& num = m_stageMap.at(i).at(j);
				switch (static_cast<eStageID>(num))
				{
				case eStageID::GoalRail: // �S�[�����[���Ȃ�
					//AddGameObject<GoalRail>(position);
					break;
					
				case eStageID::Stone: // ��Ȃ�
					mining = AddGameObject<Rock>(position, 2); 
					break;

				case eStageID::Tree: // �؂Ȃ�
					mining = AddGameObject<Tree>(position, 2);
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
			const auto& treeObj = AddGameObject<Tree>(Vec3(1.0f * i, 1.5f, 2.0f), 2);
			const auto& rockObj = AddGameObject<Rock>(Vec3(1.0f * i, 1.5f, 5.0f), 2);

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
		const auto& train = AddGameObject<GameTrain>();

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
			// �I�u�W�F�N�g�O���[�v�̍쐬
			CreateSharedObjectGroup(L"MiningObject"); // �̌@�\�I�u�W�F�N�g

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

			// �^�C�}�[�I�u�W�F�N�g�̐���
			m_timer = AddGameObject<Timer>();

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
			// �X�v���C�g�̕\��
			LogoActive();

		}
		catch (...)
		{
			throw;
		}
	}

	// �`�揈��
	void GameStage::OnDraw()
	{
		// �A�v���P�[�V�����I�u�W�F�N�g�̎擾
		const auto& app = App::GetApp();

		// �f�o�b�O������������I�ɋ�ɂ���
		app->GetScene<Scene>()->SetDebugString(L"");
		
		// FPS�̕`��
		const auto& fps = app->GetStepTimer().GetFramesPerSecond();
		Debug::Log(L"FPS : ", fps);

		// �p�����̕`�掞�̊֐������s����
		Stage::OnDraw();

		// �f�o�b�N�p������̕\����\���؂�ւ�
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawLayer(10);
		debugStr->SetDrawActive(false);
	}
}