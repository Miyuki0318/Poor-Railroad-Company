/*!
@file Building.h
@brief 建物の設定
@author 新妻葵
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class Company : public TemplateObject
	{
		const Vec3 m_position;

	public:
		Company::Company(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const Vec3 position
		) :
			TemplateObject(stagePtr),
			m_position(position)
		{
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}