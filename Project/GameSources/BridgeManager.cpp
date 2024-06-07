/*!
@file BridgeManager.cpp
@brief 木の足場管理クラス
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void BridgeManager::OnCreate()
	{
		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"WOOD_BRIDGE");
		m_ptrDraw->SetTextureResource(L"WOOD_BRIDGE_TX");
		m_ptrDraw->SetSpecular(COL_WHITE);
		m_ptrDraw->SetDiffuse(COL_WHITE);
	}

	// 初期化処理
	void BridgeManager::ResetBridge()
	{
		m_ptrDraw->ClearMatrixVec();
	}

	// 橋の追加生成
	void BridgeManager::AddBridge(const Point2D<size_t>& point)
	{
		// ステージのcsvの取得
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return;

		// 座標の設定
		Vec3 addPos = ROWCOL2POS(point.x, point.y);

		// トランスフォーム行列の設定
		Mat4x4 matrix, mtxPosition;
		mtxPosition.translation(addPos);

		// 行列の設定と追加
		matrix = m_mtxScale * m_mtxRotation * mtxPosition;
		m_ptrDraw->AddMatrix(matrix);

		// csvの書き換え
		stageMap.at(point.x).at(point.y) = UnSTAGE_ID(eStageID::WoodBridge);
	}

	// 指定の座標に水場があるか
	bool BridgeManager::GetIsWaterPoint(const Point2D<size_t>& point) const
	{
		// 配列の範囲内かのエラーチェック
		const auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return false;

		// 水場のIDかどうか
		return eStageID::Water == STAGE_ID(stageMap.at(point.x).at(point.y));
	}
}