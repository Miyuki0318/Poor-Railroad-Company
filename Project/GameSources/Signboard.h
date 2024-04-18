/*!
@file Signboard.h
@brief ŠÅ”Â(l”‘I‘ğ)‚Ìİ’è
@author VÈˆ¨
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class SignBoard : public TemplateObject
	{
		const float m_hitLength = 2.5f;

		const Vec3 m_scale = Vec3(3.0f);
		const Vec3 m_position = Vec3(47.0f, 2.5f, 10.0f);

		Mat4x4 m_spanMat;

		Vec3 m_playerPosition;
		Vec3 m_cameraPosition;
		Vec3 m_cameraAt;

	public :
		bool pushButton;

		SignBoard(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
			m_spanMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);
			pushButton = false;
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void PushButton();
		bool DistanceToPlayer();

		bool GetPushButton()
		{
			return pushButton;
		}
	};
}