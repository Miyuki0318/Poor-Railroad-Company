/*!
@file RailGuideBlinking.cpp
@brief レールを設置する場所のガイド表示
@author 小澤博貴
*/

#include "stdafx.h"
#include "RailGuideBlinking.h"
#include "GamePlayer.h"
#include "RailManager.h"

namespace basecross
{
	// 生成時の処理を実行
	void RailGuideBlinking::OnCreate()
	{
		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"RAIL");
		m_ptrDraw->SetTextureResource(L"RAIL_TX");
		m_ptrDraw->SetSpecular(COL_WHITE);
		m_ptrDraw->SetDiffuse(m_defColor);

		// 描画設定
		SetAlphaActive(true);
		SetDrawActive(false);
	}

	// 毎フレーム更新処理
	void RailGuideBlinking::OnUpdate()
	{
		// プレイヤーがレールを持ってるかで表示非表示
		const auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");
		bool isHaveRail = player->GetStatus(ePlayerStatus::IsHaveRail);
		if (isHaveRail)
		{
			// 各種更新処理
			UpdateBlinking();
			UpdateGuide();
		}

		SetDrawActive(isHaveRail);
	}

	// 点滅の更新
	void RailGuideBlinking::UpdateBlinking()
	{
		// 割合を加算
		m_blinkRatio += DELTA_TIME / m_blinkTime;
		if (m_blinkRatio >= XM_2PI) m_blinkRatio = 0.0f;

		// サインカーブを割合で求め、透明色から通常色の範囲で点滅させる
		Col4 blinkColor = Utility::SinCurve(m_blinkRatio, COL_ALPHA, m_defColor);
		m_ptrDraw->SetDiffuse(blinkColor);
	}

	// ガイドの更新
	void RailGuideBlinking::UpdateGuide()
	{
		// ガイドのcsv上のポイント配列
		const auto& railManager = GetStage()->GetSharedGameObject<RailManager>(L"RailManager");
		const auto& guidePoints = railManager->GetGuidePoints();

		// ガイドポイントに相違がなければ無視
		if (m_pastGuidePoint == guidePoints) return;
		m_pastGuidePoint = guidePoints;

		// 行列用変数と行列配列
		Mat4x4 matrix, mtxPosition, mtxRotation;
		auto& matrixVec = m_ptrDraw->GetMatrixVec();
		matrixVec.clear();
		
		// 最後のレール設置座標
		Vec3 pastPos = railManager->GetPastRailPos();

		// 配列の数ループ
		for (const auto& guide : guidePoints)
		{
			// 行列の設定と追加
			Vec3 addPos = ROWCOL2POS(guide.x, guide.y);
			mtxPosition.translation(addPos);
			mtxRotation = pastPos.x != addPos.x ? m_mtxRotAxisX : m_mtxRotAxisZ;
			matrix = m_mtxScale * mtxRotation * mtxPosition;
			matrixVec.push_back(matrix);
		}
	}
}