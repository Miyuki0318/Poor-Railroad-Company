/*!
@file TitlePlayer.h
@brief タイトル用プレイヤーオブジェクト
@author 小澤博貴
*/

#pragma once
#include "Player.h"

namespace basecross
{
	/*!
	@brief タイトル用プレイヤー
	*/
	class TitlePlayer : public Player
	{
	public:
		
		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		TitlePlayer(const shared_ptr<Stage>& stagePtr) :
			Player(stagePtr)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~TitlePlayer() {}

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

	private:

		/*!
		@brief コンポーネントの生成関数
		*/
		void CreateComponent() override;

		/*!
		@brief 移動更新関数
		*/
		void UpdateMove() override;
	};
}