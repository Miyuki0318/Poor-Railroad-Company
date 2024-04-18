/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

#define STAGE_ID(id) static_cast<eStageID>(id)

namespace basecross 
{
	/*!
	@brief ゲームの進行状況
	*/
	enum eGameProgress
	{
		Playing,
		GameClear,
		GameOver
	};

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

	/*!
	@brief ゲーム中のステージ
	*/
	class GameStage : public Stage
	{
		// ゲームクリア・ゲームオーバーのスプライト
		shared_ptr<Sprite> m_gameClearLogo;
		shared_ptr<Sprite> m_gameOverLogo;

		// ゲームの状況
		eGameProgress m_gameProgress;

		// タイマーオブジェクト
		weak_ptr<Timer> m_timer;

		// ステージマップ
		vector<vector<int>> m_stageMap;

		/*!
		@brief リソースの読込
		*/
		void CreateResourses();

		/*!
		@brief ビューとライトの生成
		*/
		void CreateViewLight();

		/*!
		@brief ステージボックスの生成
		*/
		void CreateGroundBox();

		/*!
		@brief プレイヤー生成
		*/
		void CreatePlayer();

		/*!
		@brief ステージをcsvで生成
		@param csvのファイル名
		*/
		void CreateStageCSV(string csvPath = "TestStage");

		/*!
		@brief 採掘系オブジェクトの生成生成
		*/
		void CreateStageObject();

		/*!
		@brief 線路生成
		*/
		void CreateRailManager();

		/*!
		@briefゴール地点の生成
		*/
		void CreateGoalRail();

		/*!
		@brief 列車生成
		*/
		void CreateTrain();

		/*!
		@brief 線路の終着生成
		*/
		void CreateTarminal();

		/*!
		@brief スプライトの生成
		*/
		void CreateSpriteObject();

		/*!
		@brief スプライトの表示
		*/
		void LogoActive();

	public:

		/*!
		@brief コンストラクタ
		*/
		GameStage() :Stage() {
			m_gameProgress = eGameProgress::Playing;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~GameStage() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		///*!
		//@brief 破棄される時に一度だけ呼び出される関数
		//*/
		//virtual void OnDestroy() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief 描画更新関数
		*/
		virtual void OnDraw() override;

		/*!
		@brief ステージマップの取得
		@return m_stageMap
		*/
		vector<vector<int>>& GetStageMap()
		{
			return m_stageMap;
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
		@brief ゲームの進行状態をゲットする関数
		*/
		eGameProgress GetGameProgress() const
		{
			return m_gameProgress;
		}

		/*!
		@brief ゲームの進行状態をセットする関数
		*/
		void SetGameProgress(eGameProgress progress)
		{
			m_gameProgress = progress;
		}
	};
}