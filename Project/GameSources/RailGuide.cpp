/*!
@file RailGuide.cpp
@brief レールを設置する場所のガイド表示
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理を実行
	void RailGuide::OnCreate()
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
	void RailGuide::OnUpdate()
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

		Debug::Log(L"ガイドの透明度", m_ptrDraw->GetDiffuse().w);

		SetDrawActive(isHaveRail);
	}

	// 点滅の更新
	void RailGuide::UpdateBlinking()
	{
		// 割合を加算
		m_blinkRatio += DELTA_TIME / m_blinkTime;
		if (m_blinkRatio >= XM_2PI) m_blinkRatio = 0.0f;

		// サインカーブを割合で求め、透明色から通常色の範囲で点滅させる
		Col4 blinkColor = Utility::SinCurve(m_blinkRatio, COL_ALPHA, m_defColor);
		m_ptrDraw->SetDiffuse(blinkColor);
	}

	// ガイドの更新
	void RailGuide::UpdateGuide()
	{
		// 行列用変数と行列配列
		Mat4x4 matrix, mtxPosition;
		auto& matrixVec = m_ptrDraw->GetMatrixVec();
		matrixVec.clear();
		
		// ガイドのcsv上のポイント配列
		const auto guidePoints = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetGuidePoints();
		
		// 配列の数ループ
		for (const auto& guide : guidePoints)
		{
			// 行列の設定と追加
			Vec3 addPos = Vec3(float(guide.y), m_posY, -float(guide.x));
			mtxPosition.translation(addPos);
			matrix = m_mtxScale * m_mtxRotation * mtxPosition;
			matrixVec.push_back(matrix);
		}
	}
}