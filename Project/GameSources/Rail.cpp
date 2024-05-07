/*!
@file Rail.cpp
@brief ���H�̎���
@author ��I�t
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void Rail::OnCreate()
	{
		TemplateObject::OnCreate();
		SetTransParam();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_GRAY);

		// �R���W����OBB�̒ǉ�
		m_ptrColl = AddComponent<CollisionObb>();
		m_ptrColl->SetAfterCollision(AfterCollision::None);



		// �^�O�̐ݒ�
		AddTag(L"Rail");
	}

	void Rail::OnUpdate()
	{

	}
}