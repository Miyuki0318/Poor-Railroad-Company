#pragma once
#include "stdafx.h"

namespace basecross
{
	class PauseMenu : public GameObject
	{
		const Vec3 m_DefaultPosition;
		const Vec2 m_DefaultScale;

		weak_ptr<Sprite> m_menuSprites;

	public:
		PauseMenu(const shared_ptr<Stage>& stagePtr, const Vec3& position) :
			GameObject(stagePtr),
			m_DefaultPosition(position),
			m_DefaultScale(Vec2(1920.0f, 1080.0f))
		{
		}
		~PauseMenu() {}

		void OnCreate() override;
		void OnUpdate() override;
	};
}