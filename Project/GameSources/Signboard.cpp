/*!
* 
@file Signboard.cpp
@brief ŠÅ”Â(l”‘I‘ğ)‚ÌÀ‘•
@author VÈˆ¨
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void SignBoard::OnCreate()
	{
		TemplateObject::OnCreate();

		SetScale(m_scale);
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshToTransformMatrix(m_spanMat);
		drawComp->SetMeshResource(L"BOARD");
		drawComp->SetTextureResource(L"BOARD_TX");
	}

	void SignBoard::OnUpdate()
	{
		if (FindTag(tagName))
		{
			Debug::Log(L"SIGNBOARD");
		}
	}

	void SignBoard::BoardSelect()
	{
	}
}