/*!
@file RailManager.cpp
@brief レール管理クラス
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

// カーブレールの方向設定用真偽
#define LEFT2UNDER(current, past)  past.pastPos.x < current.thisPos.x && past.pastPos.z > current.thisPos.z && current.type == eRailType::AxisZLine	// 左から下
#define LEFT2UPPER(current, past)  past.pastPos.x < current.thisPos.x && past.pastPos.z < current.thisPos.z && current.type == eRailType::AxisZLine	// 左から上
#define RIGHT2UNDER(current, past) past.pastPos.x > current.thisPos.x && past.pastPos.z > current.thisPos.z && current.type == eRailType::AxisZLine	// 右から下	
#define RIGHT2UPPER(current, past) past.pastPos.x > current.thisPos.x && past.pastPos.z < current.thisPos.z && current.type == eRailType::AxisZLine	// 右から上
#define UPPER2LEFT(current, past)  past.pastPos.x > current.thisPos.x && past.pastPos.z > current.thisPos.z && current.type == eRailType::AxisXLine	// 上から左
#define UPPER2RIGHT(current, past) past.pastPos.x < current.thisPos.x && past.pastPos.z > current.thisPos.z && current.type == eRailType::AxisXLine	// 上から右
#define UNDER2LEFT(current, past)  past.pastPos.x > current.thisPos.x && past.pastPos.z < current.thisPos.z && current.type == eRailType::AxisXLine	// 下から左
#define UNDER2RIGHT(current, past) past.pastPos.x < current.thisPos.x && past.pastPos.z < current.thisPos.z && current.type == eRailType::AxisXLine	// 下から右

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 行列計算に使うスケール
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
	void InstanceRail::OnCreate()
	{
		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(m_meshKey);
		m_ptrDraw->SetTextureResource(L"RAIL_TX");
		m_ptrDraw->SetSpecular(COL_WHITE);
		m_ptrDraw->SetDiffuse(COL_WHITE);
	}

	// インスタンス描画の行列を追加
	void InstanceRail::AddRail(Mat4x4 matrix)
	{
		if (!m_ptrDraw) return;
		m_ptrDraw->AddMatrix(matrix);
	}

	// インスタンス描画の行列を追加
	void InstanceRail::ResetRail()
	{
		m_ptrDraw->ClearMatrixVec();
	}

	// 行列の配列を取得
	vector<Mat4x4>& InstanceRail::GetMatrix()
	{
		return m_ptrDraw->GetMatrixVec();
	}

	// 生成時の処理
	void RailManager::OnCreate()
	{
		// csvの取得
		const auto& stagePtr = GetTypeStage<BaseStage>();
		const auto& stageMap = stagePtr->GetStageMap();
		m_guideMap = stageMap;

		// インスタンス描画のレールを生成
		m_instanceRail.emplace(eRailAngle::Left, stagePtr->AddGameObject<InstanceRail>(L"TURNRAIL"));
		m_instanceRail.emplace(eRailAngle::Straight, stagePtr->AddGameObject<InstanceRail>(L"RAIL"));

		// レール描画生成
		ResetInstanceRail();
	}

	// 更新処理
	void RailManager::OnUpdate()
	{
		// ステージと列車の取得
		const auto& stagePtr = GetTypeStage<BaseStage>();
		auto& train = stagePtr->GetSharedGameObject<GameTrain>(L"Train", false);
		if (!train) return;

		// 列車の移動先が先端レールじゃなければ無視
		string moveLine = train->GetRailPosLine();
		if (m_pastLine != moveLine) return;
		if (m_railDataMap.find(moveLine) == m_railDataMap.end()) return;
		
		// レールデータを取得しガイドを初期化
		auto& data = m_railDataMap.at(moveLine);
		m_guidePoints.clear();
		m_guideMap = stagePtr->GetStageMap();

		// 配列の要素番号を取得
		size_t row, col;
		GetLineStringToRowCol(row, col, moveLine);

		// ガイド付きcsvマップから設置位置の上下左右を取得
		auto& elems = CSVElementCheck::GetElemsCheck(row, col, m_guideMap);
		for (auto& elem : elems)
		{
			// 配列の範囲外なら無視
			if (!elem.isRange) continue;

			// 上下左右の内で直線ならガイドを追加
			if (CheckStraightRail(data, elem.dir))
			{
				AddGuideID(elem.row, elem.col);
			}
		}
	}

	// インスタンス描画のレールを生成
	void RailManager::ResetInstanceRail()
	{
		// ステージとCSVの取得
		const auto& stagePtr = GetTypeStage<BaseStage>();
		const auto& stageMap = stagePtr->GetStageMap();
		m_guideMap = stageMap;

		// 初期化
		m_railDataMap.clear();

		// 初期化
		for (auto& rail : m_instanceRail)
		{
			rail.second.lock()->ResetRail();
		}

		// 二重ループ
		for (size_t row = 0; row < stageMap.size(); row++)
		{
			for (size_t col = 0; col < stageMap.at(row).size(); col++)
			{
				// レールIDと先端レールID以外は無視
				eStageID id = STAGE_ID(stageMap.at(row).at(col));
				if (!GetBetween(id, eStageID::Rail, eStageID::DrawRail)) continue;

				// 開始レールならガイドIDを設定
				if (id == eStageID::StartRail) m_startRailPos = ROWCOL2POS(row, col);

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
				if (ORS(id, eStageID::Rail, eStageID::DeRail, eStageID::StartRail))
				{
					AddRailDataMap(row, col);
					m_pastLine = ROWCOL2LINE(row, col);
				}

				// インスタンス描画を追加
				AddInstanceRail(row, col);
			}
		}
	}

	// レールの追加
	void RailManager::AddRail(const Point2D<size_t>& point)
	{
		// ステージcsvの取得
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();

		// レールマップに追加
		AddRailDataMap(point.x, point.y);

		// インスタンス描画を追加
		AddInstanceRail(point.x, point.y);

		// ゴールレールと繋がったかの確認
		CheckConnectionGoalRail(point.x, point.y);

		// 保持するデータを変更
		m_railNum++;
		m_pastLine = ROWCOL2LINE(point.x, point.y);

		// csvの書き換え
		stageMap.at(point.x).at(point.y) = UnSTAGE_ID(eStageID::DeRail);
		m_guideMap = stageMap;
		if (m_isConnectionGoal)
		{
			m_guidePoints.clear();
			return;
		}

		SetGuideID(point.x, point.y);
		SetRailID(point.x, point.y);
	}

	// インスタンス描画の追加
	void RailManager::AddInstanceRail(size_t row, size_t col, eRailAngle angle)
	{
		// 座標とLINEの設定
		Vec3 addPos = ROWCOL2POS(row, col);
		string line = ROWCOL2LINE(row, col);

		// トランスフォーム行列の設定
		Mat4x4 matrix, mtxPosition, mtxRotation;
		mtxPosition.translation(addPos);

		// ローテーションの設定
		eRailType type = m_railDataMap.find(line) != m_railDataMap.end() ? m_railDataMap.at(line).type : eRailType::AxisXLine;
		mtxRotation.rotation((Quat)XMQuaternionRotationRollPitchYawFromVector(Vec3(0.0f, m_railAngleMap.at(type), 0.0f)));

		// 行列の設定と追加
		matrix = mtxScale * mtxRotation * mtxPosition;
		m_instanceRail.at(angle).lock()->AddRail(matrix);
	}

	// インスタンス描画のカーブレールを追加
	void RailManager::AddInstanceCurveRail(RailData& pastData)
	{
		// 新しい行列配列
		vector<Mat4x4> newVec;

		// 現在の直線レールの行列配列を取得
		auto& matVec = m_instanceRail.at(eRailAngle::Straight).lock()->GetMatrix();
		for (size_t i = 0; i < matVec.size(); i++)
		{
			// カーブしたレール以外をコピー
			if (matVec.at(i).getTranslation() != pastData.thisPos) newVec.push_back(matVec.at(i));
		}

		// 配列を上書き
		matVec.swap(newVec);

		// カーブレール描画を追加
		AddInstanceRail(ROW(pastData.thisPos.z), COL(pastData.thisPos.x), eRailAngle::Left);
	}

	// マップデータに追加
	void RailManager::AddRailDataMap(size_t row, size_t col)
	{
		// 座標の設定
		Vec3 addPos = ROWCOL2POS(row, col);

		// レールデータの設定
		RailData data = {};
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
			// レールのアングルを設定
			SetPastRailDataAngle(data, *pastData);

			// レールのタイプを設定
			SetPastRailDataType(data, *pastData);

			// カーブレールを追加
			AddInstanceCurveRail(*pastData);
		}

		// マップに追加
		m_railDataMap.emplace(ROWCOL2LINE(row, col), data);
	}

	// カーブに変わったレールのタイプを設定
	void RailManager::SetPastRailDataType(RailData& current, RailData& past)
	{
		// 現在のレールと前々回のレールとの相対座標とレールを設置した向きに応じて前回のレールのタイプを変更する
		if (LEFT2UNDER(current, past))  past.type = eRailType::Left2Under;
		if (LEFT2UPPER(current, past))  past.type = eRailType::Left2Upper;
		if (RIGHT2UNDER(current, past)) past.type = eRailType::Right2Under;
		if (RIGHT2UPPER(current, past)) past.type = eRailType::Right2Upper;
		if (UNDER2LEFT(current, past))  past.type = eRailType::Left2Under;
		if (UNDER2RIGHT(current, past)) past.type = eRailType::Right2Under;
		if (UPPER2LEFT(current, past))  past.type = eRailType::Left2Upper;
		if (UPPER2RIGHT(current, past)) past.type = eRailType::Right2Upper;
	}

	// カーブに変わったレールのアングルを設定
	void RailManager::SetPastRailDataAngle(RailData& current, RailData& past)
	{
		// 現在のレールと前々回のレールとの相対座標とレールを設置した向きに応じて前回のレールのアングルを変更する
		if (LEFT2UNDER(current, past))  past.angle = eRailAngle::Right;
		if (LEFT2UPPER(current, past))  past.angle = eRailAngle::Left;
		if (RIGHT2UNDER(current, past)) past.angle = eRailAngle::Left;
		if (RIGHT2UPPER(current, past)) past.angle = eRailAngle::Right;
		if (UNDER2LEFT(current, past))  past.angle = eRailAngle::Left;
		if (UNDER2RIGHT(current, past)) past.angle = eRailAngle::Right;
		if (UPPER2LEFT(current, past))  past.angle = eRailAngle::Right;
		if (UPPER2RIGHT(current, past)) past.angle = eRailAngle::Left;
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
		string line = ROWCOL2LINE(row, col);
		RailData data = m_railDataMap.at(line);

		// ガイド付きcsvマップから設置位置の上下左右を取得
		auto& elems = CSVElementCheck::GetElemsCheck(row, col, m_guideMap);
		for (auto& elem : elems)
		{
			if (!elem.isRange) continue;
			if (eStageID::GoalRail != STAGE_ID(m_guideMap.at(elem.row).at(elem.col))) continue;

			// 設置したレールが直線なら
			if (CheckStraightRail(data, elem.dir) && data.type == eRailType::AxisXLine)
			{
				m_pastLine = line;
				AddRailDataMap(elem.row, elem.col);
				m_isConnectionGoal = true;
				return;
			}

			if (!CheckStraightRail(data, elem.dir) && data.type == eRailType::AxisZLine)
			{
				m_pastLine = line;
				AddRailDataMap(elem.row, elem.col);
				m_isConnectionGoal = true;
				return;
			}
		}
	}
}