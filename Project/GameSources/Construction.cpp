/*!
@file Construction.cpp
@brief �H������̎���(�Q�[���X�^�[�g)
@author �V�Ȉ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Construction::OnCreate()
	{
		TemplateObject::OnCreate();

		SetScale(Vec3(1.0f));
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshToTransformMatrix(m_spanMat);
		drawComp->SetMeshResource(L"CONSTRUCTION");
		drawComp->SetDiffuse(COL_YELOW);
	
		GetStage()->AddGameObject<OriginalColl>(Vec3(3.0f, 1.0f, 5.0f), m_position);
	}

	void Construction::OnUpdate()
	{
		if (MoveTrainFlag())
		{
			GameStart();
		}	
	}

	void Construction::GameStart()
	{
		auto sprite = GetStage()->GetSharedGameObject<Sprite>(L"FadeSprite");

		if (sprite->GetDiffuseColor().getW() >= 1.0f)
		{
			// �X�e�[�W�ύX�̂��߁A�V�[�����擾
			auto& scene = App::GetApp()->GetScene<Scene>();

			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
		}
	}
}