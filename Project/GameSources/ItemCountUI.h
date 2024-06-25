/*!
@file ItemCountUI.h
@brief アイテムのカウンタUI
@author 小澤博貴
*/

#pragma once
#include "Number.h"
#include "CraftManager.h"

namespace basecross
{
	/*!
	@brief アイテム数UI
	*/
	class ItemCountUI : public GameObject
	{	
		weak_ptr<Sprite> m_backSprite;	// 背景スプライト
		weak_ptr<Sprite> m_itemSprite;	// アイテムアイコンスプライト
		weak_ptr<Sprite> m_slashSprite;	// ／スプライト

		// アイテム数と上限数のスプライト
		pair<weak_ptr<Number>, weak_ptr<Number>> m_itemNums;
		pair<weak_ptr<Number>, weak_ptr<Number>> m_limitNums;

		const float m_spriteScale;	// スプライトのスケール
		const float m_numberScale;	// 数字のスケール
		const float m_limitScale;	// 上限数のスケール
		const Vec2 m_backScale;		// 背景スプライトのスケール
		const Vec3 m_startPos;		// 開始座標
		const Vec3 m_spriteMargin;	// スプライト分の余白
		const Vec3 m_numberMargin;	// 数字分の余白
		const Vec3 m_limitMargin;	// 上限数分の余白
		const Vec3 m_limitPos;		// 上限数用の差分
		const wstring m_texKey;		// テクスチャ
		const eItemType m_itemType;	// アイテムタイプ

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param スケール
		@param ポジション
		@param テクスチャ
		@param アイテムタイプ
		*/
		ItemCountUI(const shared_ptr<Stage>& stagePtr,
			const float scale,
			const Vec3& position,
			const wstring& texture,
			eItemType itemType
		) :
			GameObject(stagePtr),
			m_startPos(position),
			m_texKey(texture),
			m_itemType(itemType),
			m_spriteScale(scale * 1.3f),
			m_numberScale(scale * 0.7f),
			m_limitScale(scale * 0.3f),
			m_spriteMargin(scale * 1.15f, 0.0f, 0.0f),
			m_numberMargin(scale * 0.5f, 0.0f, 0.0f),
			m_limitMargin(scale * 0.35f, 0.0f, 0.0f),
			m_limitPos(0.0f, -scale * 0.35f / 2.0f, 0.0f),
			m_backScale(Vec2(scale * 4.25f, scale * 2.0f))
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~ItemCountUI() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 数字の更新
		*/
		void UpdateNumbers();
	};
}