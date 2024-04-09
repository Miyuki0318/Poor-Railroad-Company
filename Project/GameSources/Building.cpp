/*!
@file Building.cpp
@brief �����̎���
@author �V�Ȉ�
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
		if (m_pushButton)
		{
			ButtonPush();
		}
	}

	// �{�^�������������̏���
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

	// �I�v�V������ʂ̏���
	void Company::OptionMenu()
	{

	}

	void Company::OnCollisionEnter(shared_ptr<GameObject>& object)
	{
		if (object->FindTag(L"Player"))
		{
			m_pushButton = true;
		}
		else
		{
			m_pushButton = false;
		}
	}

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
	}

	void Construction::OnUpdate()
	{
		StartButtonPush();
	}

	void Construction::StartButtonPush()
	{
		if (Input::GetPushA())
		{
			// �X�e�[�W�ύX�̂��߁A�V�[�����擾
			auto& scene = App::GetApp()->GetScene<Scene>();

			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
		}
	}

	void Construction::Start()
	{

	}
}