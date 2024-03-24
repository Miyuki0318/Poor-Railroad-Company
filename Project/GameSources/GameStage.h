/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	/*!
	@brief ゲーム中のステージ
	*/
	class GameStage : public Stage
	{
		shared_ptr<Sprite> m_gameClearLogo;
		shared_ptr<Sprite> m_gameOverLogo;

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

		//構築と破棄
		GameStage() :Stage() {}

		virtual ~GameStage() {}

		//初期化
		virtual void OnCreate() override;

		virtual void OnDraw() override;
	};


}
//end basecross

