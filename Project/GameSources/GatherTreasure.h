/*!
@file GatherEffect.h
@brief お宝機能
@author 小澤博貴
*/

#pragma once
#include "CraftManager.h"
#include "Billboard.h"

namespace basecross
{
	/*!
	@brief お宝エフェクト
	*/
	class GatherEffect : public Billboard
	{
		const float m_effectTime; // 描画時間
		float m_totalTime; // 経過時間

		const Vec2 m_drawScale; // スケール
		const Vec3 m_boundVal; // バウンド距離
		Vec3 m_startPos; // 開始座標

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		GatherEffect(const shared_ptr<Stage>& stagePtr) :
			Billboard(stagePtr, L"TREASURE_TX", Vec2(0.0f), Vec3(0.0f)),
			m_effectTime(2.0f),
			m_drawScale(0.75f),
			m_boundVal(0.0f, 0.75f, 0.0f)
		{
			m_totalTime = 0.0f;
			m_startPos.zero();
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~GatherEffect() {}

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
		@param 開始座標
		*/
		void StartDraw(const Vec3& position);
	};

	/*!
	@brief お宝機能
	*/
	class GatherTreasure : public TemplateObject
	{
		// エフェクト配列
		vector<weak_ptr<GatherEffect>> m_effectVec;

		const eItemType m_items[3]; // アイテムタイプ
		map<eItemType, int> m_addVal; // アイテム加算量

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		GatherTreasure(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_items{eItemType::Rail, eItemType::Gear, eItemType::GoldBar}
		{
			m_addVal.emplace(eItemType::Rail, 5);
			m_addVal.emplace(eItemType::Gear, 1);
			m_addVal.emplace(eItemType::GoldBar, 3);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~GatherTreasure() {}

		/*!
		@brief お宝確認処理関数
		@param 確認座標
		@return 発見したお宝データ
		*/
		pair<eItemType, int> TreasureCheck(const Vec3& position);
	};
}