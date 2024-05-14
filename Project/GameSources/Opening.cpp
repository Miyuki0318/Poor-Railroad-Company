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

		m_sprite = GetStage()->AddGameObject<Sprite>(L"TITLE_LOGO", Vec2(500.0f));

		m_sprite->SetPosition(Vec3(0.0f, -m_height, 0.0f));

		m_position = m_sprite->GetPosition();
	}

	void TitleLogo::OnUpdate()
	{
		LogoMove();
	}

	void TitleLogo::LogoMove()
	{
		const auto& app = App::GetApp();

		m_deltaTime += app->GetElapsedTime();

		if (m_maxPosY >= m_position.y)
		{

			m_position.y += m_deltaTime * m_speed;

			m_sprite->SetPosition(m_position);
		}

		Debug::Log(m_position);
	}
}