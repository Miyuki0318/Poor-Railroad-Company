/*!
@file Train.cpp
@brief ��Ԃ̎���
@author ��I�t
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void Train::OnCreate()
	{
		TemplateObject::OnCreate();
		SetPosition(m_DefaultPosition);
		SetScale(m_DefaultScale);

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_BLUE);

		// �R���W����OBB�̒ǉ�
		m_ptrColl = AddComponent<CollisionObb>();

		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailMap();
		m_railPos = railMap.begin()->first;
		m_movePos.first = m_DefaultPosition;
		m_movePos.second = railMap.begin()->second;

		// �^�O�̐ݒ�
		AddTag(L"Train");
	}

	void Train::OnUpdate()
	{
		StateProcess(m_state);
		m_beforeState = m_state;
	}

	void Train::OnCollisionEnter(shared_ptr<GameObject>& gameObject)
	{
		if (gameObject->FindTag(L"GoalRail"))
		{
			m_state = State::Arrival;
		}
	}

	void Train::StateProcess(State state)
	{
		if (state == State::Arrival) return;

		if (state == State::Derail)
		{
			GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::GameOver);
			return;
		}

		if (state == State::Onrail)
		{
			OnRailState();
		}
	}

	void Train::OnRailState()
	{
		// ���`��Ԃňړ�
		Vec3 pos = Utility::Lerp(m_movePos.first, m_movePos.second, m_moveRatio);
		m_moveRatio += DELTA_TIME / m_MoveInSeconds;

		// ������1�ȏ�ɂȂ�����0�ŏ�����
		if (m_moveRatio >= 1.0f)
		{
			m_moveRatio = 0.0f;

			// ���̃��[����ݒ肷��A�ݒ�s�Ȃ�E���X�e�[�g
			if (!SetNextRail()) m_state = State::Derail;

			SetDirection();
		}

		// ���W�̍X�V
		SetPosition(pos);
	}

	bool Train::SetNextRail()
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

		return false;
	}

	void Train::SetDirection()
	{
		// �n�_�ƏI�_�̃��[�����烉�W�A���p�����߂�
		float rad = atan2f(m_movePos.first.z - m_movePos.second.z, m_movePos.first.x - m_movePos.second.x);
		if (m_radMap.find(rad) != m_radMap.end())
		{
			// ���W�A���p�������enum��ݒ�
			m_direction = m_radMap.at(rad);
		}
	}
}