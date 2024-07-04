/*!
@file CraftUI.cpp
@brief クラフトUIの継承元オブジェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "CraftUI.h"

namespace basecross
{
	// 毎フレーム更新処理
	void CraftUI::OnUpdate()
	{
		// 前回までの状態と違ったらウィンドウの更新
		if (m_drawEnable(eWindowEnable::IsEnable) != m_drawEnable(eWindowEnable::IsPastEnable))
		{
			DrawWindow();
		}
	}

	// ウィンドウの更新
	void CraftUI::DrawWindow()
	{
		// 表示状態を変数化
		const bool isEnable = m_drawEnable(eWindowEnable::IsEnable);

		// 展開割合を追加
		m_drawRatio += (isEnable ? DELTA_TIME : -DELTA_TIME) / m_drawSpeed;
		m_drawRatio = min(m_drawRatio, 1.0f);
		m_drawRatio = max(m_drawRatio, 0.0f);

		// スケールをLerpで設定
		Vec2 scale = Utility::Lerp(Vec2(0.0f), m_drawScale, m_drawRatio);
		SetScale(scale);

		// 始点or終点になったのなら状態を揃える
		if (m_drawRatio == 1.0f || m_drawRatio == 0.0f)
		{
			m_drawEnable.Set(eWindowEnable::IsPastEnable) = isEnable;
		}
	}

	// 状態切り替え
	void CraftUI::SetEnable(bool enable)
	{
		// 表示状態を設定
		m_drawEnable.Set(eWindowEnable::IsEnable) = enable;
		m_drawEnable.Set(eWindowEnable::IsPastEnable) = !enable;
	}

	// 表示切り替え
	void CraftUI::SetDrawEnable(bool enable)
	{
		// 切替る
		SetEnable(enable);
	}

	// 表示切り替え
	void CraftUI::SetDrawEnable(bool enable, const Vec3& position)
	{
		SetEnable(enable);

		// 座標を更新
		SetPosition(position);
	}
}