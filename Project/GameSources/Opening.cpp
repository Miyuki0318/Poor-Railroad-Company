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
		switch (m_logoState)
		{
		case eLogoState::move:
			MoveTitleLogo();
			break;

		case eLogoState::push:
			PushButton();
			break;

		case eLogoState::fade:
			FadeTitleLogo();
			break;

		case eLogoState::idel:
			break;

		default:
			break;
		}
	}
	
	void TitleLogo::MoveTitleLogo()
	{
		if (m_maxPosY <= m_position.y)
		{
			m_deltaTime = 0.0f;
			m_logoState = eLogoState::push;
			GetTypeStage<TitleStage>()->StartBGM();
			return;
		}

		const auto& app = App::GetApp();

		m_deltaTime += app->GetElapsedTime();

		m_position.y += m_deltaTime * m_speed;

		m_sprite->SetPosition(m_position);
	}

	void TitleLogo::FadeTitleLogo()
	{
		if (m_sprite->FadeOutColor(m_fadeTime))
		{
			m_logoState = eLogoState::idel;
		}
	}

	void TitleLogo::PushButton()
	{
		if (Input::GetPush())
		{
			m_logoState = eLogoState::fade;
		}
	}
}