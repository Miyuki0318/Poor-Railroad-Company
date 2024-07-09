/*!
@file Company.cpp
@brief ��Ђ̎���(���j���[���)
@author �V�Ȉ�
*/

#pragma once
#include "stdafx.h"
#include "Company.h"
#include "ShopShoppingState.h"

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

		// �V���b�v�@�\�̐���
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

	// �I�v�V������ʂ̏���
	void Company::OptionMenu()
	{
		m_menuSprite->SetDrawActive(true);
	}
}