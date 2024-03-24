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
	void CraftManager::Crafting(const shared_ptr<TemplateObject>& tempPtr)
	{
		// クラフトアイコン等から選択肢を取得する機能をα版で追加
		// またQTEが無い為、今回は固定で１個クラフトできる状態

		// 型キャスト
		const auto& playerPtr = dynamic_pointer_cast<Player>(tempPtr);
		if (!playerPtr) return;

		// 簡易実装
		const int woodValue = GetRacipeValue(eCraftItem::Rail, eCraftParam::WoodValue);
		const int stoneValue = GetRacipeValue(eCraftItem::Rail, eCraftParam::StoneValue);

		// 素材要求数に足りてなければ終了
		if (woodValue > playerPtr->GetItemCount(eItemType::Wood)) return;
		if (stoneValue > playerPtr->GetItemCount(eItemType::Stone)) return;

		// 素材消費とクラフト
		playerPtr->AddItemCount(eItemType::Wood, -woodValue);
		playerPtr->AddItemCount(eItemType::Stone, -stoneValue);
		playerPtr->AddItemCount(eItemType::Rail, GetRacipeValue(eCraftItem::Rail, eCraftParam::SuccesValue));
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
}