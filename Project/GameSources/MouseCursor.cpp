/*!
@file MouseCursor.cpp
@brief キーマウ操作時のマウスカーソル
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Input;

	// 生成時の処理
	void MouseCursor::OnCreate()
	{
		// 継承元の生成時の処理
		Sprite::OnCreate();

		// レイヤー設定
		SetDrawLayer(1);
	}

	// 更新処理
	void MouseCursor::OnUpdate()
	{
		// コントローラー接続があるか
		bool isController = GetPadConected();
		SetDrawActive(!isController);
		if (isController) return;

		// マウス座標を取得
		Vec3 windowPos = Vec3(GetMousePosition());

		// ウィンドウサイズからスケールを引いた分で範囲を設定
		Vec3 windowRange = Vec3((WINDOW_SIZE - Vec2(m_scale)) / 2.0f);
		windowPos.clamp(-windowRange, windowRange);

		// 座標の更新
		SetPosition(windowPos);
	}
}