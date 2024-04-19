/*!
@file RouteMap.cpp
@brief �H���}(��Փx�I��)�̎���
@author �V�Ȉ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void RouteMap::OnCreate()
	{
		TemplateObject::OnCreate();

		SetScale(m_scale);
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshToTransformMatrix(m_spanMat);
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		auto collComp = AddComponent<CollisionObb>();
		collComp->SetDrawActive(true);
		collComp->SetFixed(true);
	}

	void RouteMap::OnUpdate()
	{
		if (DistanceToPlayer())
		{
			PushButton();
		}

		Debug::Log(pushSelectButton);
	}

	void RouteMap::PushButton()
	{
		if (Input::GetPushX())
		{
			if (!pushSelectButton)
			{
				pushSelectButton = true;
			}
			else
			{
				pushSelectButton = false;
			}
		}
	}

	bool RouteMap::DistanceToPlayer()
	{
		m_playerPosition = GetStage()->GetSharedGameObject<TitlePlayer>(L"TitlePlayer", true)->GetPosition();

		Vec3 diff = m_position - m_playerPosition;
		float distance = diff.length();

		if (distance < m_hitLength)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}