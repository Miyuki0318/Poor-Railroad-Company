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
	void LeaveMovieStage::CreateResourses()
	{
		const auto& app = App::GetApp();
		wstring mediaPath = app->GetDataDirWString();
		wstring moviePath = mediaPath + L"Movies/";
		wstring texturePath = mediaPath + L"Textures/";

		moviePath = moviePath + L"GamePlay.mp4";

		// �t�F�[�h�p�̃e�N�X�`��
		AddTextureResource(L"FADE_TX", texturePath + L"Fade_1.png");

		AddedTextureResources();
	}

	void LeaveMovieStage::CreateBackSprite()
	{
		const auto& back = AddGameObject<Sprite>(L"FADE_TX", WINDOW_SIZE);
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
		m_isPushButton = true;
	}

	void LeaveMovieStage::OnCreate()
	{
		MovieStage::OnCreate();

		CreateResourses();

		//CreateBackSprite();

		SetMovieFileName(m_moviePath);

		Play();
	}

	void LeaveMovieStage::OnUpdate()
	{
		if (Input::GetPush())
		{
			ButtonPush();
		}

		if (m_isPushButton)
		{
			ChangeStage();
		}
	}
}