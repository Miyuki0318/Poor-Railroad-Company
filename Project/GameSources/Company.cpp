/*!
@file Company.cpp
@brief ��Ђ̎���(���j���[���)
@author �V�Ȉ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Company::OnCreate()
	{
		TemplateObject::OnCreate();

		SetScale(Vec3(1.0f));
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		transComp = GetComponent<Transform>();

		drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshToTransformMatrix(m_spanMat);
		drawComp->SetMeshResource(L"COMPANY");
		drawComp->SetDiffuse(COL_GRAY);
	}

	void Company::OnUpdate()
	{
		if (FindTag(tagName))
		{
			Debug::Log(L"COMPANY");
		}
	}

	// �I�v�V������ʂ̏���
	void Company::OptionMenu()
	{

	}
}