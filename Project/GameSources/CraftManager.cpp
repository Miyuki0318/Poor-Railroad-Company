/*!
@file CraftManager.cpp
@brief クラフト管理用マネージャ
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// アイテムクラフト
	bool CraftManager::Crafting(eCraftItem item)
	{
		// 素材要求数の取得
		const int woodValue = GetRacipeValue(item, eCraftParam::WoodValue);
		const int stoneValue = GetRacipeValue(item, eCraftParam::StoneValue);

		// 作成可能かの真偽を返す
		return GetCraftPossible(woodValue, stoneValue);
	}

	// クラフトウィンドウの呼び出し
	void CraftManager::CraftingEnabled(bool enable)
	{
		// ウィンドウオブジェクトに描画呼び出しを送る
		const auto& window = m_window.lock();
		if (window)
		{
			// ウィンドウの方向と描画状態設定を送る(条件未設定だから一旦右上固定)
			window->SetWindowRect(eWindowRect::UpperRight);
			window->SetDrawEnable(enable);
		}
	}

	// QTEの開始呼び出し
	void CraftManager::StartQTE()
	{
		// qteオブジェクトにQTE開始呼び出しを送る
		const auto& qte = m_craftQTE.lock();
		if (qte)
		{
			qte->StartQTE();
		}
	}

	// QTEの停止呼び出し
	void CraftManager::StopQTE(eCraftItem item)
	{
		// qteオブジェクトを取得
		const auto& qte = m_craftQTE.lock();
		if (qte)
		{
			// QTE停止呼び出しとQTE結果の真偽を取得
			bool succes = qte->StopQTE();

			// 素材消費
			AddItemCount(eItemType::Wood, -GetRacipeValue(item, eCraftParam::WoodValue));
			AddItemCount(eItemType::Stone, -GetRacipeValue(item, eCraftParam::StoneValue));

			// QTE結果に応じて作成量を設定
			AddItemCount(eItemType::Rail, GetRacipeValue(item, succes ? eCraftParam::SuccesValue : eCraftParam::FailedValue));
		}
	}
}