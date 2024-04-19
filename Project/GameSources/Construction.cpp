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
	}

	void Construction::OnUpdate()
	{
		auto coll = GetStage()->GetSharedGameObject<ConstructionCollision>(L"CONSTRUCTCOLL");
		if (coll->GetPlayerHitFlag())
		{
			StartButtonPush();
			transComp->SetScale(Vec3(1.2f));
		}
		else
		{
			transComp->SetScale(Vec3(1.0f));
		}
		StartCountDown();
	}

	void Construction::StartButtonPush()
	{
		if (Input::GetPushA() && !isPushButton)
		{
			isPushButton = true;
		}
	}

	void Construction::StartCountDown()
	{
		if (isPushButton && m_fadeSprite->FadeInColor(3.0f))
		{
			// ステージ変更のため、シーンを取得
			auto& scene = App::GetApp()->GetScene<Scene>();

			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
		}
	}

	void ConstructionCollision::OnCreate()
	{
		TemplateObject::OnCreate();

		SetScale(m_scale);
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetDrawActive(false);

		auto CollComp = AddComponent<CollisionObb>();
		CollComp->SetDrawActive(true);
	}

	void ConstructionCollision::OnUpdate()
	{
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_position);
	}

	void ConstructionCollision::OnCollisionEnter(shared_ptr<GameObject>& object)
	{
		if (object->FindTag(L"Player"))
		{
			m_playerHit = true;
		}
	}

	void ConstructionCollision::OnCollisionExit(shared_ptr<GameObject>& object)
	{
		if (object->FindTag(L"Player"))
		{
			m_playerHit = false;
		}
	}
}