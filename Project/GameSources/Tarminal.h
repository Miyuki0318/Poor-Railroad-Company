/*!
@file Tarminal.h
@brief 線路終着オブジェクト
@author 矢吹悠葉
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {
	class Tarminal : public TemplateObject
	{
		shared_ptr<PNTStaticDraw> m_ptrDraw;
		shared_ptr<CollisionObb> m_ptrColl;

	public:
		Tarminal(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(-1.0f, 1.0f, 0.0f), Vec3(0.0f), Vec3(2.0f, 0.5f, 1.0f))
		{
		}

		Tarminal(const shared_ptr<Stage>& stagePtr, const Vec3& position) :
			TemplateObject(stagePtr, position, Vec3(0.0f), Vec3(1.0f, 0.2f, 1.0f))
		{
		}

		~Tarminal() {}

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
