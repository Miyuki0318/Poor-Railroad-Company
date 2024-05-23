/*!
@file MiningObject.cpp
@brief
@author 小宅碧
*/

#pragma once
#include "stdafx.h"
#include "Project.h"


namespace basecross {

	using namespace Utility;

	void MiningObject::OnCreate() {
		//初期位置などの設定
		TemplateObject::OnCreate();
		float rot = RangeRand(2, -1) * XM_PIDIV2;
		SetRotation(Vec3(0.0f, rot, 0.0f));
		SetPosition(m_spawnPos);
		SetScale(m_startScale);

		//描画コンポーネントの設定
		SetAlphaActive(true);
		SetDrawActive(true);
		SetAlphaActive(false);

		const auto& stage = GetTypeStage<BaseStage>();
		auto& stageMap = stage->GetStageMap();

		size_t row, col;
		row = ROW(m_spawnPos.z);
		col = COL(m_spawnPos.x);
		m_csvPos = Point2D<size_t>(row, col);

		// タグの設定
		AddTag(L"MiningObject");
	}

	void MiningObject::OnMining() {
		// 採掘した回数の増加
		m_miningCount++;

		switch (m_currentState)
		{
		case eState::Normal:
			m_currentState = eState::Damage;
			break;

		case eState::Damage:
			m_currentState = eState::None;
			break;

		default:
			break;
		}
	}
	void MiningObject::OnDelete() {
		const auto& stage = GetTypeStage<BaseStage>();
		auto& stageMap = stage->GetStageMap();

		if (Utility::WithInElemRange(m_csvPos.x, m_csvPos.y, stageMap))
		{
			stageMap.at(m_csvPos.x).at(m_csvPos.y) = 0;
		}

		stage->GetSharedGameObject<RailManager>(L"RailManager")->GuideRecalculation();
	}


	void Tree::OnCreate() {
		MiningObject::OnCreate();

		// 新規ドローコンポーネントの設定
		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		int random = RangeRand(2, 1);
		ptrDraw->SetMultiMeshResource(L"TREE" + to_wstring(random));
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		//ptrDraw->SetOwnShadowActive(true);

		// タグの設定
		AddTag(L"TREE");
	}

	void Tree::OnUpdate() {
		AccordingState();
	}

	void Tree::OnMining() {
		MiningObject::OnMining();
	}

	void Tree::OnReset() {
		// 変数の初期化
		m_miningCount = 0;
		m_currentState = eState::Normal;
		m_pastState = m_currentState;

		// オブジェクトスケールを初期状態にする
		SetScale(m_startScale);
		
		// オブジェクトの更新を再開
		SetUpdateActive(true);
	}

	void Tree::AccordingState() {
		if (m_currentState != m_pastState) {
			switch (m_currentState)
			{
			case eState::Damage:
				// スケールを半分に変更
				SetScale(m_damageScale);
				break;

			case eState::None:
				// スケールを0に変更
				SetScale(0.0f);

				// 更新を停止し、不可視にする
				SetUpdateActive(false);
				SetDrawActive(false);
				MiningObject::OnDelete();

				break;
			default:
				break;
			}

			// 前フレームのステートとして保存
			m_pastState = m_currentState;
		}
	}


	void Rock::OnCreate() {
		MiningObject::OnCreate();

		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		int random = RangeRand(3, 1);
		ptrDraw->SetMeshResource(L"ROCK" + to_wstring(random));
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		//ptrDraw->SetOwnShadowActive(true);

		// タグの設定
		AddTag(L"ROCK");
	}

	void Rock::OnUpdate() {
		AccordingState();
	}

	void Rock::OnMining() {
		MiningObject::OnMining();
	}

	void Rock::OnReset() {
		// 変数の初期化
		m_miningCount = 0;
		m_currentState = eState::Normal;
		m_pastState = m_currentState;

		// オブジェクトスケールを初期状態にする
		SetScale(m_startScale);

		// オブジェクトの更新を再開
		SetUpdateActive(true);
	}

	void Rock::AccordingState() {
		if (m_currentState != m_pastState) {
			switch (m_currentState)
			{
			case eState::Damage:
				// スケールを半分に変更
				SetScale(m_damageScale);
				break;

			case eState::None:
				// スケールを0に変更
				SetScale(0.0f);

				// 更新を停止し、不可視にする
				SetUpdateActive(false);
				SetDrawActive(false);
				MiningObject::OnDelete();

				break;
			default:
				break;
			}

			// 前フレームのステートとして保存
			m_pastState = m_currentState;
		}
	}

}