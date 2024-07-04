/*!
@file RouteMap.cpp
@brief 路線図(難易度選択)の実装
@author 新妻葵
*/

#pragma once
#include "stdafx.h"
#include "RouteMap.h"
#include "Input.h"

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

		// 左スティックの入力量を取得
		Vec2 move = Input::GetLStickValue();

		// 左スティックX軸の入力値
		float stickX = move.getX();

		if (stickX < -m_maxStickValue && m_currentX >= 0.0f)
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
		else if (stickX > m_maxStickValue && m_currentX <= 0.0f)
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

		// 大きさ・回転・位置の設定
		SetScale(m_scale);
		SetRotation(Vec3(0.0f));
		SetPosition(m_position);

		// オブジェクト描画
		m_drawComp = AddComponent<PNTStaticDraw>();
		m_drawComp.lock()->SetMeshToTransformMatrix(m_spanMat);
		m_drawComp.lock()->SetMeshResource(L"SIGNBOARD");
		m_drawComp.lock()->SetTextureResource(m_textureKey);
	}
}