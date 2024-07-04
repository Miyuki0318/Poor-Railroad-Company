/*!
@file ItemFly.h
@brief アイテム追加時のUIに飛んでいくエフェクト
@author 小澤博貴
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	/*!
	@brief アイテム追加時に飛んでいくエフェクト
	*/
	class FlySprite : public Sprite
	{
		// 曲線補間に必要な座標
		Vec3 m_targetPointStart;	// 開始位置
		Vec3 m_targetPointEnd;		// 終了位置
		Vec3 m_targetPointMiddle;	// 中間位置

		// 星型エフェクト
		weak_ptr<SpriteParticle> m_starEffect;

		const float m_flyTime;	// 飛翔時間
		float m_totalTime;	// 経過時間

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		FlySprite(const shared_ptr<Stage>& stagePtr) :
			Sprite(stagePtr, L"WHITE_TX", Vec2(50.0f)),
			m_flyTime(1.0f)
		{
			m_targetPointStart.zero();
			m_targetPointEnd.zero();
			m_targetPointMiddle.zero();

			m_totalTime = 0.0f;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~FlySprite() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 描画開始関数
		@param テクスチャキー
		@param 開始座標
		@param 中間座標
		@param 終了座標
		*/
		void StartDraw(const wstring& texWstr, const Vec3& start, const Vec3& middle, const Vec3& end);
	};

	/*!
	@brief 飛翔するアイテムエフェクトの管理オブジェクト
	*/
	class FlyItemManager : public GameObject
	{
		// スプライト配列
		vector<weak_ptr<FlySprite>> m_spriteVec;

		// アイテムUIに関するデータ
		map<eItemType, pair<Vec3, wstring>> m_itemUIPosMap;

		// プレイヤーのポインタ
		weak_ptr<TemplateObject> m_playerPtr;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param プレイヤーのポインタ
		*/
		FlyItemManager(const shared_ptr<Stage>& stagePtr,
			const shared_ptr<TemplateObject>& playerPtr
		) :
			GameObject(stagePtr),
			m_playerPtr(playerPtr)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~FlyItemManager() {}

		/*!
		@brief リセット処理
		*/
		void ResetFly();

		/*!
		@brief エフェクト開始関数
		@param アイテムのタイプ
		*/
		void StartFly(eItemType itemType);

		/*!
		@brief アイテムUIのデータ設定関数
		@param アイテムのタイプ
		@param テクスチャキー
		@param UIの座標
		*/
		void SetTargetUIData(eItemType itemType, wstring texture, Vec3 position)
		{
			m_itemUIPosMap.emplace(itemType, make_pair(position, texture));
		}
	};
}