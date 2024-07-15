/*!
@file Station.cpp
@brief Tarminalの後方マスに配置される駅オブジェ
@author 小宅碧
*/

#pragma once
#include "stdafx.h"
#include "Station.h"

namespace basecross {
	void Station::OnCreate() {
		//初期位置などの設定
		TemplateObject::OnCreate();
		SetScale(Vec3(3.0f, 3.0f, 3.0f)); // 大きさは3*3マス
		SetRotation(Vec3(0.0f));
		Vec3 hulfScale = GetScale() / 2.0f;
		SetPosition(m_spawnPos.x, m_spawnPos.y + hulfScale.y, m_spawnPos.z);

		//CollisionObb衝突判定を付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);

		// 新規ドローコンポーネントの設定
		auto ptrDraw = AddComponent<BcPNTStaticModelDraw>();
		ptrDraw->SetMeshResource(L"STATION");
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

	}

	void Station::OnReset(const Vec3& position){
		m_spawnPos = position;
		Vec3 hulfScale = GetScale() / 2.0f;
		SetPosition(m_spawnPos.x, m_spawnPos.y + hulfScale.y, m_spawnPos.z);
	}
}