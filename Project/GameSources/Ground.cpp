/*!
@file Ground.cpp
@brief 床オブジェクト
@author	小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void GroundBox::OnCreate()
	{
		// 継承元の生成時の処理を実行
		TemplateObject::OnCreate();
		SetTransParam();

		// メッシュの設定
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetTextureResource(L"GROUND_TX");
		m_ptrDraw->SetOwnShadowActive(true);

		// コリジョンコンポーネントを追加
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true); // 剛体処理をアクティブ
	}
}