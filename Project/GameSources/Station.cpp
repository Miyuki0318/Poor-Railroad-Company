/*!
@file Station.cpp
@brief Tarminalの後方マスに配置される駅オブジェ
@author 小宅碧
*/

#pragma once
#include "stdafx.h"
#include "Project.h"


namespace basecross {
	void Station::OnCreate() {
		//初期位置などの設定
		TemplateObject::OnCreate();
		SetScale(Vec3(1.0f, 2.0f, 1.0f));
		SetRotation(Vec3(0.0f));
		float hulfScale = GetScale().getY() / 2.0f;
		SetPosition(m_spawnPos.x, m_spawnPos.y + hulfScale, m_spawnPos.z);

		//CollisionObb衝突判定を付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);

		// 新規ドローコンポーネントの設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		ptrDraw->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 1.0f));

		//描画コンポーネントの設定
		SetAlphaActive(true);
		SetDrawActive(true);
		SetAlphaActive(false);

		// タグの設定
		AddTag(L"Station");

	}

	void Station::OnUpdate() {
		if (!m_setFlg) {
			// 自身のコリジョンを取得
			auto ptrColl = GetComponent<CollisionObb>();
			// MiningObjectGroupを取得
			auto group = GetStage()->GetSharedObjectGroup(L"MiningObject");
			// MiningObjectGroup同士の衝突判定をオフ
			ptrColl->SetExcludeCollisionGroup(group);

			// フラグをfalseに変更
			m_setFlg = true;
		}
	}

	void Station::OnReset(){
	
	}
}