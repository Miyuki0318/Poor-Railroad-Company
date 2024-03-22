/*!
@file Rail.cpp
@brief 線路の実装
@author 矢吹悠葉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void Rail::OnCreate()
	{
		TemplateObject::OnCreate();
		SetTransParam();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_GRAY);

		// コリジョンOBBの追加
		m_ptrColl = AddComponent<CollisionObb>();

		// タグの設定
		AddTag(L"Rail");
	}

	void Rail::OnUpdate()
	{

	}
}