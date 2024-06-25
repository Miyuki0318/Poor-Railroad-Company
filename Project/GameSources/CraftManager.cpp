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

	void CraftManager::OnCreate()
	{
		const auto& stagePtr = m_player.lock()->GetStage();

		m_iconMap.emplace(eCraftItem::Rail, stagePtr->AddGameObject<CraftItemIcon>(L"C_RAIL_TX"));
		m_iconMap.emplace(eCraftItem::WoodBridge, stagePtr->AddGameObject<CraftItemIcon>(L"C_BRIDGE_TX"));
		m_iconMap.emplace(eCraftItem::Crossing, stagePtr->AddGameObject<CraftItemIcon>(L"C_CROSSING_TX"));
	}

	// リセット処理
	void CraftManager::ResetCraftManager()
	{
		m_craftItem = eCraftItem::Rail;
		m_craftType = eItemType::Rail;

		// アイテムカウンタ
		m_itemCount.at(eItemType::Wood) = 0;
		m_itemCount.at(eItemType::Stone) = 0;
		m_itemCount.at(eItemType::Gear) = 1;
		m_itemCount.at(eItemType::Rail) = 0;
		m_itemCount.at(eItemType::WoodBridge) = 0;
		m_itemCount.at(eItemType::Crossing) = 0;
		m_itemCount.at(eItemType::GoldBar) = 0;
	}

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
			Vec3 windowRect = Vec3((WINDOW_WIDTH / 2.0f), WINDOW_HEIGHT / 2.0f, 1.0f);
			windowPos.clamp(-windowRect, windowRect);
			windowPos.z = 0.0f;
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
			window->SetDrawEnable(enable, windowPos + Vec3(0.0f, 0.0f, 0.1f));
			qte->SetDrawEnable(enable, window->GetPosition());

			// アイコンにも設定を送る
			for (auto& icon : m_iconMap)
			{
				auto& ptr = icon.second.lock();
				ptr->SetVerticesRect(rect);
				ptr->SetRectType(rect);
				ptr->SetCraftPosshible(CraftOrder(icon.first));
				ptr->SetDrawEnable(enable, windowPos);
			}
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
			qte->SetButtonTexture(m_inputButton.at(item));
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
			
			// 踏切の作成失敗なら素材を消費せず終了
			if (m_craftItem == eCraftItem::Crossing && !succes) return false;

			// 素材消費
			UseItem(eItemType::Wood, GetRacipeValue(m_craftItem, eCraftParam::WoodValue));
			UseItem(eItemType::Stone, GetRacipeValue(m_craftItem, eCraftParam::StoneValue));
			UseItem(eItemType::Gear, GetRacipeValue(m_craftItem, eCraftParam::GearValue));

			// QTE結果に応じて作成量を設定
			AddItemCount(m_craftType, GetRacipeValue(m_craftItem, succes ? eCraftParam::SuccesValue : eCraftParam::FailedValue));

			// クラフトできるかを更新
			for (auto& icon : m_iconMap)
			{
				icon.second.lock()->SetCraftPosshible(CraftOrder(icon.first));
			}
		}

		return succes;
	}

	// QTEの破棄
	void CraftManager::DestroyCraftQTE()
	{
		m_craftQTE.lock()->StopQTE();
	}
}