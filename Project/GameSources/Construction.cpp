/*!
@file Construction.cpp
@brief 工事現場の実装(ゲームスタート)
@author 新妻葵
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
			// ステージ変更のため、シーンを取得
			auto& scene = App::GetApp()->GetScene<Scene>();

			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
		}
	}
}