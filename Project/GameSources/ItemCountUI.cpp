/*!
@file ItemCountUI.cpp
@brief アイテムのカウンタUI
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void ItemCountUI::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage(); // ステージの取得

		// スプライトの生成
		m_backSprite = stagePtr->AddGameObject<Sprite>(L"UI_MENUBACK_TX", m_backScale, m_startPos + m_spriteMargin + BACK_LAYER);
		m_itemSprite = stagePtr->AddGameObject<Sprite>(m_texKey, Vec2(m_spriteScale), Vec3(m_startPos));
		m_slashSprite = stagePtr->AddGameObject<Sprite>(L"SLASH_TX", Vec2(m_limitScale), Vec3(m_startPos + m_limitPos + m_spriteMargin + m_numberMargin + m_numberMargin));
		
		// 数字用スプライトの生成
		m_itemNums = make_pair(
			stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPos + m_spriteMargin + m_numberMargin)),
			stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPos + m_spriteMargin))
		);
		m_limitNums = make_pair(
			stagePtr->AddGameObject<Number>(Vec2(m_limitScale), Vec3(m_startPos + m_limitPos + m_spriteMargin + m_numberMargin + m_limitMargin + m_limitMargin + m_limitMargin)),
			stagePtr->AddGameObject<Number>(Vec2(m_limitScale), Vec3(m_startPos + m_limitPos + m_spriteMargin + m_numberMargin + m_numberMargin + m_limitMargin))
		);

		// ディヒューズ色で色を設定
		m_itemNums.first.lock()->SetDiffuseColor(COL_BLACK);
		m_itemNums.second.lock()->SetDiffuseColor(COL_BLACK);
		m_limitNums.first.lock()->SetDiffuseColor(COL_RED);
		m_limitNums.second.lock()->SetDiffuseColor(COL_RED);

		// レイヤー設定
		m_backSprite.lock()->SetDrawLayer(4);
		m_itemSprite.lock()->SetDrawLayer(4);
		m_slashSprite.lock()->SetDrawLayer(4);
		m_backSprite.lock()->SetDrawLayer(4);
		m_itemNums.first.lock()->SetDrawLayer(4);
		m_itemNums.second.lock()->SetDrawLayer(4);
		m_limitNums.first.lock()->SetDrawLayer(4);
		m_limitNums.second.lock()->SetDrawLayer(4);
	}

	// 毎フレーム更新
	void ItemCountUI::OnUpdate()
	{
		// 数字の更新を行う
		UpdateNumbers();
	}

	// 数字の更新処理
	void ItemCountUI::UpdateNumbers()
	{
		// プレイヤーからアイテム数と上限数を取得
		const auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");
		int count = player->GetItemCount(m_itemType);
		int limit = player->GetLimitCount(m_itemType);

		// 数字スプライトの表示する数字を更新
		m_itemNums.first.lock()->SetNumber(ONE_PLACE(count));
		m_itemNums.second.lock()->SetNumber(TENS_PLACE(count));
		m_limitNums.first.lock()->SetNumber(ONE_PLACE(limit));
		m_limitNums.second.lock()->SetNumber(TENS_PLACE(limit));

		// 上限値に達してたら赤色にし、じゃなければ黒色にする
		bool isLimit = count >= limit;
		m_itemNums.first.lock()->SetDiffuseColor(isLimit ? COL_RED : COL_BLACK);
		m_itemNums.second.lock()->SetDiffuseColor(isLimit ? COL_RED : COL_BLACK);
	}
}