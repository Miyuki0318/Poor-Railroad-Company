/*!
@file CraftWindow.cpp
@brief クラフトウィンドウ
@author 小澤博貴
*/

#include "stdafx.h"
#include "CraftWindow.h"

namespace basecross
{
	// 生成時の処理
	void CraftWindow::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Sprite::OnCreate();

		// 描画モードを変更
		m_ptrDraw->SetRasterizerState(RasterizerState::DoubleDraw);
	}

	// 毎フレーム更新処理
	void CraftWindow::OnUpdate()
	{
		// 継承元の毎フレーム更新処理を実行
		CraftUI::OnUpdate();
	}
}