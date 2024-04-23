/*!
@file OriginalCollision.cpp
@brief �I���W�i���̃R���W����
@author �V�Ȉ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void OriginalColl::OnCreate()
	{
		TemplateObject::OnCreate();

		SetScale(m_scale);
		SetRotation(Vec3(0.0f));
		SetPosition(m_pos);

		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetDrawActive(false);

		auto collComp = AddComponent<CollisionObb>();
		collComp->SetFixed(true);
		collComp->SetDrawActive(true);
	}
}