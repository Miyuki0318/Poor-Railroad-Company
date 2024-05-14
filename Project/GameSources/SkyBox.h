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
		shared_ptr<MainCamera> m_camera;

	public:
		SkyBox(const shared_ptr<Stage>& stagePtr, const shared_ptr<MainCamera>& camera) :
			TemplateObject(stagePtr, Vec3(camera->GetEye()), Vec3(0.0f), Vec3(2.0f)),
			m_camera(camera)
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
