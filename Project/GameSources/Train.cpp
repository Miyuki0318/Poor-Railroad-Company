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

		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailPositions();
		m_movePos.first = m_DefaultPosition;
		m_movePos.second = railMap.begin()->second;
		m_isRailNum = railMap.begin()->first;

		// �^�O�̐ݒ�
		AddTag(L"Train");
	}

	void Train::OnUpdate()
	{
		Debug::Log(L"Start", m_movePos.first);
		Debug::Log(L"End", m_movePos.second);

		StateProcess(m_state);
		m_beforeState = m_state;
	}

	void Train::OnCollisionEnter(shared_ptr<GameObject>& gameObject)
	{
		if (gameObject->FindTag(L"Tarminal"))
		{
			m_state = State::Derail;
		}
		else if (gameObject->FindTag(L"GoalRail"))
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
			//m_position += m_moveDirection * DELTA_TIME * m_MoveSpeed;
			//SetPosition(m_position);
			OnRailState();
		}
	}

	void Train::OnRailState()
	{
		// ���`��Ԃňړ�
		Vec3 pos = Utility::Lerp(m_movePos.first, m_movePos.second, m_moveRatio);
		m_moveRatio += DELTA_TIME / m_MoveSpeed;

		// ������1�ȏ�ɂȂ�����0�ŏ�����
		if (m_moveRatio >= 1.0f)
		{
			m_moveRatio = 0.0f;

			// ���[���}�b�v�̐擪���R�s�[���č폜���A�V�����擪�����̃|�C���g�Ɋ��蓖�Ă�
			auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailPositions();
			if (railMap.empty()) return;
			m_movePos.first = railMap.at(m_isRailNum);
			railMap.erase(railMap.begin());

			// ���̃��[����������ΒE������
			if (railMap.empty())
			{
				m_state = State::Derail;
				return;
			}

			m_movePos.second = railMap.at(++m_isRailNum);
		}

		// ���W�̍X�V
		SetPosition(pos);
	}
}