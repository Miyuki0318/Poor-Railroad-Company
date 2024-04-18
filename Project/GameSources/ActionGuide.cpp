/*!
@file ActionGuide.cpp
@brief 行動のガイドアイコン描画
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理を実行
	void ActionGuide::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Billboard::OnCreate();

		// 非表示に
		SetDrawActive(false);
	}

	// 毎フレーム更新処理
	void ActionGuide::OnUpdate()
	{
		// ビルボードと座標の更新
		UpdateBillboard();
		UpdatePosition();

		// アイコンタイプの更新
		m_iconType = eActionIcon::None;
		eActionIcon icon = eActionIcon::None;
		UpdateIconType();

		// 変更があればテクスチャの変更
		if (icon != m_iconType) SetTexture(m_texMap.at(m_iconType));

		// 表示非表示切り替え
		SetDrawActive(m_iconType != eActionIcon::None);
	}

	// 座標の更新
	void ActionGuide::UpdatePosition()
	{
		// プレイヤーが存在するかのエラーチェック
		const auto& player = m_player.lock();
		if (!player) return;

		// プレイヤーの座標+頭上分の座標
		SetPosition(player->GetPosition() + m_diffPosition);
	}

	// アイコンタイプの更新
	void ActionGuide::UpdateIconType()
	{
		// プレイヤーとインディケーターが存在するか
		const auto& player = m_player.lock();
		const auto& indicator = m_indicator.lock();
		if (!player || !indicator) return;

		// クラフト可能ならクラフトアイコン
		if (player->GetCraftPosshible()) m_iconType = eActionIcon::Craft;

		// ステージcsvIDに応じて設定
		const auto& stageMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetGuideMap();
		const auto& selectPoint = indicator->GetSelectPoint();
		if (Utility::WithInElemRange(selectPoint.x, selectPoint.y, stageMap))
		{
			// IDに応じてアイコンタイプを設定
			eStageID id = static_cast<eStageID>(stageMap.at(selectPoint.x).at(selectPoint.y));
			if (m_iconMap.find(id) != m_iconMap.end()) m_iconType = m_iconMap.at(id);

			// レールを未所持ならNoneに
			if (m_iconType == eActionIcon::Rail)
			{
				if (!player->GetStatus(ePlayerStatus::IsHaveRail))
				{
					if (player->GetCraftPosshible())
					{
						m_iconType = eActionIcon::Craft;
					}
					else
					{
						m_iconType = eActionIcon::None;
					}
				}
			}
		}
	}
}