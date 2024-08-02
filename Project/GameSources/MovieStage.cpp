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
	// リソースの読込
	void LeaveMovieStage::CreateResourses()
	{
		const auto& app = App::GetApp();
		wstring mediaPath = app->GetDataDirWString();
		wstring texturePath = mediaPath + L"Textures/";

		m_moviePath = mediaPath + L"Movies/" + L"GamePlay.mp4";

		// フェード用のテクスチャ
		AddTextureResource(L"FADE_TX", texturePath + L"Fade_1.png");

		AddedTextureResources();
	}

	// 背景スプライトの生成
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
		m_pushButton = true;
	}

	// 生成時に一度だけ呼び出される関数
	void LeaveMovieStage::OnCreate()
	{
		MovieStage::OnCreate();

		CreateResourses();

		// 再生する動画指定
		SetMovieFileName(m_moviePath);

		CreateBackSprite();

		// 再生
		Play();
	}

	// 毎フレーム呼び出される関数
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