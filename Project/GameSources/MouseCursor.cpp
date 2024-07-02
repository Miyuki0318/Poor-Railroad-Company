#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	using namespace Input;

	void MouseCursor::OnUpdate()
	{
		bool notConected = !GetPadConected();
		SetDrawActive(notConected);
		if (notConected) return;

		auto& keyState = GetKeyboard();

		Vec3 windowPos;
		windowPos.x = (float)keyState.m_MouseClientPoint.x;
		windowPos.y = (float)keyState.m_MouseClientPoint.y;

		Vec3 windowRange = Vec3((WINDOW_SIZE - Vec2(m_scale)) / 2.0f);
		windowPos.clamp(-windowRange, windowRange);
		SetPosition(windowPos);
	}
}