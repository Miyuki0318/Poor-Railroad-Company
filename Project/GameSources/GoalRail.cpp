/*!
@file Rail.cpp
@brief ゴール地点オブジェクトの実装
@author 小宅碧
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void GoalRail::OnCreate()
	{
		TemplateObject::OnCreate();
		SetTransParam();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_BLACK);

		// コリジョンOBBの追加
		m_ptrColl = AddComponent<CollisionObb>();
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		// タグの設定
		AddTag(L"Rail");

		// Stationオブジェクトの生成
		const auto ptrTrans = GetComponent<Transform>();
		Vec3 pos = ptrTrans->GetPosition();
		GetStage()->AddGameObject<Station>(Vec3(pos.x, pos.y, pos.z + 1.0f));
	}

	void GoalRail::OnUpdate()
	{
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

		Debug::Log(L"ゲームの状態は", GetTypeStage<GameStage>()->GetGameProgress());

	}

	void GoalRail::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		// Trainとの衝突をチェック
		if (Other->FindTag(L"Train")) {
			// ゴール後の処理を送る
			GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::GameClear);
		}
	}
}