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

		if (state == State::Onrail)
		{
			OnRailProcess();
		}
	}

	void GameTrain::OnRailProcess()
	{
		// ���`��Ԃňړ�
		Vec3 pos = Utility::Lerp(m_movePos.first, m_movePos.second, m_moveRatio);
		float rad = -atan2f(m_movePos.second.z - m_movePos.first.z, m_movePos.second.x - m_movePos.first.x);
		//m_moveRatio += DELTA_TIME * m_MoveSpeed;
		m_moveRatio = MathF::Repeat01(m_moveRatio, m_MoveSpeed, false).value;

		// ������1�ȏ�ɂȂ�����0�ŏ�����
		if (MathF::Repeat01(m_moveRatio, m_MoveSpeed, false).outRange)
		{
			// ���̃��[����ݒ肷��A�ݒ�s�Ȃ�E���X�e�[�g
			if (!SearchNextRail()) m_state = State::Derail;

			SetNextRailDirection();
		}

		// ���W�̍X�V
		SetPosition(pos);
		SetRotation(Vec3(0.0f, rad, 0.0f));
	}

	bool GameTrain::SearchNextRail()
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailMap();
		if (railMap.empty()) return false;

		// �n�_�ƏI�_�̐ݒ�A�I�_�������ꍇ��false��Ԃ�
		const auto& drs = m_drMap.at(m_direction);
		for (const auto& dr : drs)
		{
			// �s�Ɨ�
			size_t row, col;
			row = ROW(m_movePos.second.z + dr.y);
			col = COL(m_movePos.second.x + dr.x);

			// �s��ŃL�[��ݒ�
			string line = LINE(row, col);
			if (railMap.find(line) != railMap.end())
			{
				// ���[����ݒ�
				m_movePos.first = railMap.at(m_railPos);
				m_movePos.second = railMap.at(line);
				m_railPos = line;

				return true;
			}
		}

		return CheckGoalRail(); // ���[����ݒ�ł��Ȃ�������S�[�����ǂ����m�F����
	}

	// �S�[���ɂ��ǂ蒅�������̃`�F�b�N
	bool GameTrain::CheckGoalRail()
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailMap();
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
				m_movePos.first = railMap.at(m_railPos);
				m_movePos.second = railMap.at(m_railPos);
				m_state = State::Arrival;
				return true;
			}
		}

		return false;
	}
}