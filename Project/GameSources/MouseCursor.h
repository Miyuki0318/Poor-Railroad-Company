#pragma once
#include "Sprite.h"

namespace basecross
{
	class MouseCursor : public Sprite
	{
	public:

		MouseCursor(const shared_ptr<Stage>& stagePtr) :
			Sprite(stagePtr, L"CURSOR_TX", Vec2(50.0f))
		{
		}

		virtual ~MouseCursor() {}

		void OnUpdate() override;
	};
}