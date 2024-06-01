/*!
@file Crossing.cpp
@brief 踏切と踏切管理クラス
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;
	
	// 生成時の処理
	void Crossing::OnCreate()
	{
		// 継承元の生成時の処理を実行
		TemplateObject::OnCreate();

		// トランスフォームの設定
		size_t row, col;
		GetLineStringToRowCol(row, col, m_csvLine);
		SetPosition(ROWCOL2POS(row, col));
		SetScale(1.0f);

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(L"CROSSING");
		m_ptrDraw->AddAnimation(L"OPEN", 0, 15, false);
		m_ptrDraw->AddAnimation(L"CROSS", 20, 15, false);
	}

	// 毎フレーム更新処理
	void Crossing::OnUpdate()
	{
		// アニメーションの更新
		m_ptrDraw->UpdateAnimation(DELTA_TIME);

		// アニメーションが終了していないならこれ以降は無視
		if (!m_ptrDraw->IsTargetAnimeEnd()) return;

		// csvのrowとcolを取得
		size_t row, col;
		GetLineStringToRowCol(row, col, m_csvLine);

		// ステージcsvから自身の位置のIDを取得
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		eStageID id = STAGE_ID(stageMap.at(row).at(col));

		// アニメーションがOPENで、閉じている時のIDなら開いている時のIDに切り替える
		if (m_ptrDraw->GetCurrentAnimation() == L"OPEN" && eStageID::CrossingCross == id)
		{
			stageMap.at(row).at(col) = UnSTAGE_ID(eStageID::CrossingOpen);
		}

		// アニメーションがCROSSで、開いている時のIDなら閉じている時のIDに切り替える
		if (m_ptrDraw->GetCurrentAnimation() == L"CROSS" && eStageID::CrossingOpen == id)
		{
			stageMap.at(row).at(col) = UnSTAGE_ID(eStageID::CrossingCross);
		}
	}

	// 生成時の処理
	void CrossingManager::OnCreate()
	{
		// ステージのポインタを取得
		const auto& stagePtr = GetStage();

		// 列車のポインタを保持
		m_trainPtr = stagePtr->GetSharedGameObject<GameTrain>(L"Train");

		// レールデータマップのポインタを保持
		m_railDataMap = &stagePtr->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();
	}

	// 毎フレーム更新処理
	void CrossingManager::OnUpdate()
	{
		// 列車のポインタが無ければ無視
		if (!m_trainPtr.lock()) return;

		// 列車の座標を取得
		Vec3 trainPos = m_trainPtr.lock()->GetPosition();

		// 踏切を全て確認
		for (auto& crossing : m_crossingMap)
		{
			auto& ptr = crossing.second.lock(); // シェアドポインタの取得

			// 距離を求め、開閉するかや更新処理をするかを設定
			float length = (ptr->GetPosition() - trainPos).length();
			length <= m_openRange ? ptr->SetCross() : ptr->SetOpen();
			ptr->SetUpdateActive(length <= m_updateRange);
		}
	}

	// 指定の座標にガイドがあるか
	bool CrossingManager::GetIsRailPoint(const Point2D<size_t>& point) const
	{
		// ステージの取得
		const auto& stagePtr = GetTypeStage<BaseStage>();

		// 配列の範囲内かのエラーチェック
		const auto& stageMap = stagePtr->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return false;

		// レールIDかどうか
		if (eStageID::Rail != STAGE_ID(stageMap.at(point.x).at(point.y))) return false;

		// レールが直線レールかどうか
		if (m_railDataMap->find(ROWCOL2LINE(point.x, point.y)) == m_railDataMap->end()) return false;
		if (m_railDataMap->at(ROWCOL2LINE(point.x, point.y)).angle != eRailAngle::Straight) return false;

		// 全ての条件を通過したらtrue
		return true;
	}

	// 踏切の追加処理
	void CrossingManager::AddCrossing(const Point2D<size_t>& point)
	{
		// ステージのcsvの取得
		const auto& stagePtr = GetTypeStage<BaseStage>();
		auto& stageMap = stagePtr->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return;

		// 踏切の生成
		string addLine = ROWCOL2LINE(point.x, point.y);
		auto& crossing = stagePtr->AddGameObject<Crossing>(addLine);
		m_crossingMap.emplace(addLine, crossing);

		// レールの向きに応じて差分行列を設定
		crossing->SetModelMatrix(m_railDataMap->at(addLine).type == eRailType::AxisXLine ? m_xLineModelMat : m_zLineModelMat);

		// csvの書き換え
		stageMap.at(point.x).at(point.y) = UnSTAGE_ID(eStageID::CrossingCross);
	}
}