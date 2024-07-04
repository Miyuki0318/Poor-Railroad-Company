#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void RailGuideIcon::OnCreate()
	{
		const auto& stagePtr = GetStage();

		for (size_t i = 0; i < m_iconVec.size(); i++)
		{
			auto& ptr = stagePtr->AddGameObject<Billboard>(L"GUIDE_RAIL_TX", m_deffScale, Vec3(0.0f));
			ptr->SetDrawActive(false);
			ptr->SetUpdateActive(false);

			m_iconVec.at(i) = ptr;
		}
	}

	void RailGuideIcon::OnUpdate()
	{
		// プレイヤーがレールを持ってるかで表示非表示
		const auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");
		bool isHaveRail = player->GetStatus(ePlayerStatus::IsHaveRail);

		SetIconDraw(false);

		if (isHaveRail)
		{
			// 各種更新処理
			UpdateIconMove();
			UpdateGuide();
		}
	}

	// 点滅の更新
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

	// アイコン全体の表示設定
	void RailGuideIcon::SetIconDraw(bool b)
	{
		for (auto& icon : m_iconVec)
		{
			icon.lock()->SetDrawActive(b);
		}
	}
}