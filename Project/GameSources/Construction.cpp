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

		transComp = GetComponent<Transform>();

		drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshToTransformMatrix(m_spanMat);
		drawComp->SetMeshResource(L"CONSTRUCTION");
		drawComp->SetDiffuse(COL_YELOW);

		const float width = static_cast<float>(App::GetApp()->GetGameWidth());
		const float height = static_cast<float>(App::GetApp()->GetGameHeight());

		m_fadeSprite = GetStage()->AddGameObject<Sprite>(L"FADE_TX", Vec2(width, height), Vec3(0.0f));
		m_fadeSprite->SetDiffuseColor(COL_ALPHA);
	
		GetStage()->AddGameObject<OriginalColl>(Vec3(3.0f, 1.0f, 5.0f), m_position);
	}

	void Construction::OnUpdate()
	{
		if (FindTag(tagName))
		{
			Debug::Log(L"CONSTRUCTION");
		}
	}

	void Construction::StartCountDown()
	{
		//if (m_fadeSprite->FadeInColor(3.0f))
		//{
		//	// ステージ変更のため、シーンを取得
		//	auto& scene = App::GetApp()->GetScene<Scene>();

		//	PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
		//}
	}
}