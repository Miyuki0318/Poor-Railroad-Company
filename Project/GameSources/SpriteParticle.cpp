/*!
@file SpriteParticle.cpp
@brief スプライトでのパーティクル
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 更新処理
	void SpriteParticle::OnUpdate()
	{
		if (m_spriteVec.empty()) return; // 空なら終了

		for (auto& spriteData : m_spriteVec)
		{
			// エラーチェック
			auto& sprite = spriteData.sprite.lock();
			if (!sprite) continue;

			// 経過時間が描画時間以上になってたら非アクティブ化
			if (spriteData.totalTime >= spriteData.drawTime)
			{
				sprite->SetDrawActive(false);
				sprite->SetUpdateActive(false);
				continue;
			}

			// 移動処理を送り、経過時間時間の割合に応じて透明にしていく
			sprite->MoveSprite();
			sprite->SetDiffuseColor(Utility::Lerp(COL_WHITE, COL_ALPHA, spriteData.totalTime / spriteData.drawTime));
			
			// デルタタイムで経過時間を加算
			spriteData.totalTime += DELTA_TIME;
		}
	}

	// パーティクルの追加
	void SpriteParticle::AddParticle(Vec2 scale, Vec2 velo, float rotZ, float drawTime)
	{
		// 非アクティブ状態のスプライトがあれば再利用
		for (auto& spriteData : m_spriteVec)
		{
			// エラーチェック
			auto& sprite = spriteData.sprite.lock();
			if (!sprite) continue;

			// 非アクティブなら初期化設定を行って終了
			if (!sprite->GetDrawActive())
			{
				spriteData.totalTime = 0.0f;
				spriteData.drawTime = drawTime;

				auto& sprite = spriteData.sprite.lock();
				sprite->SetPosition(m_emitterPosition);
				sprite->SetScale(scale);
				sprite->SetRotation(Vec3(0.0f, 0.0f, rotZ));
				sprite->SetVelocity(velo);
				sprite->SetDiffuseColor(COL_WHITE);
				sprite->SetDrawActive(true);
				sprite->SetUpdateActive(true);

				return;
			}
		}

		// 配列のサイズが上限数以上なら無視
		if (m_spriteVec.size() >= m_maxSprite) return;

		// 新規スプライトデータを生成
		ParticleData spriteData = {};
		spriteData.drawTime = drawTime;
		
		// ステージにスプライトを追加生成
		auto& sprite = GetStage()->AddGameObject<Sprite>(m_textureStr, scale, m_emitterPosition, rotZ);
		sprite->SetVelocity(velo);
		spriteData.sprite = sprite;

		// 配列にスプライトデータを追加
		m_spriteVec.push_back(spriteData);
	}
}