/*!
@file Company.cpp
@brief 会社の実装(メニュー画面)
@author 新妻葵
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

	// オプション画面の処理
	void Company::OptionMenu()
	{

	}
}