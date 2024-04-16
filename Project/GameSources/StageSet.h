/*!
@file StageSet.cpp
@brief ステージの配置実体
*/

#pragma once
#include "stdafx.h"
namespace basecross {
	class StageSet :public Stage
	{
		enum eGameProgress
		{
			Playing,
			GameClear,
			GameOver
		};

		// ゲームの状況
		eGameProgress m_gameProgress;
		/*!
		@brief リソースの読込
		*/
		void CreateResourses();

		/*!
		@brief ビューとライトの生成
		*/
		void CreateViewLight();

		/*!
		@brief ステージボックスの生成
		*/
		void CreateGroundBox();

		/*!
		@brief プレイヤー生成
		*/
		void CreatePlayer();

		/*!
		@brief 採掘系オブジェクトの生成生成
		*/
		void CreateStageObject();

		/*!
		@brief 線路生成
		*/
		void CreateRail();
		void CreateRails();

		/*!
		@brief 列車生成
		*/
		void CreateTrain();

		/*!
		@brief 線路の終着生成
		*/
		void CreateTarminal();

		/*!
		@brief スプライトの生成
		*/
		void CreateSpriteObject();

		/*!
		@brief スプライトの生成
		*/
		void LogoActive();

	public:

		/*!
		@brief コンストラクタ
		*/
		StageSet() :Stage() {
			m_gameProgress = eGameProgress::Playing;
		}
		/*!
		@brief デストラクタ
		*/
		virtual ~StageSet() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief 描画更新関数
		*/
		virtual void OnDraw() override;



	};

}