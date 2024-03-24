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

	void GameStage::CreateResourses()
	{
		const auto& app = App::GetApp();

		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";

		app->RegisterTexture(L"GAMECLEAR_TX", texturePath + L"Win.png");
		app->RegisterTexture(L"GAMEOVER_TX", texturePath + L"Lose.png");
	}

	void GameStage::CreateSpriteObject()
	{
		m_gameClearLogo = AddGameObject<Sprite>(L"GAMECLEAR_TX", Vec2(100.0f), Vec3(0.0f));
		m_gameOverLogo = AddGameObject<Sprite>(L"GAMEOVER_TX", Vec2(100.0f), Vec3(0.0f));

		m_gameClearLogo->SetDrawActive(false);
		m_gameOverLogo->SetDrawActive(false);
	}

	void GameStage::LogoActive()
	{
		// �d�Ԃ̏�Ԃ��擾����
		// �d�Ԃ���ꂽ��GameOverLogo��\�����A
		// �d�Ԃ��w�ɂ�������GameClearLogo��\������
	}

	void GameStage::OnCreate() 
	{
		try 
		{
			CreateResourses();

			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			// ���{�b�N�X�̐���
			CreateGroundBox();

			// �v���C���[�̐���
			CreatePlayer();

			CreateSpriteObject();
		}
		catch (...) 
		{
			throw;
		}
	}
}