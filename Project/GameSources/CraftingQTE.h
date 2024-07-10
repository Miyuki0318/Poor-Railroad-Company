/*!
@file CraftingQTE.h
@brief クラフト中のQTE
@author 小澤博貴
*/

#pragma once
#include "CraftUI.h"

namespace basecross
{
	// QTE入力するボタン
	enum class eInputButton
	{
		ButtonB, // ボタンB
		ButtonA, // ボタンA
		ButtonY, // ボタンY
	};

	/*!
	@brief クラフトQTE
	*/
	class CraftingQTE : public CraftUI
	{
		weak_ptr<CraftUI> m_barFlame;	// バーのフレーム
		weak_ptr<CraftUI> m_qtePoint;	// QTEのタイミング範囲
		weak_ptr<CraftUI> m_qteButton;	// QTEで入力するボタン
		weak_ptr<CraftUI> m_qteBack;	// QTEの背景

		map<eRectType, Vec3> m_rectDiff;
		map<eInputButton, wstring> m_buttonTexMap;
		map<eInputButton, Col4> m_buttonBarColor;

		const float m_posDiff;  // ウィンドウとの座標差分
		const float m_qteRatio; // QTEの成功位置の割合
		const float m_qteRange;	// QTEの範囲
		const float m_barSpeed; // QTEバーの速度
		float m_barRatio; // QTEバーの割合
		bool m_qteEnable; // QTE中かの真偽

	public:

		/*!
		@brief コンストラクタ
		*/
		CraftingQTE(const shared_ptr<Stage>& stagePtr) :
			CraftUI(stagePtr, L"WHITE_TX", Vec2(300.0f, 15.0f), 0.25f),
			m_posDiff(195.0f),
			m_qteRatio(0.75f),
			m_qteRange(0.1f),
			m_barSpeed(1.0f)
		{
			m_barRatio = 0.0f;
			m_qteEnable = false;

			m_rectDiff.emplace(eRectType::UpRight, Vec3(1.0f, 1.0f, 1.0f));
			m_rectDiff.emplace(eRectType::UpLeft, Vec3(-1.0f, 1.0f, 1.0f));
			m_rectDiff.emplace(eRectType::DownRight, Vec3(1.0f, -1.0f, 1.0f));
			m_rectDiff.emplace(eRectType::DownLeft, Vec3(-1.0f, -1.0f, 1.0f));

			m_buttonTexMap.emplace(eInputButton::ButtonB, L"BUTTON_B_TX");
			m_buttonTexMap.emplace(eInputButton::ButtonA, L"BUTTON_A_TX");
			m_buttonTexMap.emplace(eInputButton::ButtonY, L"BUTTON_Y_TX");

			m_buttonBarColor.emplace(eInputButton::ButtonB, Col4(1.0f, 0.23f, 0.18f, 1.0f));
			m_buttonBarColor.emplace(eInputButton::ButtonA, Col4(0.0f, 1.0f, 0.6f, 1.0f));
			m_buttonBarColor.emplace(eInputButton::ButtonY, Col4(0.9f, 0.84f, 0.0f, 1.0f));
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~CraftingQTE() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief ウィンドウサイズの更新関数
		*/
		void DrawWindow() override;

		/*!
		@brief 表示非表示設定関数
		@param bool
		@param windowの座標
		*/
		void SetDrawEnable(bool enable, Vec3 windowPos);

		/*!
		@brief 描画変更設定関数
		@param eRectType
		*/
		void SetVerticesRect(eRectType rect) override;
		
		/*!
		@brief QTEの更新関数
		*/
		void UpdateQTE();

		/*!
		@brief QTE開始関数
		@param クラフトウィンドウの座標
		*/
		void StartQTE(const Vec3& windowPos);

		/*!
		@brief QTE停止とQTE結果取得関数
		@return 成功したかの真偽
		*/
		bool StopQTE();

		/*!
		@brief QTE中かの真偽取得関数
		*/
		bool GetEnableQTE() const
		{
			return m_qteEnable;
		}

		/*!
		@brief QTE入力するボタンテクスチャ設定関数
		@param ボタン入力タイプ
		*/
		void SetButtonTexture(eInputButton button)
		{
			SetDiffuseColor(m_buttonBarColor.at(button));
			m_qteButton.lock()->SetTexture(m_buttonTexMap.at(button));
		}
	};
}