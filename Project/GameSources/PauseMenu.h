#pragma once
#include "stdafx.h"

namespace basecross
{
	class PauseMenu : public GameObject
	{
		enum State {
			None,
			Open,
			Close
		};
		State m_state;

		const Vec3 m_DefaultPosition;
		const Vec2 m_DefaultScale;
		const Vec2 m_AfterScale;
		const float m_ScaleSpeed;

		float m_scaleRatio;

		weak_ptr<Sprite> m_menuSprites;

	public:
		PauseMenu(const shared_ptr<Stage>& stagePtr, const Vec3& position) :
			GameObject(stagePtr),
			m_DefaultPosition(position),
			m_DefaultScale(Vec2(0.0f)),
			m_AfterScale(Vec2(1920.0f, 1080.0f)),
			m_ScaleSpeed(3.0f),
			m_state(State::None),
			m_scaleRatio(0.0f)
		{
		}
		~PauseMenu() {}

		void OnCreate() override;
		void OnUpdate() override;

		void OnOpen();
		void OnClose();

		void StateProcess(State state);
	};
}