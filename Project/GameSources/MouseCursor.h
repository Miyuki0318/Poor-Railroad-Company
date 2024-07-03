/*!
@file MouseCursor.h
@brief キーマウ操作時のマウスカーソル
@author 小澤博貴
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	/*!
	@brief マウスカーソル
	*/
	class MouseCursor : public Sprite
	{
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		MouseCursor(const shared_ptr<Stage>& stagePtr) :
			Sprite(stagePtr, L"CURSOR_TX", Vec2(50.0f))
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~MouseCursor() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;
	};
}