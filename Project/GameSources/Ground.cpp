/*!
@file Ground.cpp
@brief ���I�u�W�F�N�g
@author	���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void GroundBox::OnCreate()
	{
		// �p�����̐������̏��������s
		TemplateObject::OnCreate();
		SetTransParam();

		// ���b�V���̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_GREAN);

		// �R���W�����R���|�[�l���g��ǉ�
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true); // ���̏������A�N�e�B�u
	}
}