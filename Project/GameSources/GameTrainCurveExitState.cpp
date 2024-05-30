/*!
@file GameTrainCurveExitState.cpp
@brief ゲーム中の列車のカーブ終了時のステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	const float HELF_TIME = 0.5f;	// 半分の時間
	
	// インスタンス生成
	shared_ptr<GameTrainCurveExitState> GameTrainCurveExitState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<GameTrainCurveExitState> instance(new GameTrainCurveExitState);
		
		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring GameTrainCurveExitState::GetStateName() const
	{
		return L"GameTrainCurveExitState";
	}

	// ステート開始時の処理
	void GameTrainCurveExitState::Enter(const shared_ptr<GameTrain>& train)
	{
		// レールマップの取得
		const auto& railMap = train->GetRailDataMap();
		if (railMap.empty()) return;

		// カーブ中のレール、終了時のレールの座標
		Vec3 pointB = railMap.at(POS2LINE(railMap.at(train->m_railPos).futurePos)).thisPos;
		Vec3 pointC = railMap.at(POS2LINE(railMap.at(train->m_railPos).futurePos)).futurePos;
		
		// 中間の座標と終了時のレールの座標を設定
		train->m_movePos.first = Utility::Lerp(pointB, pointC, HELF_TIME);
		train->m_movePos.second = pointC;

		// 割合の初期化
		train->m_moveRatio = 0.0f;
	}

	// ステート更新時の処理
	void GameTrainCurveExitState::Execute(const shared_ptr<GameTrain>& train)
	{
		// カーブを線形補間で処理
		Vec3 pos = Utility::Lerp(train->m_movePos.first, train->m_movePos.second, train->m_moveRatio);
		train->m_moveRatio = MathF::Repeat01(train->m_moveRatio, train->m_MoveSpeed / HELF_TIME);

		// 範囲外になったら
		if (MathF::GetOutRange())
		{
			// 終了時のレールの座標をLINEとして保持
			train->m_railPos = POS2LINE(train->m_movePos.second);

			// 次のレールを見つけられなかったら次のステートに
			if (!train->SearchNextRail()) train->m_state = Train::State::Derail;
		}

		// 座標の更新
		train->SetPosition(pos);
	}

	// ステート終了時の処理
	void GameTrainCurveExitState::Exit(const shared_ptr<GameTrain>& train)
	{
		// 今のところ何もしない
	}
}