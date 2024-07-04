#pragma once
#include "Billboard.h"

namespace basecross
{
	class RailGuideIcon : public GameObject
	{
		vector<weak_ptr<Billboard>> m_iconVec;

		// 前回のガイドポイント
		vector<Point2D<size_t>> m_pastGuidePoint;

		const Vec2 m_deffScale;
		const float m_boundValue;
		const float m_drawRange;
		const float m_deffPosY;

		float m_totalTime;

	public:

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
		@brief 点滅の更新処理関数
		*/
		void UpdateIconMove();

		/*!
		@brief ガイドの配置更新処理関数
		*/
		void UpdateGuide();

		/*!
		@brief ガイドの表示設定関数
		@param 表示するかの真偽
		*/
		void SetIconDraw(bool b);
	};
}