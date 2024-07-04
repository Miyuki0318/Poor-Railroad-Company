/*!
@file GameTrainCurvingState.cpp
@brief ゲーム中の列車のカーブ中のステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "GameTrainCurvingState.h"
#include "GameTrainCurveExitState.h"
#include "MathFunc.h"

namespace basecross
{
	const float HELF_TIME = 0.5f;	// 半分の時間

	// インスタンス生成
	shared_ptr<GameTrainCurvingState> GameTrainCurvingState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<GameTrainCurvingState> instance(new GameTrainCurvingState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring GameTrainCurvingState::GetStateName() const
	{
		return L"GameTrainCurvingState";
	}

	// ステート開始時の処理
	void GameTrainCurvingState::Enter(const shared_ptr<GameTrain>& train)
	{
		// レールマップの取得
		const auto& railMap = train->GetRailDataMap();
		if (railMap.empty()) return;

		// 現在のレール、カーブするレール、終了時のレールの座標
		Vec3 pointA = railMap.at(train->m_railPos).thisPos;
		Vec3 pointB = railMap.at(POS2LINE(railMap.at(train->m_railPos).futurePos)).thisPos;
		Vec3 pointC = railMap.at(POS2LINE(railMap.at(train->m_railPos).futurePos)).futurePos;

		// 中間の座標
		Vec3 pointA2B = Utility::Lerp(pointA, pointB, HELF_TIME);
		Vec3 pointB2C = Utility::Lerp(pointB, pointC, HELF_TIME);

		// カーブ座標の設定と割合の初期化
		m_curvePoints = CurvePoints(pointA2B, pointB, pointB2C);
		train->m_moveRatio = 0.0f;

		// ローテーションY軸を保持
		m_pastRotY = train->GetRotation().y;
	}

	// ステート更新時の処理
	void GameTrainCurvingState::Execute(const shared_ptr<GameTrain>& train)
	{
		// カーブを線形補間で処理
		Vec3 pos = Utility::BezierLerp(m_curvePoints.pointA, m_curvePoints.pointB, m_curvePoints.pointC, train->m_moveRatio);

		// レールマップの取得
		const auto& railMap = train->GetRailDataMap();
		if (railMap.empty()) return;

		// ローテーションを線形補間で処理
		auto angle = railMap.at(POS2LINE(railMap.at(train->m_railPos).futurePos)).angle;
		float endRad = m_pastRotY + (angle == eRailAngle::Right ? XM_PIDIV2 : -XM_PIDIV2);
		float rad = Utility::Lerp(m_pastRotY, endRad, train->m_moveRatio);

		// 割合の加算
		train->m_moveRatio = MathF::Repeat01(train->m_moveRatio, train->m_moveSpeed * train->m_acsel);

		// 範囲外になったら
		if (MathF::GetOutRange())
		{
			// カーブ終了時のステートに切り替え
			train->m_trainState->ChangeState(GameTrainCurveExitState::Instance());
		}

		// 座標とローテーションの更新
		train->SetPosition(pos);
		train->SetRotation(Vec3(0.0f, rad, 0.0f));
	}

	// ステート終了時の処理
	void GameTrainCurvingState::Exit(const shared_ptr<GameTrain>& train)
	{
		// 今のところ何もしない
	}
}