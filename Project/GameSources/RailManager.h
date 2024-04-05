#pragma once
#include "stdafx.h"

namespace basecross
{
	// CSVのチェック用構造体
	struct CSVElementCheck
	{
		size_t row = 0;	// 列
		size_t col = 0; // 行
		bool isRange = false; // 配列の範囲内かの真偽


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

		vector<CSVElementCheck> GetMapElems(size_t i, size_t j, const vector<vector<int>>& csvMap) const
		{
			vector<CSVElementCheck> elems;
			if (csvMap.empty()) return elems;

			elems = {
				{i - 1, j, Utility::GetBetween(i - 1, 0, csvMap.size())},
				{i + 1, j, Utility::GetBetween(i + 1, 0, csvMap.size())},
				{i, j - 1, Utility::GetBetween(j - 1, 0, csvMap.at(i).size())},
				{i, j + 1, Utility::GetBetween(i + 1, 0, csvMap.at(i).size())},
			};

			return elems;
		}
	};
}