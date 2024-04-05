#pragma once
#include "stdafx.h"

namespace basecross
{
	struct CSVElementCheck
	{
		size_t row = 0;
		size_t col = 0;
		bool isRange = false;

		CSVElementCheck(size_t elemRow, size_t elemCol, bool range) :
			row(elemRow),
			col(elemCol),
			isRange(range)
		{
		}
	};

	class RailManager : public GameObject
	{
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw;
		vector<vector<int>> m_guideMap;

	public:

		RailManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
		}

		virtual ~RailManager() {}

		void OnCreate() override;

		void AddRail(const Vec3& addPos);

		void SetRailID(size_t i, size_t j) const;

		void SetGuideID(size_t i, size_t j);

		vector<vector<int>> GetGuideMap() const
		{
			return m_guideMap;
		}
	};
}