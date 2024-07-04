/*!
@file CraftItemIcon.cpp
@brief クラフトするアイテムアイコン
@author 小澤博貴
*/

#include "stdafx.h"
#include "CraftItemIcon.h"

namespace basecross
{
	// 更新処理
	void CraftItemIcon::OnUpdate()
	{
		// 継承元の更新処理
		CraftUI::OnUpdate();

		// 色をクラフトできるかで設定
		SetDiffuseColor(m_craftPosshible ? COL_WHITE : COL_GRAY);
	}
}