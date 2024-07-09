/*!
@file PauseMenu.h
@brief ポーズメニュー
@prod 矢吹悠葉
*/

#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "OzawaUtility.h"

namespace basecross
{
	class PauseMenu : public GameObject
	{
		enum State {
			Open,	// オープン
			Close,	// クローズ
			Opened, // オープン後
			Closed,	// クローズ後
			Selected // ボタンセレクト後
		};
		enum eMenuTypes {
			KeyBoard, // キーボード/マウス
			XBoxPad,  // パッド
			MenuNum
		};
		enum eButtons {
			Retry,		// リトライ
			BackTitle,	// タイトルに戻る
			ButtonNum
		};
		State m_state;	// 現在の状態
		eMenuTypes m_currentMenuType; // 現フレームのメニュータイプ
		eMenuTypes m_pastMenuType;	  // 前フレームのメニュータイプ
		eButtons m_currentButton;	  // 現フレームで選択中のボタン
		eButtons m_pastButton;		  // 前フレームで選択していたボタン

		const Vec3 m_CloseMenuPos;	 // クローズ時メニュー位置
		const Vec3 m_OpenMenuPos;	 // オープン時メニュー位置
		const Vec2 m_CloseMenuScale; // クローズ時サイズ
		const Vec2 m_OpenMenuScale;	 // オープン時サイズ

		const Vec3 m_DefaultRetryButtonPos; // リトライボタン表示位置
		const Vec3 m_DefaultTitleButtonPos; // タイトルボタン表示位置
		const Vec2 m_DefaultButtonScale;	// ボタンサイズ

		const int m_MenuLayerNum;	// メニュー描画レイヤー番号
		const int m_ButtonLayerNum;	// ボタン描画レイヤー番号

		const float m_LerpSpeed;		// 線形補間速度
		const float m_ButtonScaleSpeed;	// 大きさ変更速度
		const float m_MaxScaleRatio;	// 大きさ変更上限
		const float m_MinScaleRatio;	// 大きさ変更下限

		float m_lerpRatio;	  // 線形補間割合
		float m_scaleRatio;   // 大きさ変更割合
		float m_pastStickVal; // 直前のLスティックの量

		map<eMenuTypes, weak_ptr<Sprite>> m_menuSprites; // メニュー画像
		map<eButtons, weak_ptr<Sprite>> m_buttonSprites; // ボタン用スプライト

		/// <summary>
		/// 状態ごとの処理
		/// </summary>
		/// <param name="state">現在の状態</param>
		void StateProcess(State state); 

		/// <summary>
		/// メニュータイプ決定処理
		/// </summary>
		void MenuTypeDecision();

		/// <summary>
		/// メニュー描画切り替え
		/// </summary>
		/// <param name="drawFlag"></param>
		/// <param name="mType"></param>
		void SetDrawActiveMenu(bool drawFlag, eMenuTypes mType);

	public:
		PauseMenu(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_CloseMenuPos(Vec3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f)),
			m_OpenMenuPos(Vec3(0.0f, 0.0f, 0.0f)),
			m_CloseMenuScale(Vec2(0.0f)),
			m_OpenMenuScale(Vec2(WINDOW_WIDTH, WINDOW_HEIGHT)),
			m_DefaultRetryButtonPos(Vec3(-350.0f, -380.0f, 0.0f)),
			m_DefaultTitleButtonPos(Vec3(350.0f, -380.0f, 0.0f)),
			m_DefaultButtonScale(Vec2(275.0f, 100.0f)),
			m_MenuLayerNum(8),
			m_ButtonLayerNum(9),
			m_LerpSpeed(3.0f),
			m_ButtonScaleSpeed(2.0f),
			m_MaxScaleRatio(1.25f),
			m_MinScaleRatio(1.0f),
			m_state(State::Closed),
			m_currentMenuType(eMenuTypes::KeyBoard),
			m_pastMenuType(eMenuTypes::XBoxPad),
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
		bool OnOpen();

		/// <summary>
		/// メニュークローズ時に呼び出す関数
		/// </summary>
		bool OnClose();

		/// <summary>
		/// ボタンセレクト中の処理	
		/// </summary>
		void ButtonSelect();

		/// <summary>
		/// タイトルボタンセレクト後の処理
		/// </summary>
		void TitleButton();

		/// <summary>
		/// リトライボタンセレクト後の処理
		/// </summary>
		void RetryButton();

		/// <summary>
		/// ボタンのDrawActiveを切り替える処理
		/// </summary>
		/// <param name="drawFlag">DrawActiveのオンオフ</param>
		void SetDrawActiveButtons(bool drawFlag);
	};
}