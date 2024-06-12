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
		// モデルのテクスチャキー
		map<eMapLevel, wstring> m_modelTextures;
		
		// タグ名取得
		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();

		const Vec3 m_scale = Vec3(3.0f);
		const Vec3 m_position = Vec3(23.0f, 1.0f, -9.0f);

		const Vec2 m_spriteScale = Vec2(500.0f);

		const float m_maxStickValue = 0.2f;

		shared_ptr<PNTStaticDraw> m_drawComp;

		// 現在の入力値
		float m_currentX;

		Mat4x4 m_spanMat;

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

			m_modelTextures.emplace(eMapLevel::easyMap, L"EASY_TX");
			m_modelTextures.emplace(eMapLevel::normalMap, L"NORMAL_TX");
			m_modelTextures.emplace(eMapLevel::hardMap, L"HARD_TX");
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		/*
		@brief マップの難易度選択
		*/
		virtual void MapSelect();
	};
}