/*!
@file RouteMap.h
@brief ˜Hü}(“ïˆÕ“x‘I‘ğ)‚Ìİ’è
@author VÈˆ¨
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class RouteMap : public TemplateObject
	{
		const float m_hitLength = 2.5f;

		const Vec3 m_scale = Vec3(3.0f);
		const Vec3 m_position = Vec3(55.0f, 1.0f, 10.0f);

		Mat4x4 m_spanMat;

		Vec3 m_playerPosition;
		Vec3 m_cameraPosition;
		Vec3 m_cameraAt;

		bool m_isSelect;

	public:
		RouteMap(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
			m_spanMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void SetSelectFlag(bool flag)
		{
			m_isSelect = flag;
		}
	};
}