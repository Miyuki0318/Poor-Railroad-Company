/*!
@file Opening.cpp
@brief オープニング画面の実装
@author 新妻葵
*/

#pragma once
#include "stdafx.h"
#include "Opening.h"
#include "TitleStage.h"

namespace basecross {
	void TitleLogo::OnCreate()
	{
		TemplateObject::OnCreate();

		// タイトルロゴ生成
		m_titleLogo = GetTypeStage<TitleStage>()->AddGameObject<Sprite>(L"TITLE_LOGO", Vec2(700.0f ,500.0f));
		m_pushLogo = GetTypeStage<TitleStage>()->AddGameObject<Sprite>(L"START_TX", Vec2(300.0f, 150.0f));

		m_titleLogo->SetPosition(Vec3(0.0f, -m_startPosY, 0.0f));
		m_pushLogo->SetPosition(Vec3(0.0f, -m_startPosY + (-m_titleLogo->GetScale().y), 0.0f));

		m_titleLogo->SetDrawLayer(2);
		m_pushLogo->SetDrawLayer(2);

		m_titlePos = m_titleLogo->GetPosition();
		m_pushPos = m_pushLogo->GetPosition();
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

		case eLogoState::exit:
			FadeTitleLogo();
			break;

		case eLogoState::idel:
			break;

		default:
			break;
		}

		if (m_logoState != eLogoState::move)
		{
			SpriteBrink();
		}

		Debug::Log(m_brinkTime);
	}
	
	void TitleLogo::MoveTitleLogo()
	{
		if (m_maxPosY <= m_titlePos.y)
		{
			m_deltaTime = 0.0f;
			m_logoState = eLogoState::push;
			GetTypeStage<TitleStage>()->StartBGM();
			return;
		}

		const auto& app = App::GetApp();
		m_deltaTime += app->GetElapsedTime();

		m_vel = m_deltaTime * m_speed;

		m_titlePos.y += m_vel;
		m_pushPos.y += m_vel;

		m_titleLogo->SetPosition(m_titlePos);
		m_pushLogo->SetPosition(m_pushPos);
	}

	void TitleLogo::FadeTitleLogo()
	{
		if (m_pushPos.y <= -m_startPosY)
		{
			m_deltaTime = 0.0f;
			m_logoState = eLogoState::idel;
			return;
		}

		const auto& app = App::GetApp();
		m_deltaTime += app->GetElapsedTime();

		m_vel = m_deltaTime * m_speed;

		m_titlePos.y += m_vel * 1.25f;
		m_pushPos.y -= m_vel;

		m_titleLogo->SetPosition(m_titlePos);
		m_pushLogo->SetPosition(m_pushPos);
	}

	void TitleLogo::PushButton()
	{
		if (Input::GetPush())
		{
			StartSE(L"PUSH_SE", 1.0f);
			m_logoState = eLogoState::exit;
			m_brinkTime = 25.0f;
		}
	}

	void TitleLogo::SpriteBrink()
	{
		const auto& app = App::GetApp();

		m_color = m_pushLogo->GetDiffuseColor();

		if (m_brinkingFlag)
		{
			m_color.w -= app->GetElapsedTime() * m_brinkTime;

			if (m_color.w <= 0.0f)
			{
				m_brinkingFlag = false;
			}
		}
		else
		{
			m_color.w += app->GetElapsedTime() * m_brinkTime;

			if (m_color.w >= 1.0f)
			{
				m_brinkingFlag = true;
			}
		}

		m_pushLogo->SetDiffuseColor(m_color);
	}
}