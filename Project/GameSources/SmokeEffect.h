/*!
@file SmokeEffect.h
@brief 列車から出る煙のエフェクト
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief 煙のエフェクト
	*/
	class SmokeEffect : public MultiParticle
	{
		const float m_drawTime;		// 描画時間
		const float m_scale;		// スケール
		const float m_upVelo;		// 上方向への移動量
		const float m_veloRange;	// XとZ軸の移動量の範囲

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		SmokeEffect(const shared_ptr<Stage>& stagePtr) :
			MultiParticle(stagePtr),
			m_scale(1.0f),
			m_drawTime(1.5f),
			m_upVelo(6.0f),
			m_veloRange(0.5f)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~SmokeEffect() {}

		/*!
		@brief 煙のエフェクト生成関係
		*/
		void AddSmokeEffect();
	};
}