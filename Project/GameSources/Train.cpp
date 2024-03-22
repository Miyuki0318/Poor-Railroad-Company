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
		SetTransParam();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_BLUE);

		// �R���W����OBB�̒ǉ�
		m_ptrColl = AddComponent<CollisionObb>();

		// �^�O�̐ݒ�
		AddTag(L"Train");
	}

	void Train::OnUpdate()
	{

	}

	void Train::OnCollisionExcute(shared_ptr<GameObject>& gameObject)
	{
		if (gameObject->FindTag(L"Rail"))
		{
			m_state = State::Onrail;
		}
		else
		{
			m_state = State::Derail;
		}
	}

	void Train::Move(State state)
	{

	}
}