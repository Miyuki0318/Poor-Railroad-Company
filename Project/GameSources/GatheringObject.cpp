/*!
@file GatheringObject.cpp
@brief 採取できるオブジェクト
@author 小宅碧
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void InstanceGathers::OnCreate()
	{
		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
	}

	// メッシュデータを登録
	void InstanceGathers::SetMeshData(const wstring& meshKey, const wstring& texKey)
	{
		m_ptrDraw->SetMeshResource(meshKey);
		m_ptrDraw->SetTextureResource(texKey);
	}

	// 行列に追加
	void InstanceGathers::AddMatrix(const Mat4x4& matrix)
	{
		m_ptrDraw->AddMatrix(matrix);
	}

	// 行列を全削除
	void InstanceGathers::AllClearMatrix()
	{
		m_ptrDraw->ClearMatrixVec();
	}

	// 行列配列を取得
	vector<Mat4x4>& InstanceGathers::GetMatrixVec()
	{
		return m_ptrDraw->GetMatrixVec();
	}

	// 生成時の処理
	void GatheringManager::OnCreate()
	{
		// ステージの取得
		const auto& stagePtr = GetStage();

		// 各種IDのインスタンス描画用オブジェクトを生成してmapに保持
		m_gathersMap.emplace(eStageID::Stone1, stagePtr->AddGameObject<InstanceGathers>());
		m_gathersMap.emplace(eStageID::Stone2, stagePtr->AddGameObject<InstanceGathers>());
		m_gathersMap.emplace(eStageID::Stone3, stagePtr->AddGameObject<InstanceGathers>());
		m_gathersMap.emplace(eStageID::Tree1, stagePtr->AddGameObject<InstanceGathers>());
		m_gathersMap.emplace(eStageID::Tree2, stagePtr->AddGameObject<InstanceGathers>());

		// メッシュ設定を送る
		SetGathersMeshData();

		// 開始時の採取オブジェクト生成処理を送る
		ResetGathering();
	}

	// メッシュキーと差分行列の設定
	void GatheringManager::SetGathersMeshData()
	{
		// 各種IDに応じて設定
		m_gathersMap.at(eStageID::Stone1).lock()->SetMeshData(L"ROCK1", L"ROCK_SM_TX");
		m_gathersMap.at(eStageID::Stone2).lock()->SetMeshData(L"ROCK2", L"ROCK_SM_TX");
		m_gathersMap.at(eStageID::Stone3).lock()->SetMeshData(L"ROCK3", L"ROCK_SM_TX");
		m_gathersMap.at(eStageID::Tree1).lock()->SetMeshData(L"TREE1", L"TREE_SM_TX");
		m_gathersMap.at(eStageID::Tree2).lock()->SetMeshData(L"TREE2", L"TREE_SM_TX");
	}

	// 初期生成
	void GatheringManager::ResetGathering()
	{
		// 行列を全て初期化
		for (auto& gathers : m_gathersMap)
		{
			gathers.second.lock()->AllClearMatrix();
		}

		// ステージcsvを取得し2重ループ
		const auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		for (int row = 0; row < stageMap.size(); row++)
		{
			for (int col = 0; col < stageMap.at(row).size(); col++)
			{
				// IDに応じて行列を追加
				eStageID id = STAGE_ID(stageMap.at(row).at(col));
				if (!GetBetween(id, eStageID::Stone1, eStageID::Tree2)) continue;
				if (m_gathersMap.find(id) == m_gathersMap.end()) continue;

				// トランスフォーム行列の設定
				Mat4x4 matrix, mtxPosition, mtxRotation, mtxScale;
				mtxPosition.translation(ROWCOL2POS(row, col));
				mtxRotation.rotation(Quat().rotationRollPitchYawFromVector(Vec3(0.0f, RangeRand(2, -1) * XM_PIDIV2, 0.0f)));
				mtxScale.scale(m_fullScale);
				
				// 行列の設定と追加
				matrix = mtxScale * mtxRotation * mtxPosition;

				// 行列を追加
				m_gathersMap.at(id).lock()->AddMatrix(matrix);
			}
		}
	}

	// 採集処理
	int GatheringManager::Gathering(const Point2D<size_t>& point)
	{
		// ステージのcsvの取得
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return 0;

		// ステージIDに変換
		int num = stageMap.at(point.x).at(point.y);
		eStageID id = STAGE_ID(num);
		if (m_gathersMap.find(id) == m_gathersMap.end()) return 0;

		// 行列のスケールを変更するか行列そのものを除外するか
		vector<Mat4x4> tempMat;
		auto& matrixVec = m_gathersMap.at(id).lock()->GetMatrixVec();
		for (auto& matrix : matrixVec)
		{
			// 座標が一致したら
			if (matrix.getTranslation() == ROWCOL2POS(point.x, point.y))
			{
				Vec3 scale = matrix.scaleInMatrix().round(1); // スケール

				// スケールがハーフスケールならCSVを書き換え
				if (scale == m_helfScale) HelfSizeGathering(point);

				// スケールがフルスケールならハーフスケールに書き換えて追加
				if (scale == m_fullScale) tempMat.push_back(FullSizeGathering(matrix));
				
				continue;
			}
			// 追加
			tempMat.push_back(matrix);
		}
		// 行列配列を置き換えてIDを返す
		matrixVec.swap(tempMat);
		return num;
	}

	// ハーフサイズでの採取
	void GatheringManager::HelfSizeGathering(const Point2D<size_t>& point)
	{
		// ステージの取得
		const auto& stagePtr = GetTypeStage<BaseStage>();

		// ステージのcsvの書き換え
		auto& stageMap = stagePtr->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return;
		stageMap.at(point.x).at(point.y) = UnSTAGE_ID(eStageID::None);

		// レールマネージャーにガイドの再生成関数を送る
		stagePtr->GetSharedGameObject<RailManager>(L"RailManager")->GuideRecalculation();

		// お宝チェックを行い、お宝があればプレイヤーにアイテム追加処理を送る
		auto& addItem = stagePtr->GetSharedGameObject<GatherTreasure>(L"GatherTreasure")->TreasureCheck(ROWCOL2POS(point.x, point.y));
		if (addItem.second == 0) return;

		stagePtr->GetSharedGameObject<GamePlayer>(L"Player")->AddItemCountFly(addItem.first, addItem.second);
	}

	// フルサイズでの採取
	Mat4x4 GatheringManager::FullSizeGathering(Mat4x4& matrix)
	{
		// ハーフサイズスケールで行列を作成し返す
		Mat4x4 mat, tMat, rMat, sMat;
		tMat.translation(matrix.getTranslation());
		rMat.rotation(matrix.quatInMatrix());
		sMat.scale(m_helfScale);
		mat = sMat * rMat * tMat;
		return mat;
	}

	// 指定の座標に採集オブジェクトがあるか
	bool GatheringManager::GetIsGatheringPoint(const Point2D<size_t>& point) const
	{
		// 配列の範囲内かのエラーチェック
		const auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return false;

		// 採集オブジェクトのIDか
		eStageID id = STAGE_ID(stageMap.at(point.x).at(point.y));
		return GetBetween(id, eStageID::Stone1, eStageID::Tree2);
	}
}