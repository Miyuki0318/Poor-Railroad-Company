/*!
@file GameOverState.h
@brief ゲームオーバー時の処理ステート
@author 小澤博貴
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	// 失敗時のステートenum
	enum class eGameOverState : char
	{
		RailFadeIn,		// レールのフェード
		SelectFadeIn,	// 選択肢のフェードイン
		SelectState,	// ステージ選択
		SelectFadeOut,	// 選択肢のフェードアウト
		StandBy,		// 待機状態
	};

	// 選択したタイプ
	enum class eSelectGameOver : char
	{
		Continue,	// コンティニュー
		TitleBack,	// タイトルに戻る
	};

	// ゲームステージの名前宣言
	class GameStage;

	/*!
	@brief ゲームオーバー時のステート
	*/
	class GameOverState
	{
		// 所属ステージポインタ
		weak_ptr<GameStage> m_stage;

		// クリア時の選択肢スプライト
		weak_ptr<Sprite> m_continueSprite;	// コンティニュー
		weak_ptr<Sprite> m_titleBackSprite;	// タイトルバック
		weak_ptr<Sprite> m_railLineSprite;	// レール

		weak_ptr<SpriteParticle> m_smoke; // 煙のエフェクト

		const Vec2 m_defScale;	// スケール
		const Vec3 m_leftPos;	// 選択肢左の座標
		const Vec3 m_rightPos;	// 選択肢右の座標
		const Vec3 m_railPos;	// レールの座標
		const Vec3 m_moveVal;	// 動く量
		const Vec3 m_smokeDiff;	// 煙の出る位置
		const Vec2 m_smokeVelo;	// 煙の移動量

		const float m_boundScale;		// バウンド時のスケール
		const float m_railFadeTime;		// レールのフェード時間
		const float m_selectFadeTime;	// 選択肢のフェード時間

		float m_totalTime;	// 経過時間
		float m_pastStick;	// 前回のスティック入力

		eSelectGameOver m_currentSelect;	// 選択したステージ
		eSelectGameOver m_pastSelect;		// 前回選択したステージ
		eGameOverState m_currentState;		// ステート

		// ステートに応じた関数呼び出しマップ
		map<eGameOverState, function<void()>> m_stateFunc;

		// 選択肢に応じたスプライトマップ
		map<eSelectGameOver, weak_ptr<Sprite>> m_selectSprite;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		GameOverState(const shared_ptr<GameStage>& stagePtr) :
			m_stage(stagePtr),
			m_boundScale(1.25f),
			m_railFadeTime(3.0f),
			m_selectFadeTime(5.0f),
			m_defScale(300.0f, 350.0f),
			m_leftPos(-300.0f, -200.0f, 0.0f),
			m_rightPos(300.0f, -200.0f, 0.0f),
			m_railPos(1920.0f, -250.0f, 0.2f),
			m_moveVal(1920.0f, 0.0f, 0.0f),
			m_smokeDiff(-93.0f, 140.0f, 0.0f),
			m_smokeVelo(3.0f)
		{
			m_totalTime = 0.0f;
			m_pastStick = 0.0f;

			m_currentSelect = eSelectGameOver::Continue;
			m_pastSelect = eSelectGameOver::TitleBack;

			m_currentState = eGameOverState::RailFadeIn;
			m_stateFunc.emplace(eGameOverState::RailFadeIn, bind(&GameOverState::RailSpriteFadeIn, this));
			m_stateFunc.emplace(eGameOverState::SelectFadeIn, bind(&GameOverState::SelectSpriteFadeIn, this));
			m_stateFunc.emplace(eGameOverState::SelectState, bind(&GameOverState::SelectStageState, this));
			m_stateFunc.emplace(eGameOverState::SelectFadeOut, bind(&GameOverState::SelectSpriteFadeOut, this));
		}

		/*!
		@brief デストラクタ
		*/
		~GameOverState() {}

		/*!
		@brief スプライトの生成関数
		*/
		void CreateState();

		/*!
		@brief ステートの更新処理関数
		*/
		void UpdateState();

		/*!
		@brief ステートの初期化関数
		*/
		void ResetState();

		/*!
		@brief ステート取得関数
		@return m_gameClearState
		*/
		eGameOverState GetState() const
		{
			return m_currentState;
		}

		/*!
		@brief 選択したステージ取得関数
		@return m_selectStage
		*/
		eSelectGameOver GetSelectStage() const
		{
			return m_currentSelect;
		}

	private:

		/*!
		@brief レールスプライトのフェードイン処理
		*/
		void RailSpriteFadeIn();

		/*!
		@brief 選択肢プライトのフェードイン処理
		*/
		void SelectSpriteFadeIn();

		/*!
		@brief ステージ選択中の処理
		*/
		void SelectStageState();

		/*!
		@brief コントローラーでの選択
		*/
		void ControllerSelect();

		/*!
		@brief マウスでの選択
		*/
		void MouseSelect();

		/*!
		@brief 選択肢プライトのフェードイン処理
		*/
		void SelectSpriteFadeOut();
	};
}