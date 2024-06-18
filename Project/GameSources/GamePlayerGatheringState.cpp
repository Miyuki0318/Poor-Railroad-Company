/*!
@file GamePlayerGatheringState.cpp
@brief プレイヤーの採掘状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<GamePlayerGatheringState> GamePlayerGatheringState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<GamePlayerGatheringState> instance(new GamePlayerGatheringState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring GamePlayerGatheringState::GetStateName() const
	{
		return L"採掘状態ステート";
	}

	// ステート開始時の処理
	void GamePlayerGatheringState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// アニメーションの変更
		player->SetAnimationMesh(ePAK::Harvesting);

		// フラグを初期化
		m_isFlyEffect = false;

		// アニメーションの半分の時間を求める(フレーム数÷レベルによる速度÷FPS÷2.0f)
		m_animeHelfTime = player->m_animationMap.at(ePAK::Harvesting).flameNum
			/ player->m_playerData.at(ePST::GatherSpeed).at(player->m_playerLevel)
			/ ANIME_FPS / 2.0f;
	}

	// ステート更新時の処理
	void GamePlayerGatheringState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// アニメーション更新
		float animeSpeed = player->m_playerData.at(ePST::GatherSpeed).at(player->m_playerLevel);
		player->UpdateAnimation(animeSpeed);
		player->UpdateRotation();

		// アイテムエフェクトを出す
		float animeTime = player->m_ptrDraw->GetCurrentAnimationTime();
		if (!m_isFlyEffect && m_animeHelfTime <= animeTime)
		{
			m_isFlyEffect = true;
			player->m_itemFly.lock()->StartFly(player->m_addItem);
		}

		// 採掘中の待機時間
		if (!player->m_ptrDraw->IsTargetAnimeEnd()) return;

		// 採掘完了してたらステートを待機に遷移させる
		player->m_status.Set(ePlayerStatus::IsGathering) = false;

		// Aボタン入力があるなら
		if (Input::GetButtonB())
		{
			player->IndicatorOrder();
			m_isFlyEffect = false;
		}

		// 採取ステータスになっていればアニメーションの切り替え、なってなければステートの切り替え
		player->m_status(ePlayerStatus::IsGathering) ? player->SetAnimationMesh(ePAK::Harvesting) : player->SetState(GamePlayerIdleState::Instance());
	}

	// ステート終了時の処理
	void GamePlayerGatheringState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Aボタン入力時
	void GamePlayerGatheringState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Bボタン入力時
	void GamePlayerGatheringState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Xボタン入力時
	void GamePlayerGatheringState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Yボタン入力時
	void GamePlayerGatheringState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}
}