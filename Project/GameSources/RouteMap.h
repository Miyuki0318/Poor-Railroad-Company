/*!
@file RouteMap.h
@brief 路線図(難易度選択)の設定
@author 新妻葵
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class RouteMap : public TemplateObject
	{
		enum eMapLevel
		{
			easyMap,
			normalMap,
			hardMap
		};

		const wstring m_modelTextures[3] = {
			L"EASY_TX",
			L"NORMAL_TX",
			L"HARD_TX"
		};

		const wstring m_mapTextures[3] = {
			L"EASYMAP_TX",
			L"NORMALMAP_TX",
			L"HARDMAP_TX"
		};

		const Vec3 m_scale = Vec3(3.0f);
		const Vec3 m_position = Vec3(30.0f, 1.0f, -15.0f);

		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();
		
		const Vec2 m_spriteScale = Vec2(500.0f);

		shared_ptr<PNTStaticDraw> m_drawComp;

		float m_currentX;

		Mat4x4 m_spanMat;

		Vec3 m_playerPosition;
		Vec3 m_cameraPosition;
		Vec3 m_cameraAt;

		shared_ptr<Sprite> m_mapSprite;
		eMapLevel m_mapLevel;

	public:
		RouteMap(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
			m_spanMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);

			m_currentX = 0.0f;
			m_mapLevel = eMapLevel::easyMap;
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		/*
		@brief マップの難易度選択
		*/
		virtual void MapSelect();
	};
}