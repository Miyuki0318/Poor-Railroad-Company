/*!
@file GamePlayerSuccesState.cpp
@brief プレイヤーのゲームクリア状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<GamePlayerSuccesState> GamePlayerSuccesState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<GamePlayerSuccesState> instance(new GamePlayerSuccesState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring GamePlayerSuccesState::GetStateName() const
	{
		return L"クリア状態ステート";
	}

	// ステート開始時の処理
	void GamePlayerSuccesState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// アニメーションの変更
		if (!player->IsAnimation(ePAK::Walking))
		{
			player->SetAnimationMesh(ePAK::Walking);
		}

		// ゴール時の座標を設定
		player->m_goalPosition = player->GetPosition();
		m_totalTime = 0.0f;
		m_moveTime = (player->m_goalStagingPosition - player->m_goalPosition).length() / 2.5f;

		// 回転先をゴール演出位置方向にする
		float rotY = atan2f(player->m_goalStagingPosition.z - player->m_goalPosition.z, player->m_goalStagingPosition.x - player->m_goalPosition.x);
		player->m_rotTarget = Vec3(cos(rotY), 0.0f, sin(rotY));

		// カメラの方向を保持
		Vec3 cameraPos = player->GetStage()->GetView()->GetTargetCamera()->GetEye();
		rotY = atan2f(cameraPos.z - player->m_goalStagingPosition.z, cameraPos.x - player->m_goalStagingPosition.x);
		m_toCameraRot = Vec3(cos(rotY), 0.0f, sin(-rotY));
	}

	// ステート更新時の処理
	void GamePlayerSuccesState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// 歩くアニメーションなら
		if (player->IsAnimation(ePAK::Walking))
		{
			// ゴール時の座標から演出用の座標に移動
			Vec3 pos = Utility::Lerp(player->m_goalPosition, player->m_goalStagingPosition, m_totalTime / m_moveTime);
			m_totalTime += DELTA_TIME;
			m_totalTime = min(m_totalTime, m_moveTime);

			// 経過時間が1.0以上ならクリア時のアニメーションに切り替える
			if (m_totalTime >= m_moveTime)
			{
				// 回転先をカメラの方向にする
				if (player->m_rotTarget != m_toCameraRot)
				{
					player->m_rotTarget = m_toCameraRot;
					player->UpdateStatus();
				}

				// 回転が終わっていたらアニメーションを終了
				if (!player->GetStatus(ePlayerStatus::IsRotate) && !player->IsAnimation(ePAK::GameSucces))
				{
					player->SetAnimationMesh(ePAK::GameSucces);

					const auto& stagePtr = player->GetTypeStage<GameStage>();
					stagePtr->SetGameProgress(eGameProgress::ClearSlect);
				}
			}

			// 座標の更新
			player->SetPosition(pos);
		}

		// アニメーションを更新
		player->UpdateAnimation();
		player->UpdateRotation();
	}

	// ステート終了時の処理
	void GamePlayerSuccesState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Aボタン入力時
	void GamePlayerSuccesState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Bボタン入力時
	void GamePlayerSuccesState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Yボタン入力時
	void GamePlayerSuccesState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Xボタン入力時
	void GamePlayerSuccesState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}
}