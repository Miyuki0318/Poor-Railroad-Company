/*!
@file Company.cpp
@brief 会社の実装(メニュー画面)
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

		transComp = GetComponent<Transform>();

		drawComp = AddComponent<PNTStaticDraw>();
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
			transComp->SetScale(Vec3(1.2f));
			ButtonPush();
		}
		else
		{
			transComp->SetScale(Vec3(1.0f));
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
}