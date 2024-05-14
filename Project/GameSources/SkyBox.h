/*!
@file SkyBox.h
@brief スカイボックスオブジェクト
@author 矢吹悠葉
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {
	class SkyBox : public TemplateObject
	{
		shared_ptr<PNTStaticDraw> m_ptrDraw;

	public:
		SkyBox(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(3.0f, -5.0f, -7.0f), Vec3(0.0f), Vec3(2.0f))
		{
		}
		~SkyBox() {}

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
