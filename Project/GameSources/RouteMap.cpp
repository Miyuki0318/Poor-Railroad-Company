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
		//GetStage()->AddGameObject<Sprite>(m_mapLevel[eMapLevel::easyMap], Vec2(500.0f));
	}

	void RouteMap::OnUpdate()
	{
		if (FindTag(tagName))
		{
			Debug::Log(L"ROUTEMAP");
		}
	}
}