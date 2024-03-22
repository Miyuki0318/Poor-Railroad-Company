/*!
@file Train.cpp
@brief 列車の実装
@author 矢吹悠葉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void Train::OnCreate()
	{
		TemplateObject::OnCreate();
		SetTransParam();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_BLUE);

		// コリジョンOBBの追加
		m_ptrColl = AddComponent<CollisionObb>();

		// タグの設定
		AddTag(L"Train");
	}

	void Train::OnUpdate()
	{

	}
}