/*!
@file RouteMap.cpp
@brief ˜Hü}(“ïˆÕ“x‘I‘ğ)‚ÌÀ‘•
@author VÈˆ¨
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void RouteMap::OnCreate()
	{
		TemplateObject::OnCreate();

		SetScale(m_scale);
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshToTransformMatrix(m_spanMat);
		drawComp->SetMeshResource(L"BOARD");
		drawComp->SetTextureResource(L"BOARD_TX");

		GetStage()->AddGameObject<OriginalColl>(Vec3(5.5f, 1.0f, 1.0f), m_position);
	}

	void RouteMap::OnUpdate()
	{
	}
}