/*!
@file GameTrain.cpp
@brief ��Ԃ̎���
@author ��I�t
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	const float HELF_TIME = 0.5f;

	void GameTrain::OnCreate()
	{
		Train::OnCreate();

		m_railDataMap = &GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();

		m_trainState.reset(new StateMachine<GameTrain>(GetThis<GameTrain>()));
		m_trainState->ChangeState(GameTrainStraightState::Instance());
	}

	void GameTrain::OnUpdate()
	{
		StateProcess(m_state);

		Debug::Log(L"���݂̃X�e�[�g : ", m_trainState->GetCurrentState()->GetStateName());
	}

	void GameTrain::OnCollisionEnter(shared_ptr<GameObject>& gameObject)
	{
		if (gameObject->FindTag(L"GoalRail"))
		{
			m_state = State::Arrival;
		}
	}

	const map<string, RailData>& GameTrain::GetRailDataMap() const
	{
		return *m_railDataMap;
	}

	void GameTrain::StateProcess(State state)
	{
		const auto& stagePtr = GetTypeStage<GameStage>();


		if (state == State::Arrival)
		{
			stagePtr->SetGameProgress(eGameProgress::GameClear);
			stagePtr->GetSharedGameObject<GamePlayer>(L"Player")->SetGameResult(eGameProgress::GameClear);
			return;
		}

		if (state == State::Derail)
		{
			stagePtr->SetGameProgress(eGameProgress::GameOver);
			stagePtr->GetSharedGameObject<GamePlayer>(L"Player")->SetGameResult(eGameProgress::GameOver);
			return;
		}

		if (state == State::OnRail)
		{
			m_trainState->Update();
		}
	}

	void GameTrain::OnRailProcess()
	{
		//MoveProcess(State::Derail);

		float rad = -atan2f(m_movePos.second.z - m_movePos.first.z, m_movePos.second.x - m_movePos.first.x);
		SetRotation(Vec3(0.0f, rad, 0.0f));
	}

	bool GameTrain::SearchNextRail()
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = GetRailDataMap();
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
				m_trainState->ChangeState(GameTrainCurveStandbyState::Instance());
			}
			else
			{
				m_trainState->ChangeState(GameTrainStraightState::Instance());
			}
			return true;
		}

		return false;
	}

	// �S�[���ɂ��ǂ蒅�������̃`�F�b�N
	bool GameTrain::CheckGoalRail()
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = GetRailDataMap();
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