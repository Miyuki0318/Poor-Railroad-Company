/*!
@file MovieStage.cpp
@brief タイトルステージの実体
@author 新妻葵
*/

#include "stdafx.h"
#include "MovieStage.h"
#include "Scene.h"
#include "Input.h"

namespace basecross
{
	void LeaveMovieStage::CreateResourses()
	{
		const auto& app = App::GetApp();
		wstring mediaPath = app->GetDataDirWString() + L"Movies/";

		moviePath = mediaPath + L"GamePlay.mp4";
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

		SetMovieFileName(moviePath);

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