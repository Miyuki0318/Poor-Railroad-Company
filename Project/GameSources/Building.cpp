/*!
@file Building.cpp
@brief �����̎���
@author �V�Ȉ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Company::OnCreate()
	{
		TemplateObject::OnCreate();

		SetScale(Vec3(10.0f));
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		auto drawComp = AddComponent<PNTStaticDraw>();
		//drawComp->SetMeshToTransformMatrix(spanMat);
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetDiffuse(COL_WHITE);
	}

	void Company::OnUpdate()
	{
	}

	// �{�^�������������̏���
	void Company::ButtonPush()
	{

	}

	// �I�v�V������ʂ̏���
	void Company::OptionMenu()
	{

	}


	void Construction::OnCreate()
	{
		TemplateObject::OnCreate();

		SetScale(Vec3(5.0f, 5.0f, 10.0f));
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		auto drawComp = AddComponent<PNTStaticDraw>();
		//drawComp->SetMeshToTransformMatrix(spanMat);
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetDiffuse(COL_WHITE);
	}

	void Construction::OnUpdate()
	{

	}

	void Construction::StartButtonPush()
	{

	}

	void Construction::Start()
	{

	}
}