/*!
@file GameTrain.cpp
@brief ��Ԃ̎���
@author ��I�t
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
		// ���[���}�b�v�̎擾
		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();
		if (railMap.empty()) return false;

		// ���̃��[���̃A���O�����擾
		Vec3 checkPos = railMap.find(m_railPos) != railMap.end() ? railMap.at(m_railPos).futurePos : railMap.at(m_railPos).thisPos;
		string toNextLine = POS2LINE(checkPos);
		if (railMap.find(toNextLine) != railMap.end())
		{
			eRailAngle nextRailAngle = railMap.at(toNextLine).angle;

			if (NextRailSettings(railMap, nextRailAngle))
			{
				return true;
			}
		}

		return CheckGoalRail(); // ���[����ݒ�ł��Ȃ�������S�[�����ǂ����m�F����
	}

	bool GameTrain::NextRailSettings(const map<string, RailData>& railMap, eRailAngle nextAngle)
	{
		// 1��̃��[���f�[�^������Ȃ�
		string line = POS2LINE(railMap.at(m_railPos).futurePos);	// 1�惌�[����LINE
		if (railMap.find(line) != railMap.end())
		{
			// �����Ȃ�1��A�Ȃ�Ȃ�2��̍��W��ݒ肷��
			if (nextAngle != eRailAngle::Straight)
			{
				// ���[����ݒ�
				m_movePos.first = railMap.at(m_railPos).thisPos;	// ���݂̃��[���̍��W
				m_movePos.second = railMap.at(line).futurePos;		// 2��̃��[���̍��W
				m_railPos = POS2LINE(railMap.at(line).futurePos);	// 2��̃��[����LINE��ێ�
			}
			else
			{
				// ���[����ݒ�
				m_movePos.first = railMap.at(m_railPos).thisPos;	// ���݂̃��[���̍��W
				m_movePos.second = railMap.at(line).thisPos;		// 1��̃��[���̍��W
				m_railPos = line;									// 1��̃��[����LINE��ێ�
			}
			return true;
		}

		return false;
	}

	// �S�[���ɂ��ǂ蒅�������̃`�F�b�N
	bool GameTrain::CheckGoalRail()
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();
		if (railMap.empty()) return false;

		// Line�����񂩂�row��col�𒊏o
		size_t row, col;
		GetLineStringToRowCol(row, col, m_railPos);

		// �S�[���p���[���ƈ�v���Ă���true�A����ȊO�Ȃ�false
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