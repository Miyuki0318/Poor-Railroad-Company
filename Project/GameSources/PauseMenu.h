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
		const Vec3 m_AfterPosition;
		const Vec2 m_DefaultScale;
		const Vec2 m_AfterScale;
		const float m_LerpSpeed;

		float m_lerpRatio;

		weak_ptr<Sprite> m_menuSprites;

		void StateProcess(State state);

	public:
		PauseMenu(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_DefaultPosition(Vec3(1920.0f, 1080.0f, 0.0f)),
			m_AfterPosition(Vec3(0.0f)),
			m_DefaultScale(Vec2(0.0f)),
			m_AfterScale(Vec2(1920.0f, 1080.0f)),
			m_LerpSpeed(3.0f),
			m_state(State::None),
			m_lerpRatio(0.0f)
		{
		}
		~PauseMenu() {}

		void OnCreate() override;
		void OnUpdate() override;

		void OnOpen();
		void OnClose();

	};
}