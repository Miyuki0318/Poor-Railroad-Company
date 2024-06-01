/*!
@file SmokeEffect.cpp
@brief 列車から出る煙のエフェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 煙のエフェクト生成処理
	void SmokeEffect::AddSmokeEffect()
	{
		auto& ptrTrans = GetComponent<Transform>();

		// パーティクルを生成
		auto particlePtr = InsertParticle(2);
		particlePtr->SetEmitterPos(ptrTrans->GetWorldPosition());
		particlePtr->SetTextureResource(L"SMOKE_TX");
		particlePtr->SetMaxTime(m_drawTime);

		// 生成したスプライト配列を取得
		auto& spriteVec = particlePtr->GetParticleSpriteVec();
		for (auto& sprite : spriteVec)
		{
			// 移動量を設定
			Vec3 velo;
			velo.y = m_upVelo;
			velo.x = Utility::RangeRand(m_veloRange, -m_veloRange);
			velo.z = Utility::RangeRand(m_veloRange, -m_veloRange);
			sprite.m_Velocity = velo;

			// スケールを設定
			sprite.m_LocalScale = Vec2(Utility::RangeRand(m_scale, 0.1f));
		}
	}
}