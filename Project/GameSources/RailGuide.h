#pragma once
#include "TemplateObject.h"

namespace basecross
{
	class RailGuide : public TemplateObject
	{
	public:

		RailGuide(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f), Vec3(0.0f), Vec3(1.0f, 0.2f, 1.0f))
		{
		}

		virtual ~RailGuide() {}

		void OnCreate() override;

		void OnUpdate() override;
	};
}