#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ���\�[�X�̓ǂݍ���
	void StageSet::CreateResourses()
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
	void StageSet::CreateViewLight()
	{
		const Vec3 eye(-1.0f, 20.0f, -15.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	// ���{�b�N�X�̐���
	void StageSet::CreateGroundBox()
	{
		// ���{�b�N�X�I�u�W�F�N�g�̒ǉ�
		AddGameObject<GroundBox>(Vec3(100.0f, 2.0f, 15.0f));
	}

	// �v���C���[�̐���
	void StageSet::CreatePlayer()
	{
		// �v���C���[�I�u�W�F�N�g�̒ǉ�
		const auto& player = AddGameObject<Player>();

		// �V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"Player", player);
	}

	// �̌@���̐���
	void StageSet::CreateStageObject()
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
	void StageSet::CreateRail()
	{
		// ���H�I�u�W�F�N�g�̒ǉ�
		const auto& rail = AddGameObject<Rail>();


		// �V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"Rail", rail);
	}
	void StageSet::CreateRails()
	{
		// �V�F�A�I�u�W�F�N�g�O���[�v
		const auto& railGroup = GetSharedObjectGroup(L"Rails");

		for (int i = 0; i < 5; i++)
		{
			// ���H�I�u�W�F�N�g�̒ǉ�
			const auto& rail = AddGameObject<Rail>(Vec3((float)-i, 1.0f, 0.0f));
			// �V�F�A�I�u�W�F�N�g�ɓo�^
			SetSharedGameObject(L"Rails" + i, rail);
			railGroup->IntoGroup(rail);
		}

	}

	// ��Ԃ̐���
	void StageSet::CreateTrain()
	{
		// ��ԃI�u�W�F�N�g�̒ǉ�
		const auto& train = AddGameObject<Train>();

		// �V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"Train", train);
	}

	void StageSet::CreateTarminal()
	{
		// ��ԃI�u�W�F�N�g�̒ǉ�
		const auto& tarminal = AddGameObject<Tarminal>(Vec3(10.0f, 1.0f, 0.0f));

		// �V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"Tarminal", tarminal);
	}

	// �X�v���C�g�̐���
	void StageSet::CreateSpriteObject()
	{
		m_gameClearLogo = AddGameObject<Sprite>(L"GAMECLEAR_TX", Vec2(100.0f), Vec3(0.0f));
		m_gameOverLogo = AddGameObject<Sprite>(L"GAMEOVER_TX", Vec2(100.0f), Vec3(0.0f));
	}

	void StageSet::LogoActive()
	{
		switch (m_gameProgress)
		{
		case eGameMenber::Play:
			m_gameClearLogo->SetDrawActive(false);
			m_gameOverLogo->SetDrawActive(false);
			break;

		case eGameMenber::Clear:
			m_gameClearLogo->SetDrawActive(true);
			m_gameOverLogo->SetDrawActive(false);
			break;

		case eGameMenber::Over:
			m_gameClearLogo->SetDrawActive(false);
			m_gameOverLogo->SetDrawActive(true);
			break;
		}
	}
	//BGM�̍Đ�
	void StageSet::CreateBGM() {
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		 m_BGM= XAPtr->Start(L"Stage_BGM", XAUDIO2_LOOP_INFINITE, 0.0f);
	}

	void StageSet::OnDestroy()
	{
		//BGM�̃X�g�b�v
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Stop(m_BGM);
	}

	// �������̏���
	void StageSet::OnCreate()
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

			// MiningObject�̐���
			CreateStageObject();
			// ���H�̐���
			CreateRails();

			// ��Ԃ̐���
			CreateTrain();

			CreateTarminal();

			// �X�v���C�g�̐���
			CreateSpriteObject();

			//BGM�̐���
			CreateBGM();

			// �^�C�}�[�I�u�W�F�N�g�̐���
			m_timer = AddGameObject<Timer>();
		}


		catch (...)
		{
			throw;
		}
	}

	// ���t���[���X�V����
	void StageSet::OnUpdate()
	{
		try
		{
			LogoActive();
		}
		catch (...)
		{
			throw;
		}
	}

	// �`�揈��
	void StageSet::OnDraw()
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