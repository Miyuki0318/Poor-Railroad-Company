/*!
@file MovieStage.cpp
@brief �^�C�g���X�e�[�W�̎���
@author �V�Ȉ�
*/

#include "stdafx.h"
#include "MovieStage.h"
#include "Scene.h"
#include "Input.h"

namespace basecross
{
	// �X�e�[�W�ڍs
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

	// B�{�^�������������̏���
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