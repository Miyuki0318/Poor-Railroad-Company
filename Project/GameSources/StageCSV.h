#pragma once
#include "stdafx.h"

#define STAGE_ID(id) static_cast<eStageID>(id)

namespace basecross
{
	// ステージオブジェクトID
	enum class eStageID
	{
		None,		// 何もなし
		Rail,		// レール
		DeRail,		// 脱線部分
		GuideRail,	// ガイドレール
		GoalRail,	// ゴールレール
		Rock = 10,	// 岩
		Tree,		// 木
		Water = -1,	// 水
	};

	class StageCSV : public Stage
	{
	protected:

		// ステージマップ
		vector<vector<int>> m_stageMap;
		vector<vector<Vec3>> m_positionMap;

		/*!
		@brief ステージをcsvで生成
		@param csvのファイル名
		*/
		virtual void CreateStageCSV(string csvPath = "TestStage") = 0;

		/*!
		@brief ステージマップをcsvで書き換え
		@param csvのファイル名
		*/
		void WriteCSVMap(string csvPath = "TestStage")
		{
			m_stageMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath));

			// 二重ループ
			for (int i = 0; i < m_stageMap.size(); i++)
			{
				vector<Vec3> tempVec;
				for (int j = 0; j < m_stageMap.at(i).size(); j++)
				{
					tempVec.push_back(Vec3(float(j), 0.0f, -float(i)));
				}
				m_positionMap.push_back(tempVec);
			}
		}

	public:

		/*!
		@brief コンストラクタ
		*/
		StageCSV() : Stage() {}

		/*!
		@brief デストラクタ
		*/
		virtual ~StageCSV() {}

		/*!
		@brief ステージマップの取得
		@return m_stageMap
		*/
		vector<vector<int>>& GetStageMap()
		{
			return m_stageMap;
		}

		/*!
		@brief ステージマップの取得
		@return m_stageMap
		*/
		vector<vector<Vec3>>& GetStagePosMap()
		{
			return m_positionMap;
		}
	};
}