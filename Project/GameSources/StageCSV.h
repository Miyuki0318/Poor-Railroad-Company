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
		Stone = 10,	// 岩
		Tree,		// 木

		Grass = 101,	// 地面の草
		Rock = 102,		// 地面の石
		Air = 111,		// 空気(なんもなし)
		Water = 112,	// 水場
	};

	class StageCSV : public Stage
	{
	protected:

		// ステージマップ
		vector<vector<int>> m_stageMap;
		vector<vector<int>> m_groundMap;
		vector<vector<Vec3>> m_positionMap;

		/*!
		@brief ステージをcsvで生成
		@param csvのファイル名
		*/
		virtual void CreateStageCSV(string csvPath = "Test") = 0;

		/*!
		@brief ステージマップをcsvで書き換え
		@param csvのファイル名
		*/
		void WriteCSVMap(string csvPath = "Test")
		{
			m_stageMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Stage"));
			m_groundMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Ground"));

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
		@brief グラウンドマップの取得
		@return m_groundMap
		*/
		vector<vector<int>>& GetGroundMap()
		{
			return m_groundMap;
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