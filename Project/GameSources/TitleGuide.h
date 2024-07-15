/*!
@file TitleGuide.h
@brief タイトルステージのガイドアイコン描画
@author 小澤博貴
*/

#pragma once
#include "CraftUI.h"

namespace basecross
{
	/*!
	@brief オブジェクトガイドアイコン
	*/
	class TitleGuide : public CraftUI
	{
		// タイトルオブジェクトマップ
		map<wstring, weak_ptr<TemplateObject>> m_titleObjMap;
		
		// プレイヤーポインタ
		weak_ptr<TemplateObject> m_playerPtr;
	
		const Vec3 m_diffPosition;	// ズレ分の座標
		const float m_drawRange;	// 表示するまでの時間

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		TitleGuide(const shared_ptr<Stage>& stagePtr) :
			CraftUI(stagePtr, L"WHITE_TX", Vec2(300.0f), 0.25f),
			m_diffPosition(50.0f, 100.0f, 0.0f),
			m_drawRange(3.0f)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~TitleGuide() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 距離を比較し、テクスチャを設定する関数
		@param 範囲内に存在したか
		*/
		bool SetRangeTexture();

		/*!
		@brief プレイヤーの座標を変換し、ガイドの座標を更新する関数
		*/
		void UpdatePosition();
	};
}