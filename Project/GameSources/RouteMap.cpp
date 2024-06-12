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
		m_drawComp->SetTextureResource(m_modelTextures.at(m_mapLevel));
	}

	void RouteMap::OnUpdate()
	{
		if (FindTag(tagName))
		{
			MapSelect();
		}
	}

	void RouteMap::MapSelect()
	{
		auto scene = App::GetApp()->GetScene<Scene>();

		// ���X�e�B�b�N�̓��͗ʂ��擾
		Vec2 move = Input::GetLStickValue();

		// ���X�e�B�b�NX���̓��͒l
		float stickX = move.getX();

		if (stickX < -m_maxStickValue && m_currentX >= 0.0f)
		{
			switch (m_mapLevel)
			{
			case eMapLevel::easyMap:
				m_mapLevel = eMapLevel::hardMap;
				break;

			case eMapLevel::normalMap:
				m_mapLevel = eMapLevel::easyMap;
				break;

			case eMapLevel::hardMap:
				m_mapLevel = eMapLevel::normalMap;
				break;

			default:
				break;
			}
		}
		else if (stickX > m_maxStickValue && m_currentX <= 0.0f)
		{
			switch (m_mapLevel)
			{
			case eMapLevel::easyMap:
				m_mapLevel = eMapLevel::normalMap;
				break;

			case eMapLevel::normalMap:
				m_mapLevel = eMapLevel::hardMap;
				break;

			case eMapLevel::hardMap:
				m_mapLevel = eMapLevel::easyMap;
				break;

			default:
				break;
			}
		}

		m_currentX = stickX;
		m_drawComp->SetTextureResource(m_modelTextures[m_mapLevel]);
		scene->SetMapLevel(m_mapLevel);
	}
}