/*!
@file Tarminal.cpp
@brief ���H�I���̎���
@author ��I�t
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void Tarminal::OnCreate()
	{
		TemplateObject::OnCreate();
		SetTransParam();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_GRAY);

		// �R���W����OBB�̒ǉ�
		m_ptrColl = AddComponent<CollisionObb>();
		m_ptrColl->SetFixed(true);

		// �^�O�̐ݒ�
		AddTag(L"Tarminal");
	}

	void Tarminal::OnUpdate()
	{

	}
}