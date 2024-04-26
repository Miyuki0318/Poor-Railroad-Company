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

		// �傫���E��]�E�ʒu�̐ݒ�
		SetScale(m_scale);
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		// �I�u�W�F�N�g�`��
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshToTransformMatrix(m_spanMat);
		drawComp->SetMeshResource(L"BOARD");
		drawComp->SetTextureResource(L"BOARD_TX");

		// �R���W�����ǉ�
		GetStage()->AddGameObject<OriginalColl>(Vec3(5.5f, 1.0f, 1.0f), m_position);

		// �X�v���C�g�̒ǉ�
		GetStage()->AddGameObject<Sprite>(m_mapLevel[eMapLevel::easyMap], Vec2(500.0f));
	}

	void RouteMap::OnUpdate()
	{
		if (FindTag(tagName))
		{
			Debug::Log(L"ROUTEMAP");
		}
	}
}