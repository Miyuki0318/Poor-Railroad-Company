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
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshToTransformMatrix(m_spanMat);
		drawComp->SetMeshResource(L"BOARD");
		drawComp->SetTextureResource(L"BOARD_TX");

		// コリジョン追加
		GetStage()->AddGameObject<OriginalColl>(Vec3(5.5f, 1.0f, 1.0f), m_position);

		// スプライトの追加
		m_mapSprite = GetStage()->AddGameObject<Sprite>(m_mapTextures[m_mapLevel], Vec2(m_width,m_height));
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
		// スティックの入力量を取得
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
		m_mapSprite->SetTexture(m_mapTextures[m_mapLevel]);
	}
}