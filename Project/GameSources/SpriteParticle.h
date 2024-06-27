/*!
@file SpriteParticle.h
@brief スプライトでのパーティクル
@author 小澤博貴
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	/*!
	@brief スプライトパーティクルデータ構造体
	*/
	struct ParticleData
	{
		float totalTime = 0.0f;		// 経過時間
		float drawTime = 0.0f;		// 描画時間
		weak_ptr<Sprite> sprite;	// スプライトオブジェクト
	};

	/*!
	@brief スプライトパーティクル
	*/
	class SpriteParticle : public GameObject
	{
		// パーティクル配列
		vector<ParticleData> m_spriteVec;
		
		Vec3 m_emitterPosition;		// エミッター
		wstring m_textureStr;		// テクスチャ
		const size_t m_maxSprite;	// 上限数

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param テクスチャキー
		@param 上限数(デフォルトは60)
		*/
		SpriteParticle(const shared_ptr<Stage>& stagePtr,
			const wstring& textureStr,
			const size_t& maxSprite = 60
		) :
			GameObject(stagePtr),
			m_textureStr(textureStr),
			m_maxSprite(maxSprite)
		{
			m_spriteVec.reserve(m_maxSprite);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~SpriteParticle() {}

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief パーティクル追加関数
		@param スケール
		@param 移動量
		@param ローテーションZ軸
		@param 描画時間
		*/
		void AddParticle(Vec2 scale, Vec2 velo, float rotZ, float drawTime);

		/*!
		@brief エミッター座標設定関数
		@param position
		*/
		void SetEmitterPosition(Vec3 position)
		{
			m_emitterPosition = position;
		}
	};
}