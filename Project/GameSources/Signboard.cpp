/*!
@file Signboard.cpp
@brief �Ŕ�(�l���I��)�̎���
@author �V�Ȉ�
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void SignBoard::OnCreate()
	{
		TemplateObject::OnCreate();

		SetScale(m_scale);
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshToTransformMatrix(m_spanMat);
		drawComp->SetMeshResource(L"BOARD");
		drawComp->SetTextureResource(L"BOARD_TX");
	}

	void SignBoard::OnUpdate()
	{
		if (DistanceToPlayer())
		{
			PushButton();
		}

		Debug::Log(pushButton);
	}

	void SignBoard::PushButton()
	{
		if (Input::GetPushX())
		{
			if (!pushButton)
			{
				pushButton = true;
			}
			else
			{
				pushButton = false;
			}
		}
	}

	bool SignBoard::DistanceToPlayer()
	{
		m_playerPosition = GetStage()->GetSharedGameObject<TitlePlayer>(L"TitlePlayer",true)->GetPosition();

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