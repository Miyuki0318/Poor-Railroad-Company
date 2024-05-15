/*!
@file SkyBox.cpp
@brief スカイボックスの実装
@author 矢吹悠葉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void SkyBox::OnCreate()
	{
		TemplateObject::OnCreate();
		SetTransParam();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<BcPNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"SKY");
		m_ptrDraw->SetTextureResource(L"SKY_TX");
		m_ptrDraw->SetDiffuse(COL_WHITE);
		m_ptrDraw->SetLightingEnabled(false);

		// タグの設定
		AddTag(L"SkyBox");
	}

	void SkyBox::OnUpdate()
	{
		SetPosition(m_camera->GetEye());
	}
}