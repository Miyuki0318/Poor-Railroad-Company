/*!
@file Signboard.h
@brief ŠÅ”Â(l”‘I‘ð)‚ÌÝ’è
@author VÈˆ¨
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class SignBoard : public TemplateObject
	{
		const float m_hitLength = 2.5f;

		const Vec3 m_scale = Vec3(3.0f);
		const Vec3 m_position = Vec3(47.0f,1.0f, 10.0f);

		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();

		Mat4x4 m_spanMat;

		Vec3 m_playerPosition;
		Vec3 m_cameraPosition;
		Vec3 m_cameraAt;

	public :
		SignBoard(const shared_ptr<Stage>& stagePtr) :
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

		void BoardSelect();
	};
}