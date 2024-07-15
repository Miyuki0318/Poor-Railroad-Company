/*!
@file Number.h
@brief 数字を表示するスプライト
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"
#include "Sprite.h"

// 桁の計算用マクロ
#define ONE_PLACE(num) num / 1 % 10
#define TENS_PLACE(num) num / 10 % 10

namespace basecross
{
	// ----------------------------------------------------------------------------------
	//	Number : public Sprite 数字を表示するスプライトの継承元
	// ----------------------------------------------------------------------------------
	
	// 数字表示スプライト
	class Number : public Sprite
	{
		int m_number; // 数字

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param スケール
		@param ポジション
		*/
		Number(const shared_ptr<Stage>& stagePtr,
			const Vec2& scale, const Vec3& position
		) :
			Sprite(stagePtr, L"NUMBER_TX", scale, position)
		{
			m_number = 0;
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param スケール
		@param ポジション
		@param テクスチャ
		*/
		Number(const shared_ptr<Stage>& stagePtr,
			const Vec2& scale, const Vec3& position,
			const wstring& texture, const int& number
		) :
			Sprite(stagePtr, texture, scale, position)
		{
			m_number = number;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Number() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief 表示する数字を変更する関数
		@param 数字
		*/
		void SetNumber(const int number);
	};


	// 数字用構造体
	struct NumberCount
	{
		int goal = 0;
		int degit = 0;
		int index = 0;
		vector<weak_ptr<Number>> sprite;

		/*!
		@brief 連番数値表示関数
		@param 数値構造体
		*/
		static void ConsecutiveNumberDraw(NumberCount& count, vector<float>& totalTime)
		{
			// 桁事の比較様にwssに変換
			wstringstream time;
			time << count.goal;
			wstringstream total;
			total << static_cast<int>(totalTime.at(count.degit));

			// 配列の要素数を設定(元の桁数-現在の桁数)
			int index = count.index - count.degit;

			// 数字を経過時間の現在の桁数番目で更新
			count.sprite.at(index).lock()->SetDiffuseColor(COL_BLACK);
			count.sprite.at(index).lock()->SetNumber(static_cast<int>(totalTime.at(count.degit)));

			// 目標数の現在の桁数番目と、経過時間の0番目が同じなら
			bool notZero = (time.str()[count.degit] == '0'); // 目標数が0なら10まで数えてから切り替える
			if (time.str()[count.degit] == total.str()[int(notZero)])
			{
				// 現在の桁数を減らす
				count.degit--;

				// 0より小さくなったら
				if (count.degit < 0)
				{
					// 経過時間を初期化して終了
					for (auto& time : totalTime)
					{
						time = 0.0f;
					}
					return;
				}
			}

			// 現在の桁数番目の経過時間を加算		
			const float fpsParTime = 0.6f;
			totalTime.at(count.degit) += fpsParTime;
		}
	};
}