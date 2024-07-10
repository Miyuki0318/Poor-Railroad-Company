/*!
@file MoneyCountUI.cpp
@brief 所持金のカウンタUI
@author 小澤博貴
*/

#include "stdafx.h"
#include "MoneyCountUI.h"
#include "Scene.h"
#include "BaseStage.h"

namespace basecross
{
	// 生成時の処理
	void MoneyCountUI::OnCreate()
	{
		const auto& stagePtr = GetStage(); // ステージの取得

		// スプライトの生成
		m_backSprite = stagePtr->AddGameObject<Sprite>(L"UI_MENUBACK_TX", m_backScale, m_startPos + m_backMargin + BACK_LAYER);
		m_itemSprite = stagePtr->AddGameObject<Sprite>(L"UI_COIN_TX", Vec2(m_spriteScale), Vec3(m_startPos));

		// 数字スプライトの生成
		Vec3 margin = Vec3(m_numberMargin * 4);
		for (int i = 0; i < 5; i++)
		{
			auto& number = stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPos + m_spriteMargin + margin - (m_numberMargin * float(i))));
			number->SetDiffuseColor(COL_ALPHA);
			m_numbers.sprite.push_back(number);
		}
		
		// 開始時の表示を行う
		StartSetNumbers();
	}

	// 毎フレーム更新
	void MoneyCountUI::OnUpdate()
	{
		// 数字の更新処理
		if (!m_isGoal) UpdateNumbers();

		// 色の更新処理
		UpdateColor();
	}

	void MoneyCountUI::StartSetNumbers()
	{
		const auto& scene = App::GetApp()->GetScene<Scene>();
		int money = scene->GetMoney();
		m_numbers.goal = money;
		m_numbers.degit = max(Utility::GetUpperDigit(money) - 1, 0);
		m_numbers.index = m_numbers.degit;

		while (m_numbers.degit >= 0)
		{
			// カウントダウン表示
			NumberCount::ConsecutiveNumberDraw(m_numbers, m_totalTime);
		}
	}

	// 数字の表示更新
	void MoneyCountUI::UpdateNumbers()
	{
		// カウントダウン表示
		NumberCount::ConsecutiveNumberDraw(m_numbers, m_totalTime);

		// 表示し終わったら
		if (m_numbers.degit < 0)
		{
			// SEを止める
			m_isGoal = true;
		}
	}

	// 色の確認
	void MoneyCountUI::UpdateColor()
	{
		const auto& scene = App::GetApp()->GetScene<Scene>();
		bool isLimit = scene->GetMoney() >= MONEY_LIMIT;

		for (auto& number : m_numbers.sprite)
		{
			number.lock()->SetDiffuseColor(isLimit ? COL_RED : COL_BLACK);
		}
	}
}