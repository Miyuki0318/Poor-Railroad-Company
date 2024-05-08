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

		//描画コンポーネントの設定
		SetAlphaActive(true);
		SetDrawActive(true);
		SetAlphaActive(false);

		const auto& stage = GetTypeStage<GameStage>();
		auto& stageMap = stage->GetStageMap();

		size_t row, col;
		row = ROW(m_spawnPos.z);
		col = COL(m_spawnPos.x);
		m_csvPos = Point2D<size_t>(row, col);

		// タグの設定
		AddTag(L"MiningObject");
	}

	void MiningObject::OnDelete() {
		const auto& stage = GetTypeStage<GameStage>();
		auto& stageMap = stage->GetStageMap();

		if (Utility::WithInElemRange(m_csvPos.x, m_csvPos.y, stageMap))
		{
			stageMap.at(m_csvPos.x).at(m_csvPos.y) = 0;
		}

		stage->GetSharedGameObject<RailManager>(L"RailManager")->GuideRecalculation();
	}


	void Tree::OnCreate() {
		MiningObject::OnCreate();
		SetScale(Vec3(1.0f));

		// 新規ドローコンポーネントの設定
		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		int random = RangeRand(2, 1);
		ptrDraw->SetMultiMeshResource(L"TREE" + to_wstring(random));
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);

		// タグの設定
		AddTag(L"Tree");
	}

	void Tree::OnUpdate() {
		// 採掘回数が上限に達した場合、オブジェクトを破壊
		if (m_miningCount == m_miningCountLimit) {
			m_state = eState::Broken;
			SetUpdateActive(false);
			SetDrawActive(false);
			MiningObject::OnDelete();
		}
	}

	void Tree::OnMining() {
		// 採掘した回数の増加
		m_miningCount++;
	}

	void Tree::OnReset() {
		// 変数の初期化
		m_miningCount = 0;

		// オブジェクトの見た目を初期状態にする処理を入れる
		 
		
		// オブジェクトの更新を再開
		SetUpdateActive(true);
	}

	void Tree::AccordingState() {
		switch (m_state)
		{
		case eState::Damage:
			// 損傷時の見た目に変更する処理を入れる

			break;
		case eState::Broken:
			// 破壊されたアニメーションを再生する処理を入れる

			// 破壊時のパーティクルを再生する処理を入れる

			break;
		case eState::None:
			// 更新を停止し、不可視にする
			SetUpdateActive(false);
			SetDrawActive(false);
			MiningObject::OnDelete();

			break;
		default:
			break;
		}
	}


	void Rock::OnCreate() {
		MiningObject::OnCreate();
		SetScale(Vec3(1.0f));

		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		int random = RangeRand(3, 1);
		ptrDraw->SetMeshResource(L"ROCK" + to_wstring(random));  
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);

		// タグの設定
		AddTag(L"Rock");
	}

	void Rock::OnUpdate() {

		// 採掘回数が上限に達した場合、オブジェクトを破壊
		if (m_miningCount == m_miningCountLimit) {
			// 現状、破壊されるアニメーションがないので更新止めるだけ
			SetUpdateActive(false);
			SetDrawActive(false);
			MiningObject::OnDelete();
		}
	}

	void Rock::OnMining() {
		// 採掘した回数の増加
		m_miningCount++;
	}

	void Rock::OnReset() {
		// 変数の初期化
		m_miningCount = 0;

		//ドローコンポーネント
		

		// オブジェクトの更新を再開
		SetUpdateActive(true);
	}

	void Rock::AccordingState() {
		switch (m_state)
		{
		case eState::Damage:
			// 損傷時の見た目に変更する処理を入れる

			break;
		case eState::Broken:
			// 破壊されたアニメーションを再生する処理を入れる

			// 破壊時のパーティクルを再生する処理を入れる

			break;
		case eState::None:
			// 更新を停止し、不可視にする
			SetUpdateActive(false);
			SetDrawActive(false);
			MiningObject::OnDelete();

			break;
		default:
			break;
		}
	}

}