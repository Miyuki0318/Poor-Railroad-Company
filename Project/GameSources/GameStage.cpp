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

	// �̌@���̐���
	void GameStage::CreateStageObject()
	{
		// Tree�̒ǉ�
		shared_ptr<Tree> treeObj;
		shared_ptr<Rock> rockObj;
		for (int i = 0; i < 10; i++) {
			treeObj = AddGameObject<Tree>(Vec3(1.0f * i, 1.5f, 1.0f), 2);
			rockObj = AddGameObject<Rock>(Vec3(1.0f * i, 1.5f, 4.0f), 2);
		}

		// �V�F�A�h�I�u�W�F�N�g�O���[�v�ɓo�^
		const auto& group = GetSharedObjectGroup(L"MiningObject");
		group->IntoGroup(treeObj);
		group->IntoGroup(rockObj);
	}

	void GameStage::OnCreate() 
	{
		try 
		{
			// �I�u�W�F�N�g�O���[�v�̍쐬
			CreateSharedObjectGroup(L"MiningObject");

			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			// ���{�b�N�X�̐���
			CreateGroundBox();

			// �v���C���[�̐���
			CreatePlayer();

			// MiningObject�̐���
			CreateStageObject();
		}
		catch (...) 
		{
			throw;
		}
	}
}