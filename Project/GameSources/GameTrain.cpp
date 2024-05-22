/*!
@file GameTrain.cpp
@brief 列車の実装
@author 矢吹悠葉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void GameTrain::OnUpdate()
	{
		StateProcess(m_state);
	}

	void GameTrain::OnCollisionEnter(shared_ptr<GameObject>& gameObject)
	{
		if (gameObject->FindTag(L"GoalRail"))
		{
			m_state = State::Arrival;
		}
	}

	void GameTrain::StateProcess(State state)
	{
		if (state == State::Arrival)
		{
			GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::GameClear);
			return;
		}

		if (state == State::Derail)
		{
			GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::GameOver);
			return;
		}

		if (state == State::Onrail)
		{
			OnRailProcess();
		}
	}

	void GameTrain::OnRailProcess()
	{
		// 線形補間で移動
		Vec3 pos = Utility::Lerp(m_movePos.first, m_movePos.second, m_moveRatio);
		float rad = -atan2f(m_movePos.second.z - m_movePos.first.z, m_movePos.second.x - m_movePos.first.x);
		//m_moveRatio += DELTA_TIME * m_MoveSpeed;
		m_moveRatio = MathF::Repeat01(m_moveRatio, m_MoveSpeed, false).value;

		// 割合が1以上になったら0で初期化
		if (MathF::Repeat01(m_moveRatio, m_MoveSpeed, false).outRange)
		{
			// 次のレールを設定する、設定不可なら脱線ステート
			if (!SearchNextRail()) m_state = State::Derail;

			SetNextRailDirection();
		}

		// 座標の更新
		SetPosition(pos);
		SetRotation(Vec3(0.0f, rad, 0.0f));
	}

	bool GameTrain::SearchNextRail()
	{
		// レールマップの取得
		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailMap();
		if (railMap.empty()) return false;

		// 始点と終点の設定、終点が無い場合はfalseを返す
		const auto& drs = m_drMap.at(m_direction);
		for (const auto& dr : drs)
		{
			// 行と列
			size_t row, col;
			row = ROW(m_movePos.second.z + dr.y);
			col = COL(m_movePos.second.x + dr.x);

			// 行列でキーを設定
			string line = LINE(row, col);
			if (railMap.find(line) != railMap.end())
			{
				// レールを設定
				m_movePos.first = railMap.at(m_railPos);
				m_movePos.second = railMap.at(line);
				m_railPos = line;

				return true;
			}
		}

		return CheckGoalRail(); // レールを設定できなかったらゴールかどうか確認する
	}

	// ゴールにたどり着いたかのチェック
	bool GameTrain::CheckGoalRail()
	{
		// レールマップの取得
		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailMap();
		if (railMap.empty()) return false;

		// Line文字列からrowとcolを抽出
		size_t row, col;
		GetLineStringToRowCol(row, col, m_railPos);

		// ゴール用レールと一致してたらtrue、それ以外ならfalse
		const auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();
		if (Utility::WithInElemRange(row, row, stageMap))
		{
			if (STAGE_ID(stageMap.at(row).at(col)) == eStageID::GoalRail)
			{
				m_movePos.first = railMap.at(m_railPos);
				m_movePos.second = railMap.at(m_railPos);
				m_state = State::Arrival;
				return true;
			}
		}

		return false;
	}
}