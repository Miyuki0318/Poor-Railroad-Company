/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"
#include "BaseStage.h"

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

	/*!
	@brief ゲーム中のステージ
	*/
	class GameStage : public BaseStage
	{
		// ゲームクリア・ゲームオーバーのスプライト
		shared_ptr<Sprite> m_gameSprite;

		// ゲームの状況
		eGameProgress m_gameProgress;

		// ゲームクリアからタイトルステージへの遷移までの猶予
		const float m_defermentTransition;

		// ゲームクリアからの経過時間カウント用変数
		float m_countTime;

		/*!
		@brief リソースの読込
		*/
		void CreateResourses() override;

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
		void CreateStageCSV(string csvPath) override;

		/*!
		@brief 線路生成
		*/
		void CreateRailManager();

		/*!
		@brief 木の足場生成
		*/
		void CreateBridgeManager();

		/*!
		@brief 踏切の生成
		*/
		void CreateCrossingManager();

		/*!
		@brief 採集オブジェクトの生成
		*/
		void CreateGatheringManager();

		/*!
		@brief 歯車オブジェクトの生成
		*/
		void CreateGearManager();

		/*!
		@brief 列車生成
		*/
		void CreateTrain();

		/*!
		@brief スプライトの生成
		*/
		void CreateSpriteObject();

		/*!
		@brief UIの生成
		*/
		void CreateUIObject();

		/*!
		@brief スプライトの表示
		*/
		void LogoActive();

		/*!
		@brief タイトルステージ遷移用の処理
		*/
		void ToTitleStage();


	public:

		/*!
		@brief コンストラクタ
		*/
		GameStage(const string stagePath) :
			BaseStage(stagePath),
			m_defermentTransition(1.0f)
		{
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

		/*!
		@brief 破棄される時に一度だけ呼び出される関数
		*/
		virtual void OnDestroy() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;

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