/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"
#include "BaseStage.h"
#include "GameClearState.h"
#include "GameOverState.h"
#include "PaymentsState.h"

namespace basecross 
{
	/*!
	@brief ゲームの進行状況
	*/
	enum eGameProgress
	{
		FadeIn,				// 開始時のフェードイン
		Playing,			// プレイ中
		CraftPause,			// クラフト中のポーズ
		Pause,				// ポーズ
		GoalConect,			// ゴールと接続完了
		GameClear,			// ゲームクリア
		MoneyCalculation,	// 精算処理
		MoneyCountDown,		// 精算結果表示
		ClearSelect,		// クリア後の選択
		ToNext,				// 次のステージへ
		ToTitleClear,		// クリア時のタイトルステージへ
		ToTitleOver,		// 失敗時のタイトルステージへ
		DeRailed,			// 脱線時
		GameOver,			// ゲームオーバー
		ContinueFadeIn,		// コンティニュー時のフェードイン
		ContinueFadeOut,	// コンティニュー時のフェードアウト
	};

	/*!
	@brief ゲーム中のステージ
	*/
	class GameStage : public BaseStage
	{
		weak_ptr<SoundItem> m_bgmItem; // BGMのサウンドアイテム

		// ゲームクリア・ゲームオーバーのスプライト
		shared_ptr<Sprite> m_gameSprite;

		// コンティニュー時のスプライト
		shared_ptr<Sprite> m_continueSprite;	// コンティニュー
		shared_ptr<Sprite> m_titleBackSprite;	// タイトルバック
		const Vec2 m_defScale;
		const Vec3 m_leftPos;
		const Vec3 m_rightPos;

		// クリア時の処理
		unique_ptr<GameClearState> m_gameClearState;
		unique_ptr<GameOverState> m_gameOverState;
		unique_ptr<PaymentsState> m_paymentsState;

		// ゲームの状況
		eGameProgress m_gameProgress;

		// ゲームクリアからタイトルステージへの遷移までの猶予
		const float m_defermentTransition;

		// ゲームクリアからの経過時間カウント用変数
		float m_countTime;
		float m_totalTime;

		// コンティニュー時のスティック入力
		float m_pastStickVal;

		Vec3 m_startPosition;
		Vec3 m_goalStagingPosition;

		map<eGameProgress, function<void()>> m_progressFunc;

		/*!
		@brief リソースの読込
		*/
		void CreateResourses() override;

		/*!
		@brief BGMの再生
		*/
		void CreateStartBGM();

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
		@brief 地面をcsvで再生成
		*/
		void ResetGroundStage();

		/*!
		@brief カメラの再生成
		*/
		void ResetCameraObject();

		/*!
		@brief スプライトの表示
		*/
		void LogoActive();
	
		void ToFadeInState();

		void ToPlayingState();

		void ToMoneyCalculationState();

		void ToMoneyCountDownState();

		void ToClearSelectStage();

		void ToNextStage();

		/*!
		@brief タイトルステージ遷移用の処理
		*/
		void ToTitleStage();

		void PushButtonStart();

		/*!
		@brief コンティニュー時の処理
		*/
		void ToGameOverStage();

		void ToContinueFadeIn();
		void ToContinueFadeOut();

		void ResetState();
		void TitleBackState();

	public:

		/*!
		@brief コンストラクタ
		*/
		GameStage(const string stagePath) :
			BaseStage(stagePath),
			m_defermentTransition(3.0f),
			m_defScale(275.0f, 100.0f),
			m_leftPos(-300.0f, -200.0f, 0.0f),
			m_rightPos(300.0f, -200.0f, 0.0f)
		{
			m_countTime = 0.0f;
			m_totalTime = 0.0f;
			m_pastStickVal = 0.0f;
			m_gameProgress = eGameProgress::FadeIn;

			m_progressFunc.emplace(eGameProgress::FadeIn, bind(&GameStage::ToFadeInState, this));
			m_progressFunc.emplace(eGameProgress::Playing, bind(&GameStage::ToPlayingState, this));
			m_progressFunc.emplace(eGameProgress::MoneyCalculation, bind(&GameStage::ToMoneyCalculationState, this));
			m_progressFunc.emplace(eGameProgress::MoneyCountDown, bind(&GameStage::ToMoneyCountDownState, this));
			m_progressFunc.emplace(eGameProgress::ClearSelect, bind(&GameStage::ToClearSelectStage, this));
			m_progressFunc.emplace(eGameProgress::ToNext, bind(&GameStage::ToNextStage, this));
			m_progressFunc.emplace(eGameProgress::ToTitleClear, bind(&GameStage::ToTitleStage, this));
			m_progressFunc.emplace(eGameProgress::ToTitleOver, bind(&GameStage::ToTitleStage, this));
			m_progressFunc.emplace(eGameProgress::GameOver, bind(&GameStage::ToGameOverStage, this));
			m_progressFunc.emplace(eGameProgress::ContinueFadeIn, bind(&GameStage::ToContinueFadeIn, this));
			m_progressFunc.emplace(eGameProgress::ContinueFadeOut, bind(&GameStage::ToContinueFadeOut, this));
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~GameStage() 
		{
			m_gameClearState.reset();
			m_gameOverState.reset();
			m_paymentsState.reset();
		}

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
		@brief ステージをcsvで再生成
		*/
		void ResetCreateStage();

		bool GetFadeIn() const
		{
			return m_fadeSprite->FadeInColor(2.0f);
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