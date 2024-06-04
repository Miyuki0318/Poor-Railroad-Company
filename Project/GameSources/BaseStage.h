/*!
@file BaseStage.h
@brief 継承元のステージオブジェクト
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"
#include "ResourcesManager.h"

// ステージIDキャストマクロ
#define STAGE_ID(id) static_cast<eStageID>(id)
#define UnSTAGE_ID(id) static_cast<int>(id)

namespace basecross
{
	// ステージオブジェクトID
	enum class eStageID : int
	{
		None,				// 何もなし
		Rail,				// レール
		DeRail,				// 脱線部分
		GuideRail,			// ガイドレール
		GoalRail,			// ゴールレール
		CrossingOpen,		// 踏切(通れる)
		CrossingCross,		// 踏切(通れない)
		Stone1 = 10,		// 岩0
		Stone2,				// 岩1
		Stone3,				// 岩2
		Tree1,				// 木0
		Tree2,				// 木1
		UnBreakRock = 20,	// 壊せない岩
		Water = 31,			// 水
		WoodBridge = 40,	// 木の足場	
		Grass = 101,		// 地面の草
		Sand = 102,			// 地面の砂
		Rock = 103,			// 地面の石
		Air = 111,			// 空気(なんもなし)
		Watering = 112,		// 水場
		UnGrass = 121,		// 描画のみ通過不可
	};

	/*!
	@brief ステージの継承元
	*/
	class BaseStage : public Stage, public ResourcesManager
	{
	protected:

		// ステージマップ
		vector<vector<int>> m_stageMap;
		vector<vector<int>> m_groundMap;
		vector<vector<Vec3>> m_positionMap;

		// SEマネージャー
		unique_ptr<SoundManager> m_soundManager;

		// タイマーオブジェクト
		weak_ptr<Timer> m_timer;

		// ステージのファイル内
		const string m_stagePath;

		/*!
		@brief リソースの読込
		*/
		virtual void CreateResourses();

		/*!
		@brief リソースの解放
		*/
		virtual void ReleasedResourses();

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
		BaseStage(const string stagePath) : Stage(), m_stagePath(stagePath) {}

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
		@brief 描画更新関数
		*/
		virtual void OnDraw() override;

		/*!
		@brief SEマネージャーの生成関数
		*/
		virtual void CreateSoundManager();

		/*!
		@brief SEの再生関数
		@param SEキー
		@param 音量
		*/
		virtual shared_ptr<SoundItem> CreateSE(const wstring& seKey, float volume);

		/*!
		@brief SEの再生関数
		@param SEキー
		@param 音量
		@param オブジェクトのポインタ
		*/
		virtual shared_ptr<SoundItem> CreateSE(const wstring& seKey, float volume, const void* objPtr);

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