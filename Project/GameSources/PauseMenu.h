#pragma once
#include "stdafx.h"

namespace basecross
{
	class PauseMenu : public GameObject
	{
		enum State {
			Open,	// オープン
			Close,	// クローズ
			Opened, // オープン後
			Closed	// クローズ後
		};
		enum eButtons {
			Retry,
			BackTitle
		};
		State m_state;	// 現在の状態
		eButtons m_currentButton;
		eButtons m_pastButton;

		const Vec3 m_CloseMenuPos;	 // クローズ時メニュー位置
		const Vec3 m_OpenMenuPos;	 // オープン時メニュー位置
		const Vec2 m_CloseMenuScale; // クローズ時サイズ
		const Vec2 m_OpenMenuScale;	 // オープン時サイズ

		const Vec3 m_DefaultRetryButtonPos; // リトライボタン表示位置
		const Vec3 m_DefaultTitleButtonPos; // タイトルボタン表示位置
		const Vec2 m_DefaultButtonScale;	// ボタンサイズ

		const float m_LerpSpeed; // 線形補間速度
		const float m_ScaleSpeed; // 大きさ変更速度
		const float m_MaxScaleRatio; 
		const float m_MinScaleRatio;

		float m_lerpRatio;	// 線形補間割合
		float m_scaleRatio; // 大きさ変更割合
		float m_pastStickVal;

		weak_ptr<Sprite> m_menuSprite;		// メニュー画像
		weak_ptr<Sprite> m_retrySprite;		// リトライボタン画像
		weak_ptr<Sprite> m_titleBackSprite;	// タイトルボタン画像
		map<eButtons, weak_ptr<Sprite>> m_buttonSprites; // ボタン用スプライト

		/// <summary>
		/// 状態ごとの処理
		/// </summary>
		/// <param name="state">現在の状態</param>
		void StateProcess(State state); 

	public:
		PauseMenu(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_CloseMenuPos(Vec3(1920.0f, 1080.0f, 0.0f)),
			m_OpenMenuPos(Vec3(0.0f)),
			m_CloseMenuScale(Vec2(0.0f)),
			m_OpenMenuScale(Vec2(1920.0f, 1080.0f)),
			m_DefaultRetryButtonPos(Vec3(-350.0f, -380.0f, 0.0f)),
			m_DefaultTitleButtonPos(Vec3(350.0f, -380.0f, 0.0f)),
			m_DefaultButtonScale(Vec2(275.0f, 100.0f)),
			m_LerpSpeed(3.0f),
			m_ScaleSpeed(2.0f),
			m_MaxScaleRatio(1.25f),
			m_MinScaleRatio(1.0f),
			m_state(State::Closed),
			m_currentButton(eButtons::Retry),
			m_pastButton(eButtons::BackTitle),
			m_lerpRatio(0.0f),
			m_scaleRatio(0.0f),
			m_pastStickVal(0.0f)
		{
		}
		~PauseMenu() {}

		void OnCreate() override;
		void OnUpdate() override;

		/// <summary>
		/// メニューオープン時に呼び出す関数
		/// </summary>
		void OnOpen();

		/// <summary>
		/// メニュークローズ時に呼び出す関数
		/// </summary>
		void OnClose();

		/// <summary>
		/// ボタンセレクト中の処理
		/// </summary>
		void ButtonSelect();

		/// <summary>
		/// ボタンセレクト後の処理
		/// </summary>
		void ButtonAction();
	};
}