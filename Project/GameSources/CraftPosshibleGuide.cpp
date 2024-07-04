/*!
@file CraftPosshibleGuide.cpp
@brief クラフト可能になった時に出るエフェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void CraftPosshibleGuide::OnCreate()
	{
		// ステージからプレイヤーとエフェクトを取得
		const auto& stagePtr = GetStage();
		m_playerPtr = stagePtr->GetSharedGameObject<GamePlayer>(L"Player");
		m_effect = stagePtr->GetSharedGameObject<SpriteParticle>(L"CraftEffect");
	}

	// 毎フレーム更新処理
	void CraftPosshibleGuide::OnUpdate()
	{
		// プレイヤーが存在しなければ終了
		const auto& player = m_playerPtr.lock();
		if (!player) return;

		// クラフトマネージャーが存在しなければ終了
		const auto& craft = player->GetCraftManager();
		if (!craft) return;

		// クラフトできるかの真偽を設定
		Bool8_t<eCraftItem> tempFlag;
		tempFlag.Set(eCraftItem::Rail) = craft->CraftOrder(eCraftItem::Rail);
		tempFlag.Set(eCraftItem::WoodBridge) = craft->CraftOrder(eCraftItem::WoodBridge);
		tempFlag.Set(eCraftItem::Crossing) = craft->CraftOrder(eCraftItem::Crossing);
		
		// 前回の状態と同じなら終了
		if (m_posshible == tempFlag) return;

		// クラフトできる状態になったかのチェックと呼び出し
		CheckPosshible(tempFlag, eCraftItem::Rail);
		CheckPosshible(tempFlag, eCraftItem::WoodBridge);
		CheckPosshible(tempFlag, eCraftItem::Crossing);

		// 状態を保持
		m_posshible = tempFlag;
	}

	// 確認処理と呼び出し処理
	void CraftPosshibleGuide::CheckPosshible(Bool8_t<eCraftItem>& flag, eCraftItem type)
	{
		// 現在がクラフト可能で、前回はクラフト不可だったら
		if (flag(type) && !m_posshible(type))
		{
			// エフェクトを開始
			StartEffect(type);

			// サウンドアイテムが存在しない、またはリソースが空なら
			if (auto& item = m_seItem.lock()) if (item->m_AudioResource.lock()) return;
			m_seItem = StartSE(L"POSSHIBLE_SE", 1.0f);
		}
	}

	// エフェクト開始処理
	void CraftPosshibleGuide::StartEffect(eCraftItem type)
	{
		// エミッターの座標をマップから取得
		Vec3 emitterPos = m_uiPosMap.at(type);

		// エフェクトにエミッター座標と開始処理を送る
		m_effect.lock()->SetEmitterPosition(emitterPos);
		m_effect.lock()->AddParticle(m_effScale, Vec2(0.0f), 0.0f, m_drawTime);
	}
}