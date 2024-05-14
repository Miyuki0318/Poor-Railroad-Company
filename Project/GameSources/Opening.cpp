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
		OpenStateBase(m_openState);
	}

	void TitleLogo::OpenStateBase(eOpeningState state)
	{
		switch (m_openState)
		{
		case basecross::move:
			OpenStateMove();
			break;

		case basecross::fade:
			OpenStateFade();
			break;

		case basecross::idel:
			OpenStateIdel();
			break;

		default:
			break;
		}
	}

	void TitleLogo::OpenStateMove()
	{
		if (m_maxPosY <= m_position.y)
		{
			m_openState = eOpeningState::fade;
			m_deltaTime = 0.0f;
			return;
		}

		const auto& app = App::GetApp();

		m_deltaTime += app->GetElapsedTime();

		m_position.y += m_deltaTime * m_speed;

		m_sprite->SetPosition(m_position);
	}

	void TitleLogo::OpenStateFade()
	{
		if (m_sprite->FadeOutColor(3.0f))
		{
			m_openState = eOpeningState::idel;
			return;
		}
	}

	void TitleLogo::OpenStateIdel()
	{

	}
}