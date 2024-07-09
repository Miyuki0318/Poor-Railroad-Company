/*!
@file RouteMap.cpp
@brief �H���}(��Փx�I��)�̎���
@author �V�Ȉ�
*/

#pragma once
#include "stdafx.h"
#include "RouteMap.h"
#include "Input.h"

namespace basecross {
	void RouteMap::OnCreate()
	{
		TemplateObject::OnCreate();

		// �傫���E��]�E�ʒu�̐ݒ�
		SetScale(Vec3(1.0f));
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

		if (holdTag && !FindTag(tagName))
		{
			StartSE(L"PUSH_SE", 1.0f);
		}

		holdTag = FindTag(tagName);
	}

	void RouteMap::MapSelect()
	{
		auto scene = App::GetApp()->GetScene<Scene>();

		// ���X�e�B�b�N�̓��͗ʂ��擾
		Vec2 move = Input::GetLStickValue();

		// ���X�e�B�b�NX���̓��͒l
		float stickX = move.getX();

		moveLeft = stickX < -m_maxStickValue && m_currentX >= 0.0f;
		moveRight = stickX > m_maxStickValue && m_currentX <= 0.0f;

		if (moveLeft)
		{
			switch (m_mapLevel)
			{
			case eMapLevel::FirstMap:
				m_mapLevel = eMapLevel::FifthMap;
				break;

			case eMapLevel::SecondMap:
				m_mapLevel = eMapLevel::FirstMap;
				break;

			case eMapLevel::ThirdMap:
				m_mapLevel = eMapLevel::SecondMap;
				break;

			case eMapLevel::FourthMap:
				m_mapLevel = eMapLevel::ThirdMap;
				break;

			case eMapLevel::FifthMap:
				m_mapLevel = eMapLevel::FourthMap;
				break;

			default:
				break;
			}
		}
		else if (moveRight)
		{
			switch (m_mapLevel)
			{
			case eMapLevel::FirstMap:
				m_mapLevel = eMapLevel::SecondMap;
				break;

			case eMapLevel::SecondMap:
				m_mapLevel = eMapLevel::ThirdMap;
				break;

			case eMapLevel::ThirdMap:
				m_mapLevel = eMapLevel::FourthMap;
				break;

			case eMapLevel::FourthMap:
				m_mapLevel = eMapLevel::FifthMap;
				break;

			case eMapLevel::FifthMap:
				m_mapLevel = eMapLevel::FirstMap;
				break;

			default:
				break;
			}
		}

		m_currentX = stickX;
		m_drawComp->SetTextureResource(m_modelTextures[m_mapLevel]);
		scene->SetMapLevel(m_mapLevel);
	}

	void SignBoard::OnCreate()
	{
		TemplateObject::OnCreate();

		// �傫���E��]�E�ʒu�̐ݒ�
		SetScale(m_scale);
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		// �I�u�W�F�N�g�`��
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp.lock()->SetMeshToTransformMatrix(m_spanMat);
		m_drawComp.lock()->SetMeshResource(L"SIGNBOARD");
		m_drawComp.lock()->SetTextureResource(m_textureKey);
	}
}