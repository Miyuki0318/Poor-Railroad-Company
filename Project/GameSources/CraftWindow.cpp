/*!
@file CraftWindow.cpp
@brief クラフトウィンドウ
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void CraftWindow::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Sprite::OnCreate();

		// 非表示に
		SetDrawActive(false);
	}

	// 毎フレーム更新処理
	void CraftWindow::OnUpdate()
	{
		// 前回までの状態と違ったらウィンドウの更新
		if (m_enable(eWindowEnable::IsEnable) != m_enable(eWindowEnable::IsPastEnable))
		{
			UpdateWindow();
		}
	}

	// ウィンドウの更新
	void CraftWindow::UpdateWindow()
	{
		// 表示状態を変数化
		const bool& isEnable = m_enable(eWindowEnable::IsEnable);

		// 展開割合を追加
		m_showRatio += (isEnable ? DELTA_TIME : -DELTA_TIME) * m_showSpeed;
		m_showRatio = min(m_showRatio, 1.0f);
		m_showRatio = max(m_showRatio, 0.0f);

		// スケールをLerpで設定
		Vec2 scale = Utility::Lerp(Vec2(0.0f), m_showScale, m_showRatio);
		SetScale(scale);

		// 始点or終点になったのなら状態を揃える
		if (m_showRatio == 1.0f || m_showRatio == 0.0f)
		{
			SetDrawActive(isEnable);
			m_enable.Set(eWindowEnable::IsPastEnable) = isEnable;
		}
	}

	// 表示切り替え
	void CraftWindow::SetDrawEnable(bool b)
	{
		m_enable.Set(eWindowEnable::IsEnable) = b;
	}

	// 位置設定
	void CraftWindow::SetWindowRect(eWindowRect rect)
	{
		// 頂点データのリセット
		Utility::SimpleVertices(m_vertex.vertices);

		// 加算量用変数
		const float HELF = 0.5f;
		Vec3 addPos;

		// 向きに応じて設定
		switch (rect)
		{
		case eWindowRect::UpperRight: // 右上なら
			addPos = Vec2(HELF, HELF);
			break;

		case eWindowRect::UpperLeft: // 左上なら
			addPos = Vec2(-HELF, HELF);
			break;

		case eWindowRect::UnderRight: // 右下なら
			addPos = Vec2(HELF, -HELF);
			break;
		
		case eWindowRect::UnderLeft: // 左下なら
			addPos = Vec2(-HELF, -HELF);
			break;
		
		default:
			break;
		}

		// 頂点データの座標を加算
		for (auto& v : m_vertex.vertices)
		{
			v.position += addPos;
		}

		// 頂点データの更新
		m_ptrDraw->UpdateVertices(m_vertex.vertices);
	}
}