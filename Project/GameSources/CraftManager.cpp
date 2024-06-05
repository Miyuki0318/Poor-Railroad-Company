/*!
@file CraftManager.cpp
@brief クラフト管理用マネージャ
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Input;

	// アイテムクラフト
	bool CraftManager::CraftOrder(eCraftItem item)
	{
		// 素材要求数の取得
		const int woodValue = GetRacipeValue(item, eCraftParam::WoodValue);
		const int stoneValue = GetRacipeValue(item, eCraftParam::StoneValue);
		const int gearValue = GetRacipeValue(item, eCraftParam::GearValue);

		// 作成可能かの真偽を返す
		return GetCraftPossible(woodValue, stoneValue, gearValue);
	}

	// クラフトウィンドウの呼び出し
	void CraftManager::CraftingEnabled(bool enable)
	{
		// QTEとウィンドウオブジェクトの取得
		const auto& qte = m_craftQTE.lock();
		const auto& window = m_window.lock();
		const auto& player = m_player.lock();
		if (qte && window && player)
		{
			// スプライトの中心位置設定を送る
			Vec3 playerPos = player->GetPosition();
			Vec3 windowPos = Utility::ConvertToWorldPosition(player->GetStage()->GetView(), playerPos);
			eRectType rect = eRectType::DownLeft;
			if (windowPos.x < 0.0f) rect = eRectType::DownRight;
			if (windowPos.y < 0.0f) rect = eRectType::UpLeft;
			if (windowPos.x < 0.0f && windowPos.y < 0.0f) rect = eRectType::UpRight;

			// 頂点座標を変更
			qte->SetVerticesRect(rect);
			window->SetVerticesRect(rect);

			// 描画方向を設定
			qte->SetRectType(rect);
			window->SetRectType(rect);

			// 描画状態設定を送る
			window->SetDrawEnable(enable);
			qte->SetDrawEnable(enable, window->GetPosition());
		}
	}

	// QTEの開始呼び出し
	void CraftManager::StartQTE(eCraftItem item, eItemType type)
	{
		// QTEとウィンドウオブジェクトの取得
		const auto& qte = m_craftQTE.lock();
		const auto& window = m_window.lock();
		if (qte && window)
		{
			// qteオブジェクトにQTE開始呼び出しを送る
			qte->StartQTE(window->GetPosition());
			m_craftItem = item;
			m_craftType = type;
		}
	}

	// QTEの停止呼び出し
	bool CraftManager::StopQTE()
	{
		bool succes = false;

		// qteとオブジェクトを取得
		if (auto qte = m_craftQTE.lock())
		{
			// QTE停止呼び出しとQTE結果の真偽を取得
			succes = qte->StopQTE();

			if (m_craftItem == eCraftItem::Crossing && !succes) return false;

			// 素材消費
			UseItem(eItemType::Wood, GetRacipeValue(m_craftItem, eCraftParam::WoodValue));
			UseItem(eItemType::Stone, GetRacipeValue(m_craftItem, eCraftParam::StoneValue));
			UseItem(eItemType::Gear, GetRacipeValue(m_craftItem, eCraftParam::GearValue));

			// QTE結果に応じて作成量を設定
			AddItemCount(m_craftType, GetRacipeValue(m_craftItem, succes ? eCraftParam::SuccesValue : eCraftParam::FailedValue));
		}

		return succes;
	}
}