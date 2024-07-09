/*!
@file Company.cpp
@brief 会社の実装(メニュー画面)
@author 新妻葵
*/

#pragma once
#include "stdafx.h"
#include "Company.h"

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

		// ショップ機能の生成
		m_shopPtr = GetStage()->AddGameObject<Shop>();
	}

	void Company::OnUpdate()
	{
		if (FindTag(tagName)) {
			if (m_shopPtr.lock()->GetState() == Shop::eShopState::Close) {
				m_shopPtr.lock()->SetState(ShopShoppingState::Instance());
			}
		}
	}

	// オプション画面の処理
	void Company::OptionMenu()
	{
		m_menuSprite->SetDrawActive(true);
	}
}