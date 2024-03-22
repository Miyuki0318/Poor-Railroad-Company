/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	//�r���[�ƃ��C�g�̐���
	void GameStage::CreateViewLight()
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
		for (int i = 0; i < 5; i++)
		{
			// ���H�I�u�W�F�N�g�̒ǉ�
			const auto& rail = AddGameObject<Rail>(Vec3((float)-i, 1.0f, 0.0f));
			// �V�F�A�I�u�W�F�N�g�ɓo�^
			SetSharedGameObject(L"Rails" + i, rail);
		}

	}

	// ��Ԃ̐���
	void GameStage::CreateTrain()
	{
		// ��ԃI�u�W�F�N�g�̒ǉ�
		const auto& train = AddGameObject<Train>();

		// �V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"Train", train);
	}

	// �������̏���
	void GameStage::OnCreate() 
	{
		try 
		{
			// �̌@�ł���I�u�W�F�N�g�̃O���[�v�𐶐�
			CreateSharedObjectGroup(L"Mining");

			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			// ���{�b�N�X�̐���
			CreateGroundBox();

			// �v���C���[�̐���
			CreatePlayer();

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

	// �`�揈��
	void GameStage::OnDraw()
	{
		// �f�o�b�O������������I�ɋ�ɂ���
		App::GetApp()->GetScene<Scene>()->SetDebugString(L"");

		// �p�����̕`�掞�̊֐������s����
		Stage::OnDraw();

		// �f�o�b�N�p������̕\����\���؂�ւ�
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawLayer(10);
		debugStr->SetDrawActive(true);
	}
}