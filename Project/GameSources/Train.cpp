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
		m_isRailNum = railMap.begin()->first;
		m_movePos.first = m_DefaultPosition;
		m_movePos.second = railMap.begin()->second;

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

			// ���̃��[����ݒ肷��A�ݒ�s�Ȃ�E���X�e�[�g
			if (!SetNextRail()) m_state = State::Derail;
		}

		// ���W�̍X�V
		SetPosition(pos);
	}

	bool Train::SetNextRail()
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailPositions();
		if (railMap.empty()) return false;

		// �n�_�ƏI�_�̐ݒ�A�I�_�������ꍇ��false��Ԃ�
		m_movePos.first = railMap.at(m_isRailNum++);
		if (railMap.find(m_isRailNum) != railMap.end())
		{
			m_movePos.second = railMap.at(m_isRailNum);
			return true;
		}

		return false;
	}
}