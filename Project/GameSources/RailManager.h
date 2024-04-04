#pragma once
#include "stdafx.h"

namespace basecross
{
	class RailManager : public GameObject
	{
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw;

	public:

		RailManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
		}

		virtual ~RailManager() {}

		void OnCreate() override;

		void AddRail(const Vec3& addPos);
	};
}