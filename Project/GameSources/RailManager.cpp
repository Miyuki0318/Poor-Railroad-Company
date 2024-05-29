/*!
@file RailManager.cpp
@brief レール管理クラス
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 行列計算に使うローテーションとスケール
	const Mat4x4 mtxRotation = Mat4x4().rotation((Quat)XMQuaternionRotationRollPitchYawFromVector(Vec3(0.0f, XM_PIDIV2, 0.0f)));
	const Mat4x4 mtxScale = Mat4x4().scale(Vec3(0.675f));

	// lineからrowとcolを抽出
	void GetLineStringToRowCol(size_t& row, size_t& col, string line)
	{
		// 文字列を '-' を境に分割
		istringstream iss(line);
		string token;

		// tokenを数値に変換してrowに格納
		getline(iss, token, '-');
		row = stoul(token);

		// tokenを数値に変換してcolに格納
		getline(iss, token);
		col = stoul(token);
	}

	// 生成時の処理
	void RailManager::OnCreate()
	{
		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"RAIL");
		m_ptrDraw->SetTextureResource(L"RAIL_TX");
		m_ptrDraw->SetSpecular(COL_WHITE);
		m_ptrDraw->SetDiffuse(COL_WHITE);

		// csvの取得
		const auto& stagePtr = GetTypeStage<BaseStage>();
		const auto& stageMap = stagePtr->GetStageMap();
		m_guideMap = stageMap;

		// レール描画生成
		ResetInstanceRail();
	}

	// インスタンス描画のレールを生成
	void RailManager::ResetInstanceRail()
	{
		// ステージとCSVの取得
		const auto& stagePtr = GetTypeStage<BaseStage>();
		const auto& stageMap = stagePtr->GetStageMap();

		// 初期化
		m_ptrDraw->ClearMatrixVec();

		// 二重ループ
		for (size_t row = 0; row < stageMap.size(); row++)
		{
			for (size_t col = 0; col < stageMap.at(row).size(); col++)
			{
				// レールIDと先端レールID以外は無視
				eStageID id = STAGE_ID(stageMap.at(row).at(col));
				if (!GetBetween(id, eStageID::Rail, eStageID::GoalRail)) continue;

				// インスタンス描画を追加
				AddInstanceRail(row, col);

				// 先端レールならガイドIDを設定
				if (id == eStageID::DeRail) SetGuideID(row, col);

				// ゴールレールなら
				if (id == eStageID::GoalRail)
				{
					// 駅が存在しなければ生成
					auto station = stagePtr->GetSharedGameObject<Station>(L"Station", false);
					if (!station)
					{
						station = stagePtr->AddGameObject<Station>(ROWCOL2POS(row - 2, col));
						stagePtr->SetSharedGameObject(L"Station", station);
					}
				}

				// レール、または先端レールなら
				if (OR(id, eStageID::Rail, eStageID::DeRail))
				{
					AddRailDataMap(row, col);
					m_pastLine = ROWCOL2LINE(row, col);
				}
			}
		}
	}

	// レールの追加
	void RailManager::AddRail(const Point2D<size_t>& point)
	{
		// ステージcsvの取得
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();

		// インスタンス描画を追加
		AddInstanceRail(point.x, point.y);

		// レールマップに追加
		AddRailDataMap(point.x, point.y);

		// ゴールレールと繋がったかの確認
		CheckConnectionGoalRail(point.x, point.y);

		// 保持するデータを変更
		m_railNum++;
		m_pastLine = ROWCOL2LINE(point.x, point.y);

		// csvの書き換え
		stageMap.at(point.x).at(point.y) = UnSTAGE_ID(eStageID::DeRail);
		m_guideMap = stageMap;
		SetGuideID(point.x, point.y);
		SetRailID(point.x, point.y);
	}

	// インスタンス描画の追加
	void RailManager::AddInstanceRail(size_t row, size_t col)
	{
		// 座標の設定
		Vec3 addPos = ROWCOL2POS(row, col);

		// トランスフォーム行列の設定
		Mat4x4 matrix, mtxPosition;
		mtxPosition.translation(addPos);

		// 行列の設定と追加
		matrix = mtxScale * mtxRotation * mtxPosition;
		m_ptrDraw->AddMatrix(matrix);
	}

	// マップデータに追加
	void RailManager::AddRailDataMap(size_t row, size_t col)
	{
		// 座標の設定
		Vec3 addPos = ROWCOL2POS(row, col);

		// レールデータの設定
		RailData data = {};

		// 1つ前のレールデータ(無ければ空白)
		RailData* pastData = (m_railDataMap.find(m_pastLine) != m_railDataMap.end()) ? &m_railDataMap.at(m_pastLine) : &RailData();
		data.thisPos = addPos; // 自身の座標
		if (pastData->thisPos == Vec3(0.0f)) pastData->thisPos = addPos; // 一個前のデータの座標が空なら
		data.pastPos = pastData->thisPos; // 1つ前の座標か座標が0.0fで初期値なら自身の座標
		pastData->futurePos = addPos; // 1つ前のデータに自身の座標を登録

		bool isLineX = (addPos.z == pastData->thisPos.z);	// X軸で直線か
		bool isLineZ = (addPos.x == pastData->thisPos.x);	// Z軸で直線か
		bool isRight = (addPos.x > pastData->thisPos.x);	// 右に移動するのか
		bool isUpper = (addPos.z > pastData->thisPos.z);	// 上に移動するのか

		// 直線か、そうじゃなければ1つ前のに
		data.type = isLineX ? eRailType::AxisXLine : isLineZ ? eRailType::AxisZLine : pastData->type;

		// 1つ前のレールとタイプが違ったら向きを設定
		if (data.type != pastData->type)
		{
			pastData->angle = isRight ? (isUpper ? eRailAngle::Right : eRailAngle::Left) : (isUpper ? eRailAngle::Left : eRailAngle::Right);
			pastData->type = isRight ? (isUpper ? eRailType::Right2Under : eRailType::Right2Upper) : (isUpper ? eRailType::Left2Upper : eRailType::Left2Under);
		}

		// マップに追加
		m_railDataMap.emplace(ROWCOL2LINE(row, col), data);
	}

	// 先端レールの書き換え
	void RailManager::SetRailID(size_t row, size_t col) const
	{
		// csvの取得
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		
		// 要素数が範囲内で、先端レールなら通常のレールにする
		for (const auto& elem : CSVElementCheck::GetElemsCheck(row, col, stageMap))
		{
			// 配列の範囲外なら無視
			if (!elem.isRange) continue;

			// 書き換え
			int& num = stageMap.at(elem.row).at(elem.col);
			num = num == UnSTAGE_ID(eStageID::DeRail) ? UnSTAGE_ID(eStageID::Rail) : num;
		}
	}

	// ガイドの追加
	void RailManager::SetGuideID(size_t row, size_t col)
	{
		m_guidePoints.clear(); // 初期化
		m_pastDeRailPos = ROWCOL2POS(row, col);

		// エラーチェック
		if (m_railDataMap.find(m_pastLine) == m_railDataMap.end()) return;

		// レールデータの取得
		RailData data = m_railDataMap.at(m_pastLine);
		RailData past = m_railDataMap.at(POS2LINE(data.pastPos));

		// ガイド付きcsvマップから設置位置の上下左右を取得
		auto& elems = CSVElementCheck::GetElemsCheck(row, col, m_guideMap);
		for (auto& elem : elems)
		{
			// 配列の範囲外なら無視
			if (!elem.isRange) continue;

			// 1つ前のレールが直線じゃなければ
			if (past.angle != eRailAngle::Straight)
			{
				// 設置したレールが直線なら
				if (CheckStraightRail(data, elem.dir))
				{
					AddGuideID(elem.row, elem.col);
				}

				continue;
			}

			// 1つ前のレールが直線なら上下左右にガイドを追加
			AddGuideID(elem.row, elem.col);
		}
	}

	// ガイドの追加
	void RailManager::AddGuideID(size_t row, size_t col)
	{
		// ガイド付きcsvから書き換え番号を取得
		int& num = m_guideMap.at(row).at(col);
		int guideID = UnSTAGE_ID(eStageID::GuideRail);

		// 何も無し、またはガイドなら
		if (num == 0 || num == guideID)
		{
			num = guideID; // ガイドに書き換え

			// ガイドの列と行の番号を保持
			m_guidePoints.push_back(Point2D<size_t>(row, col));
		}
	}

	// 指定の座標にガイドがあるか
	bool RailManager::GetIsGuidePoint(const Point2D<size_t>& point) const
	{
		// 配列の範囲内かのエラーチェック
		if (!WithInElemRange(point.x, point.y, m_guideMap)) return false;

		// ガイドIDかどうか
		return eStageID::GuideRail == STAGE_ID(m_guideMap.at(point.x).at(point.y));
	}

	// ガイドの再計算処理
	void RailManager::GuideRecalculation()
	{
		// ステージcsvの取得
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		m_guideMap = stageMap;

		// サイズと列と行
		Vec3 pos = m_pastDeRailPos;
		size_t row, col;
		row = ROW(pos.z);
		col = COL(pos.x);

		// ガイドの再設定
		SetGuideID(row, col);
	}

	// レールが直線に並んでいるか
	bool RailManager::CheckStraightRail(RailData data, eNextElemDir dir)
	{
		return (data.type == eRailType::AxisZLine && OR(dir, eNextElemDir::DirFlont, eNextElemDir::DirBack))
			|| (data.type == eRailType::AxisXLine && OR(dir, eNextElemDir::DirLeft, eNextElemDir::DirRight));
	}

	// レールがゴールレールと繋がったか
	void RailManager::CheckConnectionGoalRail(size_t row, size_t col)
	{
		// レールデータの取得
		RailData data = m_railDataMap.at(ROWCOL2LINE(row, col));

		// ガイド付きcsvマップから設置位置の上下左右を取得
		auto& elems = CSVElementCheck::GetElemsCheck(row, col, m_guideMap);
		for (auto& elem : elems)
		{
			if (!elem.isRange) continue;
			if (eStageID::GoalRail != STAGE_ID(m_guideMap.at(elem.row).at(elem.col))) continue;

			// 設置したレールが直線なら
			if (CheckStraightRail(data, elem.dir))
			{
				AddRailDataMap(elem.row, elem.col);
				m_isConnectionGoal = true;
				return;
			}
		}
	}
}