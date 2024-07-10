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
	class MoneyCountUI : public TemplateObject
	{
		weak_ptr<Sprite> m_backSprite;	// 背景スプライト
		weak_ptr<Sprite> m_itemSprite;	// アイテムアイコンスプライト
		weak_ptr<Sprite> m_slashSprite;	// ／スプライト

		// 所持金の数値スプライト
		NumberCount m_numbers;
		vector<float> m_totalTime; // 経過時間
		bool m_isGoal; // 表示し終わったかの真偽


		const float m_spriteScale;	// スプライトのスケール
		const float m_numberScale;	// 数字のスケール
		const Vec2 m_backScale;		// 背景スプライトのスケール
		const Vec3 m_startPos;		// 開始座標
		const Vec3 m_spriteMargin;	// スプライト分の余白
		const Vec3 m_numberMargin;	// 数字分の余白
		const Vec3 m_backMargin;	// 背景の余白

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
			TemplateObject(stagePtr),
			m_startPos(position),
			m_spriteScale(scale * 1.3f),
			m_numberScale(scale * 0.7f),
			m_spriteMargin(scale * 1.1f, 0.0f, 0.0f),
			m_numberMargin(scale * 0.55f, 0.0f, 0.0f),
			m_backScale(Vec2(scale * 5.0f, scale * 2.0f)),
			m_backMargin(Vec3(scale * 1.4f, 0.0f, 0.0f))
		{
			m_isGoal = true;
			m_totalTime = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
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
		@brief 開始時の数値設定関数
		*/
		void StartSetNumbers();

		/*!
		@brief 数字の更新
		*/
		void UpdateNumbers();

		/*!
		@brief 色の更新
		*/
		void UpdateColor();

		/*!
		@brief 数字の桁数とインデックスの設定関数
		@param 設定する金額
		*/
		void SetNumberGoal(int money)
		{
			// 初期化設定
			m_isGoal = false;
			m_numbers.goal = money;
			m_numbers.degit = max(Utility::GetUpperDigit(money) - 1, 0);
		}
	};
}