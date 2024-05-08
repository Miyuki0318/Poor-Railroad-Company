/*!
@file CraftUI.h
@brief クラフトUIの継承元オブジェクト
@author 小澤博貴
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	// ウィンドウが表示されているかenum
	enum class eWindowEnable : uint8_t
	{
		IsEnable,     // 現在表示中か
		IsPastEnable, // 前回の状態
	};

	/*!
	@brief クラフトに関するUIの継承元
	*/
	class CraftUI : public Sprite
	{
	protected:

		// 表示中か
		Bool8_t<eWindowEnable> m_drawEnable;

		// UV座標マップ
		map<eRectType, vector<Vec2>> m_rectUVMap;

		// 描画方向
		eRectType m_rectType;

		const Vec2 m_drawScale;	 // 展開時のスケール
		const float m_drawSpeed; // 展開速度
		float m_drawRatio;		 // 展開割合

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param テクスチャキー
		@param 展開時のスケール
		@param 展開速度
		*/
		CraftUI(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr,
			const Vec2& drawScale,
			float drawSpeed
		) :
			Sprite(stagePtr, texWstr, Vec2(0.0f), Vec3(0.0f)),
			m_drawScale(drawScale),
			m_drawSpeed(drawSpeed)
		{
			m_drawRatio = 0.0f;
			m_drawEnable = 0;

			m_rectType = eRectType::UpLeft;

			m_rectUVMap.emplace(eRectType::UpRight, vector<Vec2>({ Vec2(0.0f, 0.0f), Vec2(0.5f, 0.0f), Vec2(0.0f, 0.5f), Vec2(0.5f, 0.5f) }));
			m_rectUVMap.emplace(eRectType::UpLeft, vector<Vec2>({ Vec2(0.5f, 0.0f), Vec2(1.0f, 0.0f), Vec2(0.5f, 0.5f), Vec2(1.0f, 0.5f) }));
			m_rectUVMap.emplace(eRectType::DownRight, vector<Vec2>({ Vec2(0.0f, 0.5f), Vec2(0.5f, 0.5f), Vec2(0.0f, 1.0f), Vec2(0.5f, 1.0f) }));
			m_rectUVMap.emplace(eRectType::DownLeft, vector<Vec2>({ Vec2(0.5f, 0.5f), Vec2(1.0f, 0.5f), Vec2(0.5f, 1.0f), Vec2(1.0f, 1.0f) }));
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~CraftUI() {}

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief ウィンドウサイズの更新関数
		*/
		virtual void DrawWindow();

		/*!
		@brief 表示非表示設定関数
		@param bool
		*/
		virtual void SetEnable(bool enable);

		/*!
		@brief 表示非表示設定関数
		@param bool
		*/
		virtual void SetDrawEnable(bool enable);

		/*!
		@brief ウィンドウ描画が完了したかの真偽取得関数
		@return 完了してたらtrue、それ以外はfalse
		*/
		virtual bool GetShowWindow() const
		{
			return m_drawEnable(eWindowEnable::IsEnable, eWindowEnable::IsPastEnable);
		}

		/*!
		@brief 描画変更設定関数
		@param eVerticesRect
		*/
		virtual void SetRectType(eRectType rect)
		{
			m_rectType = rect;
		}
	};
}