/*!
@file BaseStage.h
@brief 継承元のステージオブジェクト
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

// ステージIDキャストマクロ
#define STAGE_ID(id) static_cast<eStageID>(id)

namespace basecross
{
	// ステージオブジェクトID
	enum class eStageID
	{
		None,			// 何もなし
		Rail,			// レール
		DeRail,			// 脱線部分
		GuideRail,		// ガイドレール
		GoalRail,		// ゴールレール
		Stone = 10,		// 岩
		Tree,			// 木
		Grass = 101,	// 地面の草
		Rock = 102,		// 地面の石
		Air = 111,		// 空気(なんもなし)
		Water = 112,	// 水場
	};

	/*!
	@brief ステージの継承元
	*/
	class BaseStage : public Stage
	{
	protected:

		// ステージマップ
		vector<vector<int>> m_stageMap;
		vector<vector<int>> m_groundMap;
		vector<vector<Vec3>> m_positionMap;

		// SEマネージャー
		unique_ptr<SEManager> m_seManager;

		// タイマーオブジェクト
		weak_ptr<Timer> m_timer;

		/*!
		@brief ステージをcsvで生成
		@param csvのファイル名
		*/
		virtual void CreateStageCSV(string csvPath = "Test") = 0;

		/*!
		@brief ステージマップをcsvで書き換え
		@param csvのファイル名
		*/
		void WriteCSVMap(string csvPath = "Test");

	public:

		/*!
		@brief コンストラクタ
		*/
		BaseStage() : Stage() {}

		/*!
		@brief デストラクタ
		*/
		virtual ~BaseStage() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief 破棄される時に一度だけ呼び出される関数
		*/
		virtual void OnDestroy() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief SEマネージャーの生成関数
		*/
		virtual void CreateSEManager();

		/*!
		@brief SEの再生関数
		@param SEキー
		@param 音量
		*/
		virtual void CreateSE(const wstring& seKey, float volume);

		/*!
		@brief SEの再生関数
		@param SEキー
		@param 音量
		@param オブジェクトのポインタ
		*/
		virtual void CreateSE(const wstring& seKey, float volume, const void* objPtr);

		/*!
		@brief SEの停止関数
		@param SEキー
		*/
		virtual void StopSE(const wstring& seKey);

		/*!
		@brief SEの停止関数
		@param SEキー
		@param オブジェクトのポインタ
		*/
		virtual void StopSE(const wstring& seKey, const void* objPtr);

		/*!
		@brief タイマークラス取得関数
		@return const shared_ptr<Timer>
		*/
		const shared_ptr<Timer> GetTimer() const
		{
			return m_timer.lock();
		}

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