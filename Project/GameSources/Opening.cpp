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

		m_position = m_sprite->GetComponent<Transform>()->GetPosition();
	}

	void TitleLogo::OnUpdate()
	{

	}

	void TitleLogo::LogoMove()
	{

	}
}