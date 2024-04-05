/*!
@file Building.cpp
@brief åöï®ÇÃé¿ëï
@author êVç»à®
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Company::OnCreate()
	{
		TemplateObject::OnCreate();

		SetScale(Vec3(5.0f));
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
}