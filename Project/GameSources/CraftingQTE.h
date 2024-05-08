/*!
@file CraftingQTE.h
@brief クラフト中のQTE
@author 小澤博貴
*/

#pragma once
#include "CraftUI.h"

namespace basecross
{
	/*!
	@brief クラフトQTE
	*/
	class CraftingQTE : public CraftUI
	{
		weak_ptr<CraftUI> m_barFlame; // バーのフレーム
		weak_ptr<CraftUI> m_qtePoint; // QTEのタイミング範囲

		map<eRectType, Vec3> m_rectDiff;

		const float m_posDiff;  // ウィンドウとの座標差分
		const float m_qteRatio; // QTEの成功位置の割合
		const float m_barSpeed; // QTEバーの速度
		float m_barRatio; // QTEバーの割合
		bool m_qteEnable; // QTE中かの真偽

	public:

		/*!
		@brief コンストラクタ
		*/
		CraftingQTE(const shared_ptr<Stage>& stagePtr) :
			CraftUI(stagePtr, L"WHITE_TX", Vec2(200.0f, 10.0f), 0.25f),
			m_posDiff(130.0f),
			m_qteRatio(0.75f),
			m_barSpeed(1.0f)
		{
			m_barRatio = 0.0f;
			m_qteEnable = false;

			m_rectDiff.emplace(eRectType::UpRight, Vec3(1.0f, 1.0f, 1.0f));
			m_rectDiff.emplace(eRectType::UpLeft, Vec3(-1.0f, 1.0f, 1.0f));
			m_rectDiff.emplace(eRectType::DownRight, Vec3(1.0f, -1.0f, 1.0f));
			m_rectDiff.emplace(eRectType::DownLeft, Vec3(-1.0f, -1.0f, 1.0f));
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
		*/
		void SetDrawEnable(bool enable, Vec3 windowPos);

		/*!
		@brief 描画変更設定関数
		@param eVerticesRect
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
	};
}