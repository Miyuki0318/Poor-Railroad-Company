/*!
@file Opening.cpp
@brief オープニング画面の実装
@author 新妻葵
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void TitleLogo::OnCreate()
	{
		TemplateObject::OnCreate();

		// タイトルロゴ生成
		m_sprite = GetStage()->AddGameObject<Sprite>(L"TITLE_LOGO", Vec2(500.0f));

		m_sprite->SetPosition(Vec3(0.0f, -m_startPosY, 0.0f));

		m_position = m_sprite->GetPosition();
	}

	void TitleLogo::OnUpdate()
	{
		if (!m_fade)
		{
			MoveTitleLogo();
		}
		else
		{
			FadeTitleLogo();
		}
	}
	
	void TitleLogo::MoveTitleLogo()
	{
		if (m_maxPosY <= m_position.y)
		{
			m_deltaTime = 0.0f;
			PushButton();
			return;
		}

		const auto& app = App::GetApp();

		m_deltaTime += app->GetElapsedTime();

		m_position.y += m_deltaTime * m_speed;

		m_sprite->SetPosition(m_position);
	}

	void TitleLogo::FadeTitleLogo()
	{
		m_sprite->FadeOutColor(m_fadeTime);
	}

	void TitleLogo::PushButton()
	{
		if (Input::GetPush())
		{
			m_fade = true;
		}
	}
}