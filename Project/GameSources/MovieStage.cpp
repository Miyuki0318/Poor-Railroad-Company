/*!
@file MovieStage.cpp
@brief �^�C�g���X�e�[�W�̎���
@author �V�Ȉ�
*/

#include "stdafx.h"
#include "MovieStage.h"
#include "Scene.h"
#include "Input.h"
#include "Sprite.h"

namespace basecross
{
	// ���\�[�X�̓Ǎ�
	void LeaveMovieStage::CreateResourses()
	{
		const auto& app = App::GetApp();
		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";

		m_moviePath = mediaPath + L"Movies/" + L"GamePlay.mp4";

		// �t�F�[�h�p�̃e�N�X�`��
		AddTextureResource(L"FADE_TX", texturePath + L"Fade_1.png");

		AddedTextureResources();
	}

	// �w�i�X�v���C�g�̐���
	void LeaveMovieStage::CreateBackSprite()
	{
		const auto& back = AddGameObject<Sprite>(L"FADE_TX", WINDOW_SIZE);
		SetSharedGameObject(L"BackSprite", back);
	}

	// �X�e�[�W�ڍs
	void LeaveMovieStage::ChangeStage()
	{
		const auto& app = App::GetApp();

		m_timer += app->GetElapsedTime();

		if (m_timer >= m_fadeTime)
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), app->GetScene<Scene>(), L"TitleStage");
		}
	}

	// B�{�^�������������̏���
	void LeaveMovieStage::ButtonPush()
	{
		m_pushButton = true;
	}

	// �������Ɉ�x�����Ăяo�����֐�
	void LeaveMovieStage::OnCreate()
	{
		MovieStage::OnCreate();

		CreateResourses();

		// �Đ����铮��w��
		SetMovieFileName(m_moviePath);

		CreateBackSprite();

		// �Đ�
		Play();
	}

	// ���t���[���Ăяo�����֐�
	void LeaveMovieStage::OnUpdate()
	{
		if (Input::GetPush())
		{
			ButtonPush();
			OnDestroy();
		}

		if (m_pushButton)
		{
			ChangeStage();
		}
	}
}