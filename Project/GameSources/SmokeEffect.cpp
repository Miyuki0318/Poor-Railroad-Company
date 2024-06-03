/*!
@file SmokeEffect.cpp
@brief 列車から出る煙のエフェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	using namespace Utility;

	// 煙のエフェクト生成処理
	void SmokeEffect::AddSmokeEffect(float rotY)
	{
		// トランスフォームコンポーネント
		auto& ptrTrans = GetComponent<Transform>();

		// パーティクルを生成
		auto particlePtr = InsertParticle(2);
		particlePtr->SetEmitterPos(ptrTrans->GetWorldPosition());
		particlePtr->SetTextureResource(L"SMOKE_TX");
		particlePtr->SetMaxTime(m_drawTime);

		// 親オブジェクトの回転Y軸から方向を求める
		float x = -cos(rotY) / 2.0f;
		float z = -sin(rotY) / 2.0f;

		// 生成したスプライト配列を取得
		auto& spriteVec = particlePtr->GetParticleSpriteVec();
		for (auto& sprite : spriteVec)
		{
			// 移動量を設定
			Vec3 velo; 
			velo.y = m_upVelo;
			velo.x = RangeRand(x + m_veloRange, x - m_veloRange);
			velo.z = RangeRand(z + m_veloRange, z - m_veloRange);
			sprite.m_Velocity = velo;

			// スケールを設定
			sprite.m_LocalScale = Vec2(RangeRand(m_scale, 0.1f));
		}
	}
}