#pragma once
#include "stdafx.h"

#define STAGE_ID(id) static_cast<eStageID>(id)

namespace basecross
{
	class StageCSV : public Stage
	{
	protected:

		// ステージマップ
		vector<vector<int>> m_stageMap;

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
	};
}