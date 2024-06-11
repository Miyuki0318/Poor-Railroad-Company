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
		FadeIn,
		StartSE,
		Playing,
		GameClear,
		GameOver,
		ContinueFade,
	};

	// コンティニュー処理中のステートenum
	enum class eContinueState
	{
		FadeIn,
		SelectFade,
		Selecting,
		SelectState,
		FadeOut,
	};

	// コンティニュー時の選択肢
	enum class eContinueSelect
	{
		Continue,
		TitleBack,
	};

	/*!
	@brief ゲーム中のステージ
	*/
	class GameStage : public BaseStage
	{
		// ゲームクリア・ゲームオーバーのスプライト
		shared_ptr<Sprite> m_gameSprite;

		// コンティニュー時のスプライト
		shared_ptr<Sprite> m_ctSprite;	// コンティニュー
		shared_ptr<Sprite> m_tbSprite;	// タイトルバック
		const Vec2 m_defScale;

		// ゲームの状況
		eGameProgress m_gameProgress;

		// コンティニュー処理のステート
		eContinueState m_continueState;

		// コンティニュー時の選択肢
		eContinueSelect m_currentState;
		eContinueSelect m_pastState;

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
		map<eContinueState, function<void()>> m_continueFunc;
		map<eContinueSelect, weak_ptr<Sprite>> m_selectMap;
		map<eContinueSelect, function<void()>> m_selectFunc;

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
		@brief スプライトの表示
		*/
		void LogoActive();

		void ToFadeInState();

		//void ToStartSEState();

		/*!
		@brief タイトルステージ遷移用の処理
		*/
		void ToTitleStage();

		/*!
		@brief コンティニュー時の処理
		*/
		void ToContinueStage();

		void ContinueFadeInState();
		void ContinueSelectFadeState();
		void ContinueSelectingState();
		void ContinueSelectState();
		void ContinueFadeOutState();

		void ResetState();
		void TitleBackState();

	public:

		/*!
		@brief コンストラクタ
		*/
		GameStage(const string stagePath) :
			BaseStage(stagePath),
			m_defermentTransition(3.0f),
			m_defScale(275.0f, 100.0f)
		{
			m_countTime = 0.0f;
			m_totalTime = 0.0f;
			m_pastStickVal = 0.0f;
			m_gameProgress = eGameProgress::FadeIn;
			m_continueState = eContinueState::FadeIn;
			m_currentState = eContinueSelect::Continue;
			m_pastState = eContinueSelect::TitleBack;

			m_progressFunc.emplace(eGameProgress::FadeIn, bind(&GameStage::ToFadeInState, this));
			//m_progressFunc.emplace(eGameProgress::StartSE, bind(&GameStage::ToStartSEState, this));
			m_progressFunc.emplace(eGameProgress::GameClear, bind(&GameStage::ToTitleStage, this));
			m_progressFunc.emplace(eGameProgress::GameOver, bind(&GameStage::ToContinueStage, this));
			m_progressFunc.emplace(eGameProgress::ContinueFade, bind(&GameStage::ToContinueStage, this));

			m_continueFunc.emplace(eContinueState::FadeIn, bind(&GameStage::ContinueFadeInState, this));
			m_continueFunc.emplace(eContinueState::SelectFade, bind(&GameStage::ContinueSelectFadeState, this));
			m_continueFunc.emplace(eContinueState::Selecting, bind(&GameStage::ContinueSelectingState, this));
			m_continueFunc.emplace(eContinueState::SelectState, bind(&GameStage::ContinueSelectState, this));
			m_continueFunc.emplace(eContinueState::FadeOut, bind(&GameStage::ContinueFadeOutState, this));

			m_selectFunc.emplace(eContinueSelect::Continue, bind(&GameStage::ResetState, this));
			m_selectFunc.emplace(eContinueSelect::TitleBack, bind(&GameStage::TitleBackState, this));
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