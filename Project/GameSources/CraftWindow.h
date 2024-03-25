/*!
@file CraftWindow.h
@brief クラフトウィンドウ
@author 小澤博貴
*/
#pragma once
#include "Sprite.h"

namespace basecross
{
	// ウィンドウ描画方向enum
	enum class eWindowRect
	{
		UpperRight,	// 右上
		UpperLeft,	// 左上
		UnderRight,	// 右下
		UnderLeft,	// 左下
	};

	// ウィンドウが表示されているかenum
	enum class eWindowEnable : uint8_t
	{
		IsEnable,     // 現在表示中か
		IsPastEnable, // 前回の状態
	};

	/*!
	@brief クラフトウィンドウ
	*/
	class CraftWindow : public Sprite
	{
		// 表示中か
		Bool8_t<eWindowEnable> m_enable;

		const Vec2 m_showScale;	 // 展開時のスケール
		const float m_showSpeed; // 展開速度
		float m_showRatio;		 // 展開割合

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		CraftWindow(const shared_ptr<Stage>& stagePtr) :
			Sprite(stagePtr, L"CWINDOW_TX", Vec2(0.0f), Vec3(0.0f)),
			m_showScale(Vec2(200.0f)),
			m_showSpeed(0.25f)
		{
			m_enable = 0;
			m_showRatio = 0.0f;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~CraftWindow() {}

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
		void UpdateWindow();

		/*!
		@brief 表示非表示設定関数
		@param bool
		*/
		void SetDrawEnable(bool enable);

		/*!
		@brief 描画位置設定関数
		@param eWindowRect
		*/
		void SetWindowRect(eWindowRect rect);

		/*!
		@brief ウィンドウ描画が完了したかの真偽取得関数
		@return 完了してたらtrue、それ以外はfalse
		*/
		bool GetShowWindow() const
		{
			return m_enable(eWindowEnable::IsEnable, eWindowEnable::IsPastEnable);
		}
	};
}