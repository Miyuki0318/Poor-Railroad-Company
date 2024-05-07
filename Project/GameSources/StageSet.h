#pragma once
#include "stdafx.h"

namespace basecross
{
	enum eGameMenber
	{
		Play,
		Clear,
		Over
	};
	class StageSet : public Stage
	{
		shared_ptr<SoundItem> m_BGM;

		// ゲームクリア・ゲームオーバーのスプライト
		shared_ptr<Sprite> m_gameClearLogo;
		shared_ptr<Sprite> m_gameOverLogo;

		// ゲームの状況
		eGameMenber m_gameProgress;

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

		//BGMの生成
		//void PlayBGM();

	public:
		StageSet() :Stage() {
			m_gameProgress = eGameMenber::Play;
		}
			

		virtual ~StageSet() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		virtual void OnDestroy()override;
		//タイマークラス取得関数
		const shared_ptr<Timer> GetTimer() const
		{
			return m_timer.lock();
		}
		//ゲームの進行状態をゲットする関数
		eGameMenber GetGameProgress()
		{
			return m_gameProgress;
		}
		//ゲームの進行状態をセットする関数
		void SetGameProgress(eGameMenber progress)
		{
			m_gameProgress = progress;
		}
	};

}
