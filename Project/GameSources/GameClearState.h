/*!
@file GameClearState.h
@brief ゲームクリア時の処理ステート
@author 小澤博貴
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	// クリア時のステートenum
	enum class eGameClearState : char
	{
		RailFadeIn,		// レールのフェード
		SelectFadeIn,	// 選択肢のフェードイン
		SelectStage,	// ステージ選択
		SelectFadeOut,	// 選択肢のフェードアウト
		StandBy,		// 待機状態
	};

	// 選択したステージタイプ
	enum class eSelectStage : char
	{
		NextStage,	// 次のステージ
		TitleBack,	// タイトルステージ
	};

	// ゲームステージの名前宣言
	class GameStage;

	/*!
	@brief ゲームクリア時のステート
	*/
	class GameClearState
	{
		// 所属ステージポインタ
		weak_ptr<GameStage> m_stage;

		// クリア時の選択肢スプライト
		weak_ptr<Sprite> m_nextStageSprite;	// ネクストステージ
		weak_ptr<Sprite> m_clearBackSprite;	// クリア時のタイトルバック
		weak_ptr<Sprite> m_railLineSprite;	// クリア時のタイトルバック

		const Vec2 m_defScale;	// スケール
		const Vec3 m_leftPos;	// 選択肢左の座標
		const Vec3 m_rightPos;	// 選択肢右の座標
		const Vec3 m_railPos;	// レールの座標
		const Vec3 m_moveVal;	// 動く量

		const float m_boundScale;		// バウンド時のスケール
		const float m_railFadeTime;		// レールのフェード時間
		const float m_selectFadeTime;	// 選択肢のフェード時間

		float m_totalTime;	// 経過時間
		float m_pastStick;	// 前回のスティック入力
		
		eSelectStage m_selectStage; // 選択したステージ
		eSelectStage m_pastSelect;	// 前回選択したステージ
		eGameClearState m_clearState; // ステート

		// ステートに応じた関数呼び出しマップ
		map<eGameClearState, function<void()>> m_stateFunc;

		// 選択肢に応じたスプライトマップ
		map<eSelectStage, weak_ptr<Sprite>> m_selectSprite;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		GameClearState(const shared_ptr<GameStage>& stagePtr) :
			m_stage(stagePtr),
			m_boundScale(1.25f),
			m_railFadeTime(3.0f),
			m_selectFadeTime(5.0f),
			m_defScale(300.0f, 350.0f),
			m_leftPos(-300.0f, -200.0f, 0.0f),
			m_rightPos(300.0f, -200.0f, 0.0f),
			m_railPos(-1920.0f, -250.0f, 0.2f),
			m_moveVal(1920.0f, 0.0f, 0.0f)
		{
			m_totalTime = 0.0f;
			m_pastStick = 0.0f;

			m_selectStage = eSelectStage::NextStage;
			m_pastSelect = eSelectStage::TitleBack;

			m_clearState = eGameClearState::RailFadeIn;
			m_stateFunc.emplace(eGameClearState::RailFadeIn, bind(&GameClearState::RailSpriteFadeIn, this));
			m_stateFunc.emplace(eGameClearState::SelectFadeIn, bind(&GameClearState::SelectSpriteFadeIn, this));
			m_stateFunc.emplace(eGameClearState::SelectStage, bind(&GameClearState::SelectStageState, this));
			m_stateFunc.emplace(eGameClearState::SelectFadeOut, bind(&GameClearState::SelectSpriteFadeOut, this));
		}

		/*!
		@brief デストラクタ
		*/
		~GameClearState() {}

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
		@return m_clearState
		*/
		eGameClearState GetClearState() const
		{
			return m_clearState;
		}

		/*!
		@brief 選択したステージ取得関数
		@return m_selectStage
		*/
		eSelectStage GetSelectStage() const
		{
			return m_selectStage;
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
		@brief 選択肢プライトのフェードイン処理
		*/
		void SelectSpriteFadeOut();
	};
}