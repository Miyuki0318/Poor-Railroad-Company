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
	bool CraftManager::CraftOrder()
	{
		// eCraftItem item = m_selectIcon.lock()->GetSelectItem();
		eCraftItem item = eCraftItem::Rail;

		// 素材要求数の取得
		const int woodValue = GetRacipeValue(item, eCraftParam::WoodValue);
		const int stoneValue = GetRacipeValue(item, eCraftParam::StoneValue);

		// 作成可能かの真偽を返す
		return GetCraftPossible(woodValue, stoneValue);
	}

	// クラフトウィンドウの呼び出し
	void CraftManager::CraftingEnabled(bool enable)
	{
		// QTEとウィンドウオブジェクトの取得
		const auto& qte = m_craftQTE.lock();
		const auto& window = m_window.lock();
		if (qte && window)
		{
			// 描画状態設定を送る
			qte->SetDrawEnable(enable, window->GetPosition());
			window->SetDrawEnable(enable);

			// スプライトの中心位置設定を送る(条件未設定だから一旦右上固定)
			//eVerticesRect rect = GetCraftWindowDrawRect();
			eVerticesRect rect = eVerticesRect::UpperRight;
			qte->SetVerticesRect(rect);
			window->SetVerticesRect(rect);
		}
	}

	// QTEの開始呼び出し
	void CraftManager::StartQTE()
	{
		// QTEとウィンドウオブジェクトの取得
		const auto& qte = m_craftQTE.lock();
		const auto& window = m_window.lock();
		if (qte && window)
		{
			// qteオブジェクトにQTE開始呼び出しを送る
			qte->StartQTE(window->GetPosition());
		}
	}

	// QTEの停止呼び出し
	void CraftManager::StopQTE()
	{
		// qteと選択アイコンオブジェクトを取得
		const auto& qte = m_craftQTE.lock();
		//const auto& selectIcon = m_selectIcon.lock();
		//if (qte && selectIcon)
		if (qte)
		{
			// eCraftItem item = m_selectIcon.lock()->GetSelectItem();
			eCraftItem item = eCraftItem::Rail;

			// QTE停止呼び出しとQTE結果の真偽を取得
			bool succes = qte->StopQTE();

			// 素材消費
			UseItem(eItemType::Wood, GetRacipeValue(item, eCraftParam::WoodValue));
			UseItem(eItemType::Stone, GetRacipeValue(item, eCraftParam::StoneValue));

			// QTE結果に応じて作成量を設定
			AddItemCount(eItemType::Rail, GetRacipeValue(item, succes ? eCraftParam::SuccesValue : eCraftParam::FailedValue));
		}
	}
}