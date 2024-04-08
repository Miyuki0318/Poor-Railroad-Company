/*!
@file Building.cpp
@brief 建物の実装
@author 新妻葵
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

	// ボタンを押した時の処理
	void Company::ButtonPush()
	{

	}

	// オプション画面の処理
	void Company::OptionMenu()
	{

	}
}