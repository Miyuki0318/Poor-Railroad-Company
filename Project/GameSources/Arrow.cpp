/*!
@file Arrow.cpp
@brief 矢印UIの実装
@author 新妻葵
*/

#pragma once
#include "stdafx.h"
#include "Arrow.h"
#include "TitleStage.h"
#include "RouteMap.h"
#include "MainCamera.h"

namespace basecross {
	void SelectArrow::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage(); // ステージの取得

		m_rightArrow = stagePtr->AddGameObject<Sprite>(L"RIGHTARROW_TX", scale, Vec3(posX, posY, 0.0f));
		m_leftArrow = stagePtr->AddGameObject<Sprite>(L"LEFTARROW_TX", scale, Vec3(-posX, posY, 0.0f));

		m_rightArrow.lock()->SetDrawActive(true);
		m_leftArrow.lock()->SetDrawActive(true);
	}

	void SelectArrow::OnUpdate()
	{
		// タイトルステージ取得
		const auto& stage = GetTypeStage<TitleStage>();

		// 路線図オブジェクト取得
		const auto& routeMap = stage->GetSharedGameObject<RouteMap>(L"RouteMap");

		// タイトルのカメラ取得
		const auto& camera = stage->GetView()->GetTargetCamera();
		auto& mainCamera = dynamic_pointer_cast<MainCamera>(camera);

		bool match = stage->MatchSelectObject(routeMap);
		bool zoomed = mainCamera->m_cameraState == MainCamera::ZoomedIn;

		m_rightArrow.lock()->SetDrawActive(match && zoomed);
		m_leftArrow.lock()->SetDrawActive(match && zoomed);

		if (match && zoomed)
		{
			ChangeColor();
		}
	}

	void SelectArrow::ChangeColor()
	{		
		// タイトルステージ取得
		const auto& stage = GetTypeStage<TitleStage>();

		// 路線図オブジェクト取得
		const auto& routeMap = stage->GetSharedGameObject<RouteMap>(L"RouteMap");

		float stickL = routeMap->GetInputMoveX();

		bool selectRight = routeMap->GetInputMoveRight();
		bool selectLeft = routeMap->GetInputMoveLeft();

		if (stickL != 0.0f)
		{
			if (selectRight)
			{
				m_rightArrow.lock()->SetDiffuseColor(COL_GRAY);
				m_leftArrow.lock()->SetDiffuseColor(COL_WHITE);
			}
			else if (selectLeft)
			{
				m_rightArrow.lock()->SetDiffuseColor(COL_WHITE);
				m_leftArrow.lock()->SetDiffuseColor(COL_GRAY);
			}
		}
		else
		{
			m_rightArrow.lock()->SetDiffuseColor(COL_WHITE);
			m_leftArrow.lock()->SetDiffuseColor(COL_WHITE);
		}
	}
}