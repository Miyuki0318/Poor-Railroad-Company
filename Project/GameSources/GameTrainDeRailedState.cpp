/*!
@file GameTrainDeRailedState.h
@brief ゲーム中の列車の脱線演出のステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	const float HELF_TIME = 0.5f;	// 半分の時間

	// インスタンス生成
	shared_ptr<GameTrainDeRailedState> GameTrainDeRailedState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<GameTrainDeRailedState> instance(new GameTrainDeRailedState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring GameTrainDeRailedState::GetStateName() const
	{
		return L"GameTrainDeRailedState";
	}

	// ステート開始時の処理
	void GameTrainDeRailedState::Enter(const shared_ptr<GameTrain>& train)
	{
		// レールマップの取得
		const auto& railMap = train->GetRailDataMap();
		if (railMap.empty()) return;

		// レールマネージャーの取得
		const auto& railManager = train->m_railManager.lock();
		if (!railManager) return;

		// レールを設定
		string pastLine = POS2LINE(railManager->GetPastRailPos());
		Vec3 pointA = railMap.at(pastLine).thisPos;
		Vec3 pastPos = railMap.at(pastLine).pastPos;
		float rot = -atan2f(pointA.z - pastPos.z, pointA.x - pastPos.x);
		Vec3 pointB = pointA + Vec3(cos(rot), 0.0f, sin(-rot));
		Vec3 pointC = pointB + (Vec3(cos(rot + XM_PIDIV2), 0.0f, sin(rot + XM_PIDIV2)) * Utility::RangeRand(0.5f, -0.5f));
		Vec3 pointA2B = Utility::Lerp(pointA, pointB, HELF_TIME);
		
		// カーブ座標の設定と割合の初期化
		m_curvePoints = CurvePoints(pointA, pointA2B, pointC);
		train->m_moveRatio = 0.0f;

		// ローテーションY軸を保持
		m_pastRotY = train->GetRotation().y;
	}

	// ステート更新時の処理
	void GameTrainDeRailedState::Execute(const shared_ptr<GameTrain>& train)
	{
		// カーブを線形補間で処理
		Vec3 pos = Utility::BezierLerp(m_curvePoints.pointA, m_curvePoints.pointB, m_curvePoints.pointC, train->m_moveRatio);

		// 回転の設定
		float endRad = -atan2f(m_curvePoints.pointC.z - m_curvePoints.pointA.z, m_curvePoints.pointC.x - m_curvePoints.pointA.x);
		float rad = Utility::Lerp(m_pastRotY, endRad, train->m_moveRatio);

		// 割合の加算
		train->m_moveRatio = MathF::Repeat01(train->m_moveRatio, train->m_moveSpeed * train->m_acsel);

		// 範囲外になったら
		if (MathF::GetOutRange())
		{
			// 待機状態のステートに切り替え
			train->m_state = Train::State::StandBy;
			train->GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::GameOver);
		}

		// 座標とローテーションの更新
		train->SetPosition(pos);
		train->SetRotation(Vec3(0.0f, rad, 0.0f));
	}

	// ステート終了時の処理
	void GameTrainDeRailedState::Exit(const shared_ptr<GameTrain>& train)
	{
		// 今のところ何もしない
	}
}