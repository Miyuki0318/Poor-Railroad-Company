/*!
* 
@file Signboard.cpp
@brief �Ŕ�(�l���I��)�̎���
@author �V�Ȉ�
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
	
		GetStage()->AddGameObject<OriginalColl>(Vec3(5.5f, 1.0f, 1.0f), m_position);

		//m_mapSprite->GetStage()->AddGameObject<Sprite>()
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