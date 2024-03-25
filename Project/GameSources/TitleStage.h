/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class TitleStage : public Stage
	{
	private:
		bool m_startPush;

		shared_ptr<Sprite> m_fadeSprite;

		/*
		@brief ビューとライトの生成
		*/
		void CreateViewLight();

		/*
		@brief リソースの読込
		*/
		void CreateResourses();

		/*
		@brief スプライトの生成
		*/
		void CreateSpriteObject();
		
		/*
		@brief スタートボタンを押した時の処理
		*/
		void PushStartButton();

		/*
		@brief フェード処理
		*/
		void FadeSprite();

	public:
		/*
		@brief コンストラクタ
		*/
		TitleStage() : Stage()
		{
			m_startPush = false;
		}

		/*
		@brief デストラクタ
		*/
		virtual ~TitleStage() {};

		/*
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;
		
		/*
		@brief 毎フレーム呼び出される関数
		*/
		virtual void OnUpdate() override;
		
		/*
		@brief オブジェクト破棄時に呼び出される関数
		*/
		virtual void OnDestroy() override;
	};
}