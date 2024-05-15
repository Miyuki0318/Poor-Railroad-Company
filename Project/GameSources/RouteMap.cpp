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
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshToTransformMatrix(m_spanMat);
		m_drawComp->SetMeshResource(L"BOARD");
		m_drawComp->SetTextureResource(m_modelTextures[m_mapLevel]);

		// �R���W�����ǉ�
		GetStage()->AddGameObject<OriginalColl>(Vec3(5.5f, 1.0f, 1.0f), m_position);

		// �X�v���C�g�̒ǉ�
		m_mapSprite = GetStage()->AddGameObject<Sprite>(m_mapTextures[m_mapLevel], m_spriteScale);
		m_mapSprite->SetDrawActive(false);
	}

	void RouteMap::OnUpdate()
	{
		if (FindTag(tagName))
		{
			m_mapSprite->SetDrawActive(true);
			MapSelect();
		}
		else
		{
			m_mapSprite->SetDrawActive(false);
		}
	}

	void RouteMap::MapSelect()
	{
		// �X�e�B�b�N�̓��͗ʂ��擾
		Vec2 move = Input::GetLStickValue();

		float stickX = move.getX();

		if (stickX < 0.0f && m_currentX >= 0.0f)
		{
			switch (m_mapLevel)
			{
			case RouteMap::easyMap:
				m_mapLevel = RouteMap::hardMap;
				break;

			case RouteMap::normalMap:
				m_mapLevel = RouteMap::easyMap;
				break;

			case RouteMap::hardMap:
				m_mapLevel = RouteMap::normalMap;
				break;

			default:
				break;
			}
		}
		else if (stickX > 0.0f && m_currentX <= 0.0f)
		{
			switch (m_mapLevel)
			{
			case RouteMap::easyMap:
				m_mapLevel = RouteMap::normalMap;
				break;

			case RouteMap::normalMap:
				m_mapLevel = RouteMap::hardMap;
				break;

			case RouteMap::hardMap:
				m_mapLevel = RouteMap::easyMap;
				break;

			default:
				break;
			}
		}

		m_currentX = stickX;
		m_drawComp->SetTextureResource(m_modelTextures[m_mapLevel]);
		m_mapSprite->SetTexture(m_mapTextures[m_mapLevel]);
	}
}