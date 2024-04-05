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
	}

	//�r���[�ƃ��C�g�̐���
	void GameStage::CreateViewLight()
	{
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<MainCamera>();
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
		AddGameObject<GroundBox>(Vec3(100.0f, 2.0f, 15.0f));
	}

	// �v���C���[�̐���
	void GameStage::CreatePlayer()
	{
		// �v���C���[�I�u�W�F�N�g�̒ǉ�
		const auto& player = AddGameObject<Player>();

		// �V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"Player", player);
	}

	// csv�ł̃X�e�[�W����
	void GameStage::CreateStageCSV(string csvPath)
	{
		// ���Ɖ��̒[
		const float leftX = -7.0f;
		const float upperZ = 7.0f;

		// CSV�f�[�^(int�^�̓񎟌��z��)
		m_stageMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath));

		// �I�u�W�F�N�g�O���[�v
		const auto& railGroup = GetSharedObjectGroup(L"Rails");
		const auto& miningGroup = GetSharedObjectGroup(L"MiningObject");

		// ��d���[�v
		for (int i = 0; i < m_stageMap.size(); i++)
		{
			for (int j = 0; j < m_stageMap.at(i).size(); j++)
			{
				// ���I�u�W�F�N�g
				shared_ptr<Rail> rail = nullptr;
				shared_ptr<MiningObject> mining = nullptr;

				// ���W(����+��ԍ��~�X�P�[��,����+�s��-�s�ԍ��~�X�P�[��)
				Vec3 position = Vec3(leftX + j, 1.5f, upperZ + -i);

				// ���l�̕ʖ�
				const int& num = m_stageMap.at(i).at(j);
				switch (static_cast<eStageID>(num))
				{
				case eStageID::Rail: // ���[���Ȃ�
					//rail = AddGameObject<Rail>(position);
					break;

				case eStageID::GoalRail: // �S�[�����[���Ȃ�
					AddGameObject<GoalRail>(position);
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
				if (rail)
				{
					railGroup->IntoGroup(rail);
				}
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

	// ���H�̐���
	void GameStage::CreateRail()
	{
		// ���H�I�u�W�F�N�g�̒ǉ�
		const auto& rail = AddGameObject<Rail>();

		// �V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"Rail", rail);
	}
	void GameStage::CreateRails()
	{
		const auto& railManager = AddGameObject<RailManager>();
		SetSharedGameObject(L"RailManager", railManager);
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
			CreateSharedObjectGroup(L"Rails"); // ���[���I�u�W�F�N�g

			// ���\�[�X�̓ǂݍ���
			CreateResourses();

			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			// ���{�b�N�X�̐���
			CreateGroundBox();

			// �v���C���[�̐���
			CreatePlayer();

			//// MiningObject�̐���
			//CreateStageObject();

			//// �S�[���n�_�̐���
			//CreateGoalRail();

			// CSV�ŃX�e�[�W�𐶐�
			CreateStageCSV();

			// ���H�̐���
			CreateRails();

			// ��Ԃ̐���
			CreateTrain();

			CreateTarminal();

			// �X�v���C�g�̐���
			CreateSpriteObject();

			// �^�C�}�[�I�u�W�F�N�g�̐���
			m_timer = AddGameObject<Timer>();

			// �v���C���[�ƃJ�������擾
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
		debugStr->SetDrawActive(true);
	}
}