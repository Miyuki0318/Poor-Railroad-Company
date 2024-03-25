/*!
@file Tarminal.cpp
@brief 線路終着の実装
@author 矢吹悠葉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void Tarminal::OnCreate()
	{
		TemplateObject::OnCreate();
		SetTransParam();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_GRAY);

		// コリジョンOBBの追加
		m_ptrColl = AddComponent<CollisionObb>();
		m_ptrColl->SetFixed(true);

		// タグの設定
		AddTag(L"Tarminal");
	}

	void Tarminal::OnUpdate()
	{

	}
}