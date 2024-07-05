/*!
@file BaseStage.h
@brief 継承元のステージオブジェクト
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"
#include "ResourcesManager.h"
#include "SoundManager.h"
#include "Timer.h"
#include "Scene.h"
#include "Sprite.h"

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
		StartRail,			// 開始レール
		DrawRail,			// 描画のみのレール
		Stone1 = 10,		// 岩0
		Stone2,				// 岩1
		Stone3,				// 岩2
		Tree1,				// 木0
		Tree2,				// 木1
		UnBreakRock = 20,	// 壊せない岩
		CrossingOpen = 22,	// 踏切(通れる)
		CrossingCross,		// 踏切(通れない)
		Gear = 25,			// ギア
		Water = 31,			// 水
		WoodBridge = 40,	// 木の足場	
		PlayerStart = 50,	// プレイヤーの開始位置
		PlayerGoal,			// プレイヤーのゴール演出位置
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

		// フェード用スプライト
		shared_ptr<Sprite> m_fadeSprite;

		// ステージのファイル内
		string m_stagePath;

		bool m_isStaging;
		float m_stageDistanceX;
		const float m_bgmVolume;

		/*!
		@brief リソースの読込
		*/
		virtual void CreateResourses();

		/*!
		@brief リソースの解放
		*/
		virtual void ReleasedResourses();

		/*
		@brief フェード用スプライトの生成
		*/
		virtual void CreateFadeSprite();

		/*!
		@brief ステージをcsvで生成
		@param csvのファイル名
		*/
		virtual void CreateStageCSV(string csvPath) = 0;

		/*!
		@brief ステージマップをcsvで書き換え
		@param csvのファイル名
		*/
		void WriteCSVMap(string csvPath);

	public:

		/*!
		@brief コンストラクタ
		*/
		BaseStage(const string stagePath) : 
			Stage(), 
			m_stagePath(stagePath),
			m_bgmVolume(0.5f)
		{
			m_isStaging = false;
			m_stageDistanceX = 0.0f;
		}

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
		@brief 演出中かの真偽取得関数
		@return m_isStaging
		*/
		bool GetIsStaging() const
		{
			return m_isStaging;
		}

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

		/*!
		@brief お金の取得
		@return money
		*/
		int GetMoney() const
		{
			// Sceneを取得
			const weak_ptr<Scene> scene = App::GetApp()->GetScene<Scene>();
			// Sceneからmoneyを取得
			int money = scene.lock()->GetMoney();
			// moneyの値を返す
			return money;
		}

		/*!
		@brief お金の加算
		@param[in] 加算する値
		*/
		void AddMoney(int value) const
		{
			// Sceneを取得
			const weak_ptr<Scene> scene = App::GetApp()->GetScene<Scene>();
			// Sceneからmoneyを取得
			int money = scene.lock()->GetMoney();
			//moneyに値を加算
			money += value;
			money = min(money, MONEY_LIMIT);
			// Sceneのmoneyを更新
			scene.lock()->SetMoney(money);
		}

		/*!
		@brief お金の使用
		@param[in] 使用する値
		*/
		void UseMoney(int value) const
		{
			// Sceneを取得
			const weak_ptr<Scene> scene = App::GetApp()->GetScene<Scene>();
			// Sceneからmoneyを取得
			int money = scene.lock()->GetMoney();
			//moneyから値を減算
			money -= value;
			money = max(money, 0);
			// Sceneのmoneyを更新
			scene.lock()->SetMoney(money);
		}
	};
}