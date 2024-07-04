/*!
@file ItemFly.cpp
@brief アイテム追加時のUIに飛んでいくエフェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void FlySprite::OnCreate()
	{
		// 継承元の生成時の処理を送る
		Sprite::OnCreate();

		// 非アクティブに
		SetDrawActive(false);
		SetUpdateActive(false);

		// 星型エフェクトの生成
		m_starEffect = GetStage()->GetSharedGameObject<SpriteParticle>(L"StarEffect");
	}

	// 更新処理
	void FlySprite::OnUpdate()
	{
		// 曲線補間で座標を求める
		Vec3 pos = BezierLerp(m_targetPointStart, m_targetPointMiddle, m_targetPointEnd, m_totalTime / m_flyTime);
		m_totalTime += DELTA_TIME;
		m_totalTime = min(m_totalTime, m_flyTime);

		// 経過時間が飛翔時間以上になったら
		if (m_totalTime >= m_flyTime)
		{
			// 座標を終了座標にし、非アクティブに
			pos = m_targetPointEnd;
			SetDrawActive(false);
			SetUpdateActive(false);
		}
		
		// 座標の更新
		Vec3 pastPos = GetPosition();
		SetPosition(pos);
		SetRotation(Vec3(0.0f, 0.0f, m_totalTime));

		// 星型エフェクトの生成
		Vec2 velo = Vec2(Vec3(pastPos - pos).normalize());
		auto& starEffect = m_starEffect.lock();
		starEffect->SetEmitterPosition(pos + Vec3(0.0f, 0.0f, 0.1f));
		starEffect->AddParticle(Vec2(m_scale), -velo, RangeRand(XM_PI, -XM_PI), 0.25f);
	}

	// 開始設定
	void FlySprite::StartDraw(const wstring& texWstr, const Vec3& start, const Vec3& middle, const Vec3& end)
	{
		// 初期設定
		SetTexture(texWstr); // テクスチャを設定
		m_targetPointStart = start; // 開始座標
		m_targetPointMiddle = middle; // 中間座標
		m_targetPointEnd = end; // 終了座標
		m_totalTime = 0.0f; // 経過時間を0に

		// アクティブに
		SetDrawActive(true);
		SetUpdateActive(true);
	}

	// リセット処理
	void FlyItemManager::ResetFly()
	{
		// 全ループ
		for (auto& weakPtr : m_spriteVec)
		{
			// エラーチェック
			auto& sprite = weakPtr.lock();
			if (!sprite) continue;

			// 非アクティブ処理
			sprite->SetDrawActive(false);
			sprite->SetUpdateActive(false);
		}
	}

	// 開始呼び出し
	void FlyItemManager::StartFly(eItemType itemType)
	{
		// エラーチェック
		if (m_itemUIPosMap.find(itemType) == m_itemUIPosMap.end()) return;
		
		// アイテムタイプとステージの取得
		const auto& item = m_itemUIPosMap.at(itemType);
		const auto& stagePtr = GetStage();
		
		// プレイヤーの座標を2D座標に変換
		Vec3 playerPos = ConvertToWorldPosition(stagePtr->GetView(), m_playerPtr.lock()->GetPosition());
		playerPos.z = 0.0f;

		// 中間座標を設定
		Vec3 middlePos = Vec3(playerPos.x, item.first.y, 0.0f);

		// スプライト配列に非アクティブがあるならそれを扱う
		for (auto& weakPtr : m_spriteVec)
		{
			// エラーチェック
			auto& sprite = weakPtr.lock();
			if (!sprite) continue;

			// 非アクティブなら開始処理を送り終了
			if (!sprite->GetUpdateActive())
			{
				sprite->StartDraw(item.second, playerPos, middlePos, item.first);
				return;
			}
		}

		// 非アクティブが無かったので生成して開始処理を送る
		auto& newSprite = stagePtr->AddGameObject<FlySprite>();
		newSprite->StartDraw(item.second, playerPos, middlePos, item.first);
		
		// 配列に追加
		m_spriteVec.push_back(newSprite);
	}
}