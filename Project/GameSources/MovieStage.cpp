/*!
@file MovieStage.cpp
@brief タイトルステージの実体
@author 新妻葵
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

		m_moviePath = moviePath + L"GamePlay.mp4";

		// フェード用のテクスチャ
		AddTextureResource(L"FADE_TX", texturePath + L"Fade_1.png");

		AddedTextureResources();
	}

	void LeaveMovieStage::CreateBackSprite()
	{
		const auto& back = AddGameObject<Sprite>(L"FADE_TX", WINDOW_SIZE);
		SetSharedGameObject(L"BackSprite", back);
	}

	// ステージ移行
	void LeaveMovieStage::ChangeStage()
	{
		const auto& app = App::GetApp();

		m_timer += app->GetElapsedTime();

		if (m_timer >= m_fadeTime)
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), app->GetScene<Scene>(), L"TitleStage");
		}
	}

	// Bボタンを押した時の処理
	void LeaveMovieStage::ButtonPush()
	{
		m_isPushButton = true;
	}

	void LeaveMovieStage::OnCreate()
	{
		MovieStage::OnCreate();

		CreateResourses();

		SetMovieFileName(m_moviePath);

		CreateBackSprite();

		Play();
	}

	void LeaveMovieStage::OnUpdate()
	{


		if (Input::GetPush())
		{
			ButtonPush();
			OnDestroy();
		}

		if (m_isPushButton)
		{
			ChangeStage();
		}
	}
}