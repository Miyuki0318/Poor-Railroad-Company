/*!
@file GameTrainStraightState.cpp
@brief ゲーム中の列車の直線走行中のステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "GameTrainStraightState.h"
#include "MathFunc.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<GameTrainStraightState> GameTrainStraightState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<GameTrainStraightState> instance(new GameTrainStraightState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring GameTrainStraightState::GetStateName() const
	{
		return L"GameTrainStraightState";
	}

	// ステート開始時の処理
	void GameTrainStraightState::Enter(const shared_ptr<GameTrain>& train)
	{
		// レールマップの取得
		const auto& railMap = train->GetRailDataMap();
		if (railMap.empty()) return;

		// レールを設定
		string line = POS2LINE(railMap.at(train->m_railPos).futurePos);	// 1個先レールのLINE
		train->m_movePos.first = railMap.at(train->m_railPos).thisPos;	// 現在のレールの座標
		train->m_movePos.second = railMap.at(line).thisPos;				// 1個先のレールの座標
		train->m_railPos = line;										// 1個先のレールのLINEを保持

		// 割合の初期化
		train->m_moveRatio = 0.0f;

		// ローテーションの更新
		float rad = -atan2f(train->m_movePos.second.z - train->m_movePos.first.z, train->m_movePos.second.x - train->m_movePos.first.x);
		train->SetRotation(Vec3(0.0f, rad, 0.0f));
	}

	// ステート更新時の処理
	void GameTrainStraightState::Execute(const shared_ptr<GameTrain>& train)
	{
		// 線形補間で移動
		Vec3 pos = Utility::Lerp(train->m_movePos.first, train->m_movePos.second, train->m_moveRatio);
		train->m_moveRatio = MathF::Repeat01(train->m_moveRatio, train->m_moveSpeed * train->m_acsel);

		// 範囲外になったら
		if (MathF::GetOutRange())
		{
			// 次のレールを見つけられなかったら次のステートに
			if (!train->SearchNextRail()) train->m_state = Train::State::Derail;

			train->SetNextRailDirection(); // 次のレールの方向を設定
		}

		// 座標の更新
		train->SetPosition(pos);
	}

	// ステート終了時の処理
	void GameTrainStraightState::Exit(const shared_ptr<GameTrain>& train)
	{
		// 今のところ何もしない
	}
}