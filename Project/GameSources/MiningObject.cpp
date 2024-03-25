/*!
@file MiningObject.cpp
@brief
@author 小宅碧
*/

#pragma once
#include "stdafx.h"
#include "Project.h"


namespace basecross {
	void MiningObject::OnCreate() {
		//初期位置などの設定
		TemplateObject::OnCreate();
		SetScale(Vec3(1.0f));
		SetRotation(Vec3(0.0f));
		SetPosition(m_spawnPos);

		//CollisionObb衝突判定を付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);

		//描画コンポーネントの設定
		SetAlphaActive(true);
		SetDrawActive(true);

		// タグの設定
		AddTag(L"MiningObject");
	}



	void Tree::OnCreate() {
		MiningObject::OnCreate();

		// 新規ドローコンポーネントの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		ptrDraw->SetDiffuse(Col4(0.0f, 0.4f, 0.1f, 1.0f));

		// タグの設定
		AddTag(L"Tree");
	}

	void Tree::OnUpdate() {
		if (m_setFlg) {
			// 自身のコリジョンを取得
			auto ptrColl = GetComponent<CollisionObb>();
			// MiningObjectGroupを取得
			auto group = GetStage()->GetSharedObjectGroup(L"MiningObject");
			// MiningObjectGroup同士の衝突判定をオフ
			ptrColl->SetExcludeCollisionGroup(group);

			// フラグをfalseに変更
			m_setFlg = false;
		}

		// 採掘回数が上限に達した場合、オブジェクトを破壊
		if (m_miningCount == m_miningCountLimit) {
			// 現状、破壊されるアニメーションがないので更新止めるだけ
			SetUpdateActive(false);
		}
	}

	void Tree::OnMining() {
		// 採掘した回数の増加
		m_miningCount++;
	}

	void Tree::OnReset() {
		// 変数の初期化
		m_miningCount = 0;
		// オブジェクトの更新を再開
		SetUpdateActive(true);
	}



	void Rock::OnCreate() {
		MiningObject::OnCreate();

		// 新規ドローコンポーネントの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		ptrDraw->SetDiffuse(Col4(0.3f, 0.3f, 0.3f, 1.0f));

		// タグの設定
		AddTag(L"Rock");
	}

	void Rock::OnUpdate() {
		if (m_setFlg) {
			// 自身のコリジョンを取得
			auto ptrColl = GetComponent<CollisionObb>();
			// MiningObjectGroupを取得
			auto group = GetStage()->GetSharedObjectGroup(L"MiningObject");
			// MiningObjectGroup同士の衝突判定をオフ
			ptrColl->SetExcludeCollisionGroup(group);

			// フラグをfalseに変更
			m_setFlg = false;
		}

		// 採掘回数が上限に達した場合、オブジェクトを破壊
		if (m_miningCount == m_miningCountLimit) {
			// 現状、破壊されるアニメーションがないので更新止めるだけ
			SetUpdateActive(false);
		}
	}

	void Rock::OnMining() {
		// 採掘した回数の増加
		m_miningCount++;
	}

	void Rock::OnReset() {
		// 変数の初期化
		m_miningCount = 0;
		// オブジェクトの更新を再開
		SetUpdateActive(true);
	}


}