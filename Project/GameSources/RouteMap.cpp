/*!
@file RouteMap.cpp
@brief 路線図(難易度選択)の実装
@author 新妻葵
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void RouteMap::OnCreate()
	{
		TemplateObject::OnCreate();

		// 大きさ・回転・位置の設定
		SetScale(m_scale);
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		// オブジェクト描画
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp->SetMeshToTransformMatrix(m_spanMat);
		m_drawComp->SetMeshResource(L"BOARD");
		m_drawComp->SetTextureResource(m_modelTextures.at(m_mapLevel));

		// スプライトの追加
		m_mapSprite = GetStage()->AddGameObject<Sprite>(m_mapTextures.at(m_mapLevel), m_spriteScale);
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
		auto scene = App::GetApp()->GetScene<Scene>();

		// 左スティックの入力量を取得
		Vec2 move = Input::GetLStickValue();

		// 左スティックX軸の入力値
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
		m_mapSprite->SetTexture(m_mapTextures[m_mapLevel]);
		scene->SetMapLevel(m_mapLevel);
	}
}