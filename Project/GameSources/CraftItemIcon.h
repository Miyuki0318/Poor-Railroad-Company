/*!
@file CraftItemIcon.h
@brief クラフトするアイテムアイコン
@author 小澤博貴
*/

#pragma once
#include "CraftUI.h"

namespace basecross
{
	/*!
	@brief クラフトアイテムアイコン
	*/
	class CraftItemIcon : public CraftUI
	{
		bool m_craftPosshible; // クラフトできるかの真偽

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param テクスチャキー
		*/
		CraftItemIcon(const shared_ptr<Stage>& stagePtr,
			const wstring texture
		) :
			CraftUI(stagePtr, texture, Vec2(200.0f), 0.25f)
		{
			m_craftPosshible = false;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~CraftItemIcon() {}

		/*!
		@brief 生成時の処理関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief クラフトできるかの真偽設定関数
		@param クラフトできるか
		*/
		void SetCraftPosshible(bool posshible)
		{
			m_craftPosshible = posshible;
		}
	};
}