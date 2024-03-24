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

		// タイマーオブジェクト
		weak_ptr<Timer> m_timer;

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

		/*!
		@brief コンストラクタ
		*/
		GameStage() :Stage() {}

		/*!
		@brief デストラクタ
		*/
		virtual ~GameStage() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		///*!
		//@brief 破棄される時に一度だけ呼び出される関数
		//*/
		//virtual void OnDestroy() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief 描画更新関数
		*/
		virtual void OnDraw() override;

		/*!
		@brief タイマークラス取得関数
		@return const shared_ptr<Timer>
		*/
		const shared_ptr<Timer> GetTimer() const
		{
			return m_timer.lock();
		}
	};
}