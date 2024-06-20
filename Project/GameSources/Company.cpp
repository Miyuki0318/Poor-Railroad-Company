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
		drawComp->SetTextureResource(L"COMPANY_TX");

		m_menuSprite = GetStage()->AddGameObject<Sprite>(L"MENU_TX", m_scale);
		m_menuSprite->SetDrawActive(FindTag(tagName));
	}

	void Company::OnUpdate()
	{
		m_menuSprite->SetDrawActive(FindTag(tagName));
	}

	// オプション画面の処理
	void Company::OptionMenu()
	{
		m_menuSprite->SetDrawActive(true);
	}
}