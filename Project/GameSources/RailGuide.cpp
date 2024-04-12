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
		m_ptrDraw = AddComponent<PCTStaticInstanceDraw>();
		m_ptrDraw->SetOriginalMeshUse(true);
		m_ptrDraw->CreateOriginalMesh(m_vertex);
		m_ptrDraw->SetTextureResource(L"ICON_RAIL_TX");

		// 描画設定
		SetAlphaActive(true);
		SetDrawActive(false);
	}

	// 毎フレーム更新処理
	void RailGuide::OnUpdate()
	{
		// 各種更新処理
		UpdateBillboard();
		UpdateGuide();

		// プレイヤーがレールを持ってるかで表示非表示
		const auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");
		SetDrawActive(player->GetStatus(ePlayerStatus::IsHaveRail));
	}

	// ビルボードの更新
	void RailGuide::UpdateBillboard()
	{
		// カメラを元にビルボード関数でクォータニオンを設定
		const auto& stage = GetStage();
		const auto& ptrCamera = stage->GetView()->GetTargetCamera();
		Quat qt = Utility::GetBillboardQuat(ptrCamera->GetAt() - ptrCamera->GetEye());
		m_mtxRotation.rotation(qt);
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