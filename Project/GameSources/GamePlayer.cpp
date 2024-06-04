/*!
@file GamePlayer.cpp
@brief ゲーム中のプレイヤーオブジェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Input;
	using namespace Utility;

	// 生成時の処理
	void GamePlayer::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Player::OnCreate();

		// プレイヤーに付加する機能の生成
		CreatePlayerFeatures();

		// ステートマシンの初期化
		m_playerState.reset(new PlayerStateMachine(GetThis<GamePlayer>()));
		m_playerState->SetState(PlayerIdleState::Instance());
	}

	// 毎フレーム更新処理
	void GamePlayer::OnUpdate()
	{
		// ステートマシンの更新処理を送る
		m_playerState->Update();

		// ステートマシンにAボタン入力時の処理を送る
		if (GetPushA()) m_playerState->PushA();

		// ステートマシンにBボタン入力時の処理を送る
		if (GetPushB()) m_playerState->PushB();

		// ステートマシンにYボタン入力時の処理を送る
		if (GetPushY()) m_playerState->PushY();

		// ステートマシンにXボタン入力時の処理を送る
		if (GetPushX()) m_playerState->PushX();

		// アイテム状態の更新
		UpdateStatus();

		// デバック用文字列
		Debug::Log(L"プレイヤーの座標 : ", GetPosition());
		Debug::Log(L"プレイヤーのステート : ", m_playerState->GetCurrentState()->GetStateName());
	}

	// コンポーネント設定
	void GamePlayer::CreateComponent()
	{
		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->SetSpecularColor(COL_BLACK);
		m_ptrDraw->SetOwnShadowActive(true);
		m_ptrDraw->SetLightingEnabled(false);

		// 影の設定
		m_ptrShadow = AddComponent<Shadowmap>();
		m_ptrShadow->SetMeshToTransformMatrix(m_modelMat);

		// メッシュとアニメーションの設定
		SetAnimationMesh(ePAKey::Waiting);

		// コリジョンOBBの追加
		AddComponent<CollisionCapsule>();

		// 重力の追加
		AddComponent<Gravity>();
	}

	// プレイヤーに付加する機能の生成
	void GamePlayer::CreatePlayerFeatures()
	{
		// ステージの取得(shared_ptrをconstで取得)
		const auto& stagePtr = GetStage();

		// thisスマートポインタ
		const auto& thisPtr = GetThis<TemplateObject>();

		// セレクトインディケーターの生成
		m_indicator = stagePtr->AddGameObject<SelectIndicator>(thisPtr);

		// アクションガイドの生成
		stagePtr->AddGameObject<ActionGuide>(GetThis<GamePlayer>(), m_indicator.lock());

		// クラフトマネージャの生成
		const auto& cWindow = stagePtr->AddGameObject<CraftWindow>();
		const auto& cQTE = stagePtr->AddGameObject<CraftingQTE>();
		m_craft.reset(new CraftManager(thisPtr, cWindow, cQTE));
	}

	// インディケーターの取得に応じて処理
	void GamePlayer::IndicatorOrder()
	{
		// エラーチェック
		const auto& indicator = m_indicator.lock();
		if (!indicator) return;
		
		// 採掘命令、採掘できたら終了
		if (GatheringOrder(indicator)) return;

		// レール設置命令、設置できたら終了
		if (AddRailOrder(indicator)) return;

		// 木の足場設置命令、設置できたら終了
		if (AddBridgeOrder(indicator)) return;

		// 踏切の設置命令、設置できたら終了
		if (AddCrossingOrder(indicator)) return;
	}

	// 採掘命令
	bool GamePlayer::GatheringOrder(const shared_ptr<SelectIndicator>& indicator)
	{
		// 採掘命令を送り、採掘できたらタグセットを受け取る
		int id = indicator->GatheringOrder();

		// 採掘オブジェクトのタグセットが空じゃなければ採掘処理を送る
		if (id != 0)
		{
			GatheringProcces(id);
			return true;
		}
		
		return false;
	}

	// 採掘処理
	void GamePlayer::GatheringProcces(int stageID)
	{
		// 採掘対象マップを用いて採掘数を追加
		eStageID id = STAGE_ID(stageID);
		
		// アイテムカウンタの追加とSEの再生
		if (m_gCountMap.find(id) != m_gCountMap.end())
		{
			AddItemCount(m_gCountMap.at(id));
		}

		if (m_gSoundMap.find(id) != m_gSoundMap.end())
		{
			StartSE(m_gSoundMap.at(id) + L"_SE", 1.0f);
		}

		// 採掘状態にする
		m_status.Set(ePlayerStatus::IsGathering) = true;
	}

	// レール追加命令
	bool GamePlayer::AddRailOrder(const shared_ptr<SelectIndicator>& indicator)
	{
		// レールを所持してたら
		if (GetItemCount(eItemType::Rail))
		{
			// 設置命令を送り、設置できたら所持数を減らす
			if (indicator->RailedOrder())
			{
				m_craft->UseItem(eItemType::Rail);
				StartSE(L"ADDRAIL_SE", 1.0f);
				return true;
			}
		}

		return false;
	}

	// 木の足場追加命令
	bool GamePlayer::AddBridgeOrder(const shared_ptr<SelectIndicator>& indicator)
	{
		// 木の足場を所持してたら
		if (GetItemCount(eItemType::WoodBridge))
		{
			if (indicator->BridgeOrder())
			{
				m_craft->UseItem(eItemType::WoodBridge);
				//StartSE(L"ADDBRIDGE_SE", 1.0f);
				return true;
			}
		}

		return false;
	}

	// 踏切追加命令
	bool GamePlayer::AddCrossingOrder(const shared_ptr<SelectIndicator>& indicator)
	{
		// 木の足場を所持してたら
		if (GetItemCount(eItemType::Gear) && GetItemCount(eItemType::Wood))
		{
			if (indicator->CrossingOrder())
			{
				m_craft->UseItem(eItemType::Gear);
				m_craft->UseItem(eItemType::Wood);
				//StartSE(L"ADDBRIDGE_SE", 1.0f);
				return true;
			}
		}

		return false;
	}

	// クラフト状態でのXボタン入力
	void GamePlayer::SwitchCraftWindow()
	{
		// クラフト中じゃない時に、クラフト不可なら
		if (!m_status(ePlayerStatus::IsCrafting) && !GetCraftPosshible()) return;

		// クラフト状態を切り替える
		m_status.Set(ePlayerStatus::IsCrafting) = !m_status(ePlayerStatus::IsCrafting);

		// クラフトマネージャーにクラフト状態を送る
		m_craft->CraftingEnabled(m_status(ePlayerStatus::IsCrafting));
	}

	// アイテム状態の更新
	void GamePlayer::UpdateStatus()
	{
		// 移動状態の更新
		m_status.Set(ePlayerStatus::IsRotate) = ((m_currentRot - m_rotTarget).length() > XM_1DIV2PI);
		m_status.Set(ePlayerStatus::IsIdle) = !m_status(ePlayerStatus::IsMove) && !m_status(ePlayerStatus::IsRotate);

		// アイテム状態の更新(今後アイテムの追加があれば適宜追加)
		m_status.Set(ePlayerStatus::IsHaveRail) = GetItemCount(eItemType::Rail);
		m_status.Set(ePlayerStatus::IsHaveWood) = GetItemCount(eItemType::Wood);
		m_status.Set(ePlayerStatus::IsHaveStone) = GetItemCount(eItemType::Stone);
	}

	// 移動更新
	void GamePlayer::UpdateMove()
	{
		// 継承元の移動更新処理
		Player::UpdateMove();

		// LStickの入力があるなら
		bool isLStick = IsInputLStick();
		if (isLStick)
		{
			// LStick入力量の取得
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			m_acsel += DELTA_TIME;
			m_acsel = min(m_acsel, m_maxAcsel);
			SetRotateTarget(stickValue); // 回転関数
			ControllerMovement(stickValue);	// 移動関数
		}

		// 移動状態を設定
		m_status.Set(ePlayerStatus::IsMove) = isLStick;
	}

	// ゲーム結果に応じてステートを切り替える
	void GamePlayer::SetGameResult(eGameProgress result)
	{
		// プレイヤー中なら無視
		if (result == eGameProgress::Playing) return;

		// クリアならサクセスステート、失敗ならフェイルドステートに切り替える
		result == GameClear ? SetState(PlayerSuccesState::Instance()) : SetState(PlayerFailedState::Instance());
	}
}