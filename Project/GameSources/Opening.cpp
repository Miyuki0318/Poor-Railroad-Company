/*!
@file Opening.cpp
@brief オープニング画面の実装
@author 新妻葵
*/

#pragma once
#include "stdafx.h"
#include "Opening.h"
#include "Input.h"
#include "DebugLog.h"
#include "TitleStage.h"

namespace basecross {
	void TitleLogo::OnCreate()
	{
		TemplateObject::OnCreate();

		// パラメータ設定
		const Vec2 logoScale = Vec2(700.0f, 500.0f);
		const Vec2 buttonScale = Vec2(350.0f, 100.0f);
		const Vec3 logoPos = Vec3(0.0f, -m_startPosY, 0.0f);
		const Vec3 buttonPos = Vec3(0.0f, -m_startPosY + (-m_titleLogo->GetScale().y), 0.0f);

		// タイトルロゴ生成
		m_titleLogo = GetTypeStage<TitleStage>()->AddGameObject<Sprite>(L"TITLE_LOGO", logoScale, logoPos);
		m_pushLogo = GetTypeStage<TitleStage>()->AddGameObject<Sprite>(L"START_TX", buttonScale, buttonPos);

		m_titleLogo->SetDrawLayer(2);
		m_pushLogo->SetDrawLayer(2);

		m_titlePos = m_titleLogo->GetPosition();
		m_pushPos = m_pushLogo->GetPosition();
	}

	void TitleLogo::OnUpdate()
	{
		if (m_logoState == eOpeningState::move)
		{
			MoveTitleLogo();
		}

		if (m_logoState == eOpeningState::push)
		{
			PushButton();
		}
		if (m_logoState == eOpeningState::exit)
		{
			FadeTitleLogo();
		}

		if (m_logoState != eOpeningState::move)
		{
			SpriteBrink();
		}
	}
	
	// ゲーム開始時の処理
	void TitleLogo::MoveTitleLogo()
	{
		if (m_maxPosY <= m_titlePos.y)
		{
			m_deltaTime = 0.0f;
			m_logoState = eOpeningState::push;
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

	// フェード処理
	void TitleLogo::FadeTitleLogo()
	{
		if (m_pushPos.y <= -m_startPosY)
		{
			m_deltaTime = 0.0f;
			m_logoState = eOpeningState::idel;
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

	// ボタンを押した時の処理
	void TitleLogo::PushButton()
	{
		if (Input::GetPush())
		{
			StartSE(L"PUSH_SE", 1.0f);
			m_logoState = eOpeningState::exit;
			m_brinkTime = 25.0f;
		}
	}

	// ボタンの点滅処理
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