/*!
@file MoneyCountUI.cpp
@brief 所持金のカウンタUI
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void MoneyCountUI::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage(); // ステージの取得

		// スプライトの生成
		m_backSprite = stagePtr->AddGameObject<Sprite>(L"UI_MENUBACK_TX", m_backScale, m_startPos + Vec3(m_backScale.x / 3.5f, 0.0f, 0.1f));
		m_itemSprite = stagePtr->AddGameObject<Sprite>(L"UI_COIN_TX", Vec2(m_spriteScale), Vec3(m_startPos));

		// 数字スプライトの生成
		for (int i = 0; i < 5; i++)
		{
			auto& number = stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPos + m_spriteMargin + (m_numberMargin * float(i))));
			number->SetDiffuseColor(COL_BLACK);
			m_numbers.push_back(number);
		}
	}

	// 毎フレーム更新
	void MoneyCountUI::OnUpdate()
	{
		// 数字の更新処理
		UpdateNumbers();
	}

	// 数字の表示更新
	void MoneyCountUI::UpdateNumbers()
	{
		// シーンから所持金を取得
		const auto& scene = App::GetApp()->GetScene<Scene>();
		int money = scene->GetMoney();

		// 上限になってないかの真偽
		bool isLimit = (money >= MONEY_LIMIT);

		// 数字の設定処理
		int place = 10000;
		for (auto& number : m_numbers)
		{
			number.lock()->SetNumber(money / place % 10);
			number.lock()->SetDiffuseColor(isLimit ? COL_RED : COL_BLACK);
			place = place / 10;
		}
	}
}