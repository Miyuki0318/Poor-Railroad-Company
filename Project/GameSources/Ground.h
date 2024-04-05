/*!
@file Ground.h
@brief 床オブジェクト
@author	小澤博貴
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief 床ボックス
	*/
	class GroundBox : public TemplateObject
	{
		shared_ptr<PNTStaticDraw> m_ptrDraw; // 描画コンポーネント

	public:

		/*!
		@brief コンストラクタ
		@param ステージのポインタ
		@param スケール
		*/
		GroundBox(const shared_ptr<Stage>& stagePtr,
			const Vec3& scale
		) :
			TemplateObject(stagePtr, Vec3(49.5f, 0.0f, 7.0f), Vec3(0.0f), scale)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~GroundBox() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;
	};
}