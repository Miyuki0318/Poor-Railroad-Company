/*!
@file CraftPosshibleGuide.h
@brief クラフト可能になった時に出るエフェクト
@author 小澤博貴
*/

#pragma once
#include "SpriteParticle.h"
#include "GamePlayer.h"
#include "Bool_t.h"

namespace basecross
{
	/*!
	@brief クラフト可能時のエフェクト
	*/
	class CraftPosshibleGuide : public TemplateObject
	{
		weak_ptr<SpriteParticle> m_effect;// パーティクル
		weak_ptr<GamePlayer> m_playerPtr;	// プレイヤー
		weak_ptr<SoundItem> m_seItem;		// SE
		map<eCraftItem, Vec3> m_uiPosMap;	// UIの座標マップ
		Bool8_t<eCraftItem> m_posshible;	// クラフト可能か

		const float m_drawTime;	// 表示時間
		const Vec2 m_effScale;	// スケール

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		CraftPosshibleGuide(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_drawTime(1.0f),
			m_effScale(200.0f)
		{
			m_posshible = 0;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~CraftPosshibleGuide() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief クラフト可能になったかどうかのチェック関数
		@param 現在のフラグ
		@param クラフトするアイテム
		*/
		void CheckPosshible(Bool8_t<eCraftItem>& flag, eCraftItem type);

		/*!
		@brief エフェクト開始関数
		@param クラフトするアイテム
		*/
		void StartEffect(eCraftItem type);

		/*!
		@brief エフェクトの生成時用UI座標設定関数
		@param クラフトするアイテム
		@param UIの座標
		*/
		void SetUIPosition(eCraftItem type, Vec3 position)
		{
			m_uiPosMap.emplace(type, position);
		}
	}; 
}