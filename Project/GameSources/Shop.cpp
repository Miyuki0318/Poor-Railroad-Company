/*!
@file Shop.cpp
@brief ショップ機能の実装
@author 小宅碧
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Shop::OnCreate()
	{
		// ステートマシンの初期化
		m_ShopState.reset(new ShopStateMachine(GetThis<Shop>()));
		// 最初のステートを設定
		m_ShopState->SetState(ShopCloseState::Instance());



	}

	void Shop::OnUpdate()
	{
		// ステートマシンの更新処理を送る
		m_ShopState->Update();
	}
}