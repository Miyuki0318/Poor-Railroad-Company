/*!
@file GoalGuide.cpp
@brief ゴール駅方向を表示するガイドUI
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void GoalGuide::OnCreate()
	{
		// 継承元の生成時の処理
		Sprite::OnCreate();

		// スプライトの生成とゴール駅の取得
		const auto& stagePtr = GetStage();
		m_arrowSprite = stagePtr->AddGameObject<Sprite>(L"GOAL_ARROW_TX", Vec2(m_scale));
		m_goalStation = stagePtr->GetSharedGameObject<TemplateObject>(L"Station");
	}

	// 毎フレーム更新
	void GoalGuide::OnUpdate()
	{
		// ビューを取得し、2D座標変換を行う
		const auto& view = GetStage()->GetView();
		Vec3 stationPos = ConvertToWorldPosition(view, m_goalStation.lock()->GetPosition());
		stationPos.z = 0.0f;

		// 画面内にクリッピングする
		Vec3 pos = WindowClipLineVec(Vec3(0.0f), stationPos);
		pos.z = 0.5f;

		// ウィンドウサイズからスケール分を引いた数でクランプする
		Vec3 windowMax = Vec3((WINDOW_SIZE / 2.0f) - Vec2(m_scale / 2.0f), 1.0f);
		pos.clamp(-windowMax, windowMax);		

		// 矢印の角度を求める
		float rad = atan2f(stationPos.y - pos.y, stationPos.x - pos.x);

		// 座標の更新など
		SetPosition(pos);
		m_arrowSprite.lock()->SetPosition(pos + Vec3(0.0f, 0.0f, 0.1f));
		m_arrowSprite.lock()->SetRotation(Vec3(0.0f, 0.0f, rad));

		// ゴール駅が画面内なら非表示
		bool isDraw = !GetBetween(stationPos, -windowMax, windowMax);
		SetDrawActive(isDraw);
		m_arrowSprite.lock()->SetDrawActive(isDraw);
	}
}