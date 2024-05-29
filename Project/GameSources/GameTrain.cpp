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

		if (state == State::OnRail)
		{
			OnRailProcess();
		}
	}

	void GameTrain::OnRailProcess()
	{
		MoveProcess(State::Derail);

		float rad = -atan2f(m_movePos.second.z - m_movePos.first.z, m_movePos.second.x - m_movePos.first.x);
		SetRotation(Vec3(0.0f, rad, 0.0f));
	}

	bool GameTrain::SearchNextRail()
	{
		// レールマップの取得
		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();
		if (railMap.empty()) return false;

		eRailAngle nextRailAngle = railMap.at(POS2LINE(railMap.at(m_railPos).futurePos)).angle;
		if (nextRailAngle != eRailAngle::Straight)
		{
			Debug::Log(L"うんち！");
		}

		if (NextRailSettings(railMap, nextRailAngle))
		{
			return true;
		}
		return CheckGoalRail(); // レールを設定できなかったらゴールかどうか確認する
	}

	bool GameTrain::NextRailSettings(const map<string, RailData>& railMap, eRailAngle nextAngle)
	{
		// 始点と終点の設定、終点が無い場合はfalseを返す
		const auto& drs = m_drMap.at(m_direction);
		for (const auto& dr : drs)
		{
			// 行と列
			size_t row, col;
			row = ROW(m_movePos.second.z + dr.y);
			col = COL(m_movePos.second.x + dr.x);

			// 行列でキーを設定
			string line = ROWCOL2LINE(row, col);
			if (railMap.find(line) != railMap.end())
			{
				// レールを設定
				m_movePos.first = railMap.at(m_railPos).thisPos;
				m_movePos.second = railMap.at(line).thisPos;
				m_railPos = line;

				return true;
			}
		}

		return false;
	}

	// ゴールにたどり着いたかのチェック
	bool GameTrain::CheckGoalRail()
	{
		// レールマップの取得
		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();
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
				m_movePos.first = railMap.at(m_railPos).thisPos;
				m_movePos.second = railMap.at(m_railPos).thisPos;
				m_state = State::Arrival;
				return true;
			}
		}

		return false;
	}
}