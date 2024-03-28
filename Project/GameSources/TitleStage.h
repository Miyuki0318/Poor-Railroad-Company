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
		// スタートボタンが押されたか
		bool m_startPush;

		// フェードが終わったか
		bool m_fadeStop;

		const float m_fadeOutTime = 1.0f;
		const float m_fadeInTime = 3.0f;

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
		@brief フェードイン処理
		*/
		void FadeInSprite();

		/*
		@brief フェードアウト処理
		*/
		void FadeOutSprite();

	public:
		/*
		@brief コンストラクタ
		*/
		TitleStage() : Stage()
		{
			m_startPush = false;
			m_fadeStop = false;
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