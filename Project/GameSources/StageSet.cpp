/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

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
		// �������̏���
		void StageSet::OnCreate()
		{
			try{

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