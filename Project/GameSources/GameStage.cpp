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

		app->RegisterTexture(L"GAMECLEAR_TX", texturePath + L"Win.png");
		app->RegisterTexture(L"GAMEOVER_TX", texturePath + L"Lose.png");

		// �N���t�g�E�B���h�E�̃e�N�X�`��
		app->RegisterTexture(L"C_WINDOW_TX", texturePath + L"CraftWindow.png");

		// QTE�o�[�̃t���[���̃e�N�X�`��
		app->RegisterTexture(L"C_QTE_FLAME_TX", texturePath + L"BarFlame.png");

		// �n�ʂ̉��e�N�X�`��
		app->RegisterTexture(L"GROUND_TX", texturePath + L"ForestGround.png");

		// �A�C�R���e�N�X�`��
		app->RegisterTexture(L"I_AXE_TX", texturePath + L"AxeIcon.png");
		app->RegisterTexture(L"I_PICK_TX", texturePath + L"PickIcon.png");
		app->RegisterTexture(L"I_RAIL_TX", texturePath + L"RailIcon.png");
		app->RegisterTexture(L"I_CRAFT_TX", texturePath + L"CraftIcon.png");
		app->RegisterTexture(L"I_WOOD_TX", texturePath + L"Wood.png");
		app->RegisterTexture(L"I_STONE_TX", texturePath + L"Stone.png");
		app->RegisterTexture(L"I_BALOON_CENTER_TX", texturePath + L"BalloonCenter.png");
		app->RegisterTexture(L"I_BALOON_RIGHT_TX", texturePath + L"BalloonRight.png");
		app->RegisterTexture(L"I_BALOON_LEFT_TX", texturePath + L"BalloonLeft.png");
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
		AddGameObject<GroundBox>(Vec3(49.5f, 0.0f, -7.0f) ,Vec3(100.0f, 2.0f, 15.0f));
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
		m_stageMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath));

		// �I�u�W�F�N�g�O���[�v
		const auto& miningGroup = GetSharedObjectGroup(L"MiningObject");

		// ��d���[�v
		for (int i = 0; i < m_stageMap.size(); i++)
		{
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
					
				case eStageID::Rock: // ��Ȃ�
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
			}
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
		const auto& train = AddGameObject<Train>();

		// �V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"Train", train);
	}

	void GameStage::CreateTarminal()
	{
		// ��ԃI�u�W�F�N�g�̒ǉ�
		const auto& tarminal = AddGameObject<Tarminal>(Vec3(10.0f, 1.0f, 0.0f));

		// �V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"Tarminal", tarminal);
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
		// �A�C�e����UI
		AddGameObject<ItemCountUI>(Vec3(-580.0f, 350.0f, 0.0f));
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

			// ���{�b�N�X�̐���
			CreateGroundBox();

			// �v���C���[�̐���
			CreatePlayer();

			// CSV�ŃX�e�[�W�𐶐�
			CreateStageCSV();

			// ���H�̐���
			CreateRailManager();

			// ��Ԃ̐���
			CreateTrain();

			// �X�v���C�g�̐���
			CreateSpriteObject();

			// UI�̐���
			CreateUIObject();

			// �^�C�}�[�I�u�W�F�N�g�̐���
			m_timer = AddGameObject<Timer>();

			// ��ԂƃJ�������擾
			auto train = GetSharedGameObject<Train>(L"Train");
			auto& camera = GetView()->GetTargetCamera();
			auto mainCamera = dynamic_pointer_cast<MainCamera>(camera);
			// ���C���J�����̃^�[�Q�b�g�ɗ�Ԃ��Z�b�g����
			mainCamera->SetTargetObject(train);
			mainCamera->SetAt(train->GetDefaultPosition());
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