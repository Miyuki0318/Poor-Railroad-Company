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
		//m_ptrColl->SetFixed(true);

		// �^�O�̐ݒ�
		AddTag(L"Train");
	}

	void Train::OnUpdate()
	{
		Move(m_state);
		m_beforeState = m_state;

		//Vec3 pos = GetPosition();
		//pos.y = m_DefaultPosition.y;

		//SetPosition(pos);

		Debug::Log(L"�g���C���̍��W : ", GetPosition());
	}

	void Train::OnCollisionEnter(shared_ptr<GameObject>& gameObject)
	{
		if (gameObject->FindTag(L"Tarminal"))
		{
			m_state = State::Derail;
		}
	}

	void Train::Move(State state)
	{
		if (state == State::Onrail)
		{
			m_position.x += DELTA_TIME * m_MoveSpeed;
			SetPosition(m_position);
		}
		else if (state == State::Derail && m_state != m_beforeState)
		{
			GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::GameOver);
		}
	}
}