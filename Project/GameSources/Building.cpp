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

		SetScale(Vec3(1.0f));
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshToTransformMatrix(m_spanMat);
		drawComp->SetMeshResource(L"COMPANY");
		drawComp->SetDiffuse(COL_GRAY);

		m_menuSprite = GetStage()->AddGameObject<Sprite>(L"MENU_TX", Vec2(500.0f), Vec3(0.0f));
		m_menuSprite->SetDrawActive(m_spriteActive);
	}

	void Company::OnUpdate()
	{
		auto& a = GetStage()->GetSharedGameObject<CompanyCollision>(L"COMPANYCOLL");
		if (a->GetPlayerHitFlag())
		{
			ButtonPush();
		}
	}

	// ボタンを押した時の処理
	void Company::ButtonPush()
	{
		if (Input::GetPushX())
		{
			if (!m_spriteActive)
			{
				m_spriteActive = true;
			}
			else
			{
				m_spriteActive = false;
			}
		}

		m_menuSprite->SetDrawActive(m_spriteActive);
	}

	// オプション画面の処理
	void Company::OptionMenu()
	{

	}

	void CompanyCollision::OnCreate()
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

	void CompanyCollision::OnUpdate()
	{
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_position);
	}

	void CompanyCollision::OnCollisionEnter(shared_ptr<GameObject>& object)
	{
		if (object->FindTag(L"Player"))
		{
			m_playerHit = true;
		}
	}

	void CompanyCollision::OnCollisionExit(shared_ptr<GameObject>& object)
	{
		if (object->FindTag(L"Player"))
		{
			m_playerHit = false;
		}
	}

	/*
	* 工事現場の実装
	*/
	void Construction::OnCreate()
	{
		TemplateObject::OnCreate();

		SetScale(Vec3(1.0f));
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		auto drawComp = AddComponent<PNTStaticDraw>();
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