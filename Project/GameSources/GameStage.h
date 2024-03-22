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
		/*!
		@brief ビューとライトの生成
		*/
		void CreateViewLight();

		/*!
		@brief ステージボックスの生成
		*/
		void CreateGroundBox();

	public:

		//構築と破棄
		GameStage() :Stage() {}

		virtual ~GameStage() {}

		//初期化
		virtual void OnCreate() override;
	};


}
//end basecross

