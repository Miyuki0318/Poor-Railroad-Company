/*!
@file GameTrainCurveStandbyState.cpp
@brief ゲーム中の列車のカーブ開始時のステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	const float HELF_TIME = 0.5f;	// 半分の時間
	const float ACSEL = 0.75f;	// 半分の時間

	// インスタンス生成
	shared_ptr<GameTrainCurveStandbyState> GameTrainCurveStandbyState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<GameTrainCurveStandbyState> instance(new GameTrainCurveStandbyState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring GameTrainCurveStandbyState::GetStateName() const
	{
		return L"GameTrainCurveStandbyState";
	}

	// ステート開始時の処理
	void GameTrainCurveStandbyState::Enter(const shared_ptr<GameTrain>& train)
	{	
		// レールマップの取得
		const auto& railMap = train->GetRailDataMap();
		if (railMap.empty()) return;

		// 現在のレール、カーブ中のレールの座標
		Vec3 pointA = railMap.at(train->m_railPos).thisPos;
		Vec3 pointB = railMap.at(train->m_railPos).futurePos;

		// 開始時のレールの座標と中間の座標を設定
		train->m_movePos.first = pointA;
		train->m_movePos.second = Utility::Lerp(pointA, pointB, HELF_TIME);

		// 割合の初期化
		train->m_moveRatio = 0.0f;
	}

	// ステート更新時の処理
	void GameTrainCurveStandbyState::Execute(const shared_ptr<GameTrain>& train)
	{
		// 加速度を1.0から0.5に下げる
		train->m_acsel = Utility::Lerp(1.0f, ACSEL, train->m_moveRatio);

		// カーブを線形補間で処理
		Vec3 pos = Utility::Lerp(train->m_movePos.first, train->m_movePos.second, train->m_moveRatio);
		train->m_moveRatio = MathF::Repeat01(train->m_moveRatio, train->m_moveSpeed * train->m_acsel / ACSEL);

		// 範囲外になったら
		if (MathF::GetOutRange())
		{
			// カーブ中のステートに切り替える
			train->m_trainState->ChangeState(GameTrainCurvingState::Instance());
		}

		// 座標の更新
		train->SetPosition(pos);
	}

	// ステート終了時の処理
	void GameTrainCurveStandbyState::Exit(const shared_ptr<GameTrain>& train)
	{
		train->m_acsel = ACSEL;
	}
}