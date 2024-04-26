/*!
@file OriginalCollision.h
@brief オリジナルのコリジョン
@author 新妻葵
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class OriginalColl : public TemplateObject
	{
	private:
		const Vec3 m_scale;
		const Vec3 m_pos;

	public:
		OriginalColl(shared_ptr<Stage>& ptrStage,
			const Vec3 scale,
			const Vec3 pos
		):
			TemplateObject(ptrStage),
			m_scale(scale),
			m_pos(pos)
		{
		}

		virtual void OnCreate() override;
	};
}