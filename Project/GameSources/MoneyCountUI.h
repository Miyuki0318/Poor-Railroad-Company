/*!
@file MoneyCountUI.h
@brief 所持金のカウンタUI
@author 小澤博貴
*/

#pragma once
#include "Number.h"

namespace basecross
{	
	/*!
	@brief 所持金UI
	*/
	class MoneyCountUI : public GameObject
	{
		weak_ptr<Sprite> m_backSprite;	// 背景スプライト
		weak_ptr<Sprite> m_itemSprite;	// アイテムアイコンスプライト
		weak_ptr<Sprite> m_slashSprite;	// ／スプライト

		// 所持金の数値スプライト
		vector<weak_ptr<Number>> m_numbers;

		const float m_spriteScale;	// スプライトのスケール
		const float m_numberScale;	// 数字のスケール
		const Vec2 m_backScale;		// 背景スプライトのスケール
		const Vec3 m_startPos;		// 開始座標
		const Vec3 m_spriteMargin;	// スプライト分の余白
		const Vec3 m_numberMargin;	// 数字分の余白

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param スケール
		@param ポジション
		@param テクスチャ
		@param アイテムタイプ
		*/
		MoneyCountUI(const shared_ptr<Stage>& stagePtr,
			const float scale,
			const Vec3& position
		) :
			GameObject(stagePtr),
			m_startPos(position),
			m_spriteScale(scale * 1.3f),
			m_numberScale(scale * 0.7f),
			m_spriteMargin(scale * 1.1f, 0.0f, 0.0f),
			m_numberMargin(scale * 0.55f, 0.0f, 0.0f),
			m_backScale(Vec2(scale * 5.0f, scale * 2.0f))
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~MoneyCountUI() {}

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