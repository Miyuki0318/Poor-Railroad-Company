/*!
@file RailGuideIcon.h
@brief レールを設置する場所のガイド表示
@author 小澤博貴
*/

#pragma once
#include "Billboard.h"

namespace basecross
{
	/*!
	@brief レール設置ガイド
	*/
	class RailGuideIcon : public GameObject
	{
		// アイコン配列
		vector<weak_ptr<Billboard>> m_iconVec;

		// 前回のガイドポイント
		vector<Point2D<size_t>> m_pastGuidePoint;

		// プレイヤーのポインタ
		weak_ptr<TemplateObject> m_playerPtr;

		const Vec2 m_deffScale;		// スケール
		const float m_boundValue;	// バウンドする量
		const float m_drawRange;	// 表示する距離
		const float m_deffPosY;		// デフォルトの座標Y

		float m_totalTime;	// 経過時間

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		RailGuideIcon(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_drawRange(3.0f),
			m_deffScale(0.75f),
			m_deffPosY(1.75f),
			m_boundValue(0.5f)
		{
			m_totalTime = 0.0f;

			m_iconVec.resize(3);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~RailGuideIcon() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 目立つ動きの更新処理関数
		*/
		void UpdateIconMove();

		/*!
		@brief ガイドの配置更新処理関数
		*/
		void UpdateGuide();

		/*!
		@brief プレイヤーとの距離で表示するかの更新処理関数
		*/
		void UpdateRange();

		/*!
		@brief ガイドの表示設定関数
		@param 表示するかの真偽
		*/
		void SetIconDraw(bool b);
	};
}