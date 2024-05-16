/*!
@file SkyBox.cpp
@brief �X�J�C�{�b�N�X�̎���
@author ��I�t
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void SkyBox::OnCreate()
	{
		TemplateObject::OnCreate();
		SetTransParam();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<BcPNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"SKY");
		m_ptrDraw->SetTextureResource(L"SKY_TX");
		m_ptrDraw->SetDiffuse(COL_WHITE);
		m_ptrDraw->SetLightingEnabled(false);

		// �^�O�̐ݒ�
		AddTag(L"SkyBox");
	}

	void SkyBox::OnUpdate()
	{
		SetPosition(m_camera->GetEye());
	}
}