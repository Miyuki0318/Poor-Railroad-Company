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

		SetScale(Vec3(10.0f));
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		auto drawComp = AddComponent<PNTStaticDraw>();
		//drawComp->SetMeshToTransformMatrix(spanMat);
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetDiffuse(COL_GRAY);

		m_menuSprite = GetStage()->AddGameObject<Sprite>(L"MENU_TX", Vec2(500.0f), Vec3(0.0f));
		m_menuSprite->SetDrawActive(m_spriteActive);
	}

	void Company::OnUpdate()
	{
		ButtonPush();
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


	void Construction::OnCreate()
	{
		TemplateObject::OnCreate();

		SetScale(Vec3(5.0f, 5.0f, 10.0f));
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		auto drawComp = AddComponent<PNTStaticDraw>();
		//drawComp->SetMeshToTransformMatrix(spanMat);
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetDiffuse(COL_YELOW);
	}

	void Construction::OnUpdate()
	{
		StartButtonPush();
	}

	void Construction::StartButtonPush()
	{
		if (Input::GetPushA())
		{
			// ステージ変更のため、シーンを取得
			auto& scene = App::GetApp()->GetScene<Scene>();

			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
		}
	}

	void Construction::Start()
	{

	}
}