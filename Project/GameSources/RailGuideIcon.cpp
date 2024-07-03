/*!
@file RailGuideIcon.cpp
@brief レールを設置する場所のガイド表示
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void RailGuideIcon::OnCreate()
	{
		// ステージの取得
		const auto& stagePtr = GetStage();

		// プレイヤーのポインタを保持
		m_playerPtr = stagePtr->GetSharedGameObject<GamePlayer>(L"Player");

		// アイコンの生成
		for (size_t i = 0; i < m_iconVec.size(); i++)
		{
			auto& ptr = stagePtr->AddGameObject<Billboard>(L"GUIDE_RAIL_TX", m_deffScale, Vec3(0.0f));
			ptr->SetDrawActive(false);
			ptr->SetUpdateActive(false);

			m_iconVec.at(i) = ptr;
		}
	}

	// 毎フレーム更新
	void RailGuideIcon::OnUpdate()
	{
		// 一度非表示にする
		SetIconDraw(false);

		// ステージを取得し、イベント中なら非表示にして終了
		const auto& stagePtr = GetTypeStage<GameStage>();
		if (stagePtr->GetIsStaging()) return;

		// プレイヤーがレールを持ってるかで表示非表示
		const auto& player = stagePtr->GetSharedGameObject<GamePlayer>(L"Player");
		bool isHaveRail = player->GetStatus(ePlayerStatus::IsHaveRail);

		// プレイヤーがレールを所持しているなら
		if (isHaveRail)
		{
			// 各種更新処理
			UpdateIconMove();
			UpdateGuide();
			UpdateRange();
		}
	}

	// 動きの更新
	void RailGuideIcon::UpdateIconMove()
	{
		// カメラを取得
		const auto& camera = GetStage()->GetView()->GetTargetCamera();

		// 移動量を求める
		float moveVal = Utility::SinCurve(m_totalTime, 0.0f, m_boundValue);
		Quat rot = Utility::GetBillboardRotateQuat(camera->GetAt() - camera->GetEye(), Vec3(0.0f, m_totalTime, 0.0f));

		// 割合を加算
		m_totalTime += DELTA_TIME * XM_PI;
		if (m_totalTime >= XM_2PI) m_totalTime = 0.0f;

		// 座標の更新
		for (size_t i = 0; i < m_pastGuidePoint.size(); i++)
		{
			Point2D<size_t> point = m_pastGuidePoint.at(i);
			Vec3 pos = Vec3(float(point.y), m_deffPosY, -float(point.x));
			m_iconVec.at(i).lock()->SetPosition(pos + Vec3(0.0f, moveVal, 0.0f));
			m_iconVec.at(i).lock()->SetQuaternion(rot);
		}
	}

	// ガイドの更新
	void RailGuideIcon::UpdateGuide()
	{
		// ガイドのcsv上のポイント配列
		const auto& railManager = GetStage()->GetSharedGameObject<RailManager>(L"RailManager");
		const auto& guidePoints = railManager->GetGuidePoints();

		bool isIdentity = (guidePoints == m_pastGuidePoint);
		m_pastGuidePoint = guidePoints;

		// 座標の更新
		for (size_t i = 0; i < m_iconVec.size(); i++)
		{
			bool isRange = Utility::WithInElemRange(i, guidePoints.size());
			m_iconVec.at(i).lock()->SetDrawActive(isRange);
			if (!isRange) continue;
			if (isIdentity) continue;

			Point2D<size_t> point = guidePoints.at(i);
			m_iconVec.at(i).lock()->SetPosition(Vec3(float(point.y), m_deffPosY, -float(point.x)));
		}
	}

	// プレイヤーとの距離で表示するか
	void RailGuideIcon::UpdateRange()
	{
		// プレイヤーの取得
		const auto& playerPtr = m_playerPtr.lock();
		if (!playerPtr) return;

		// プレイヤーの座標
		Vec3 playerPos = playerPtr->GetPosition();

		// 距離を比較し、描画距離より離れていれば表示
		for (auto& icon : m_iconVec)
		{
			float length = (icon.lock()->GetPosition() - playerPos).length();
			icon.lock()->SetDrawActive(m_drawRange <= length);
		}
	}

	// アイコン全体の表示設定
	void RailGuideIcon::SetIconDraw(bool b)
	{
		// 座標の更新
		for (size_t i = 0; i < m_iconVec.size(); i++)
		{
			bool isRange = Utility::WithInElemRange(i, m_pastGuidePoint.size());
			m_iconVec.at(i).lock()->SetDrawActive(isRange && b);
		}
	}
}