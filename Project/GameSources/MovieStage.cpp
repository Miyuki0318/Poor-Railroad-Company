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
	// ステージ移行
	void LeaveMovieStage::ChangeStage()
	{
		const auto& app = App::GetApp();

		m_timer += app->GetElapsedTime();

		if (m_timer >= m_fadeTime)
		{
			m_timer = m_fadeTime;
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

		wstring dataPath;
		App::GetApp()->GetAssetsDirectory(dataPath);

		wstring moviePath = dataPath + L"";
		SetMovieFileName(moviePath);

		Play();
	}

	void LeaveMovieStage::OnUpdate()
	{
		if (Input::GetPush())
		{
			ButtonPush();
		}
	}
}