/*!
@file GamePlayer.cpp
@brief ゲーム中のプレイヤーオブジェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "Input.h"
#include "OzawaUtility.h"
#include "GamePlayer.h"
#include "GamePlayerState.h"
#include "ActionGuide.h"

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

		// 開始時の座標に移動
		SetPosition(m_startPosition);

		// ステートマシンの初期化
		m_playerState.reset(new GamePlayerStateMachine(GetThis<GamePlayer>()));
		m_playerState->SetState(GamePlayerIdleState::Instance());
		m_status.Set(ePlayerStatus::IsIdle) = true;

		// ステータスの設定
		const auto& scene = App::GetApp()->GetScene<Scene>();
		m_statusLevel = scene->GetStatusLevel();
		m_limitChoicesLevel = scene->GetLimitChoicesLevel();
		m_startGearLevel = scene->GetStartGearLevel();

		// 初期化
		m_addItem = make_pair(eItemType::None, 0);

		// 開始時の所持ギア設定
		AddItemCount(eItemType::Gear, (int)m_playerData.at(ePST::StartGear).at(m_startGearLevel));
	}

	// 毎フレーム更新処理
	void GamePlayer::OnUpdate()
	{
		// ポーズ中なら無視
		const auto& prog = GetTypeStage<GameStage>()->GetGameProgress();
		bool isPause = ORS(prog, eGameProgress::FadeIn, eGameProgress::Pause, eGameProgress::Opening);
		if (isPause) return;

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
		UpdateItemCount();
	}

	// リセット処理
	void GamePlayer::ResetPlayer(const Vec3& start, const Vec3& goal)
	{
		// 初期化
		m_acsel = 0.0f;
		m_moveValue = 0.0f;
		m_rotTarget.zero(); 
		m_currentRot.zero();
		m_startPosition = start;
		m_goalStagingPosition = goal;
		SetPosition(m_startPosition);
		ResetCrafting();

		// 待機状態ステートに変更
		m_playerState->SetState(GamePlayerIdleState::Instance());

		// クラフトマネージャーにリセット処理を送る
		m_craft->ResetCraftManager();

		// ステータスの設定
		const auto& scene = App::GetApp()->GetScene<Scene>();
		m_statusLevel = scene->GetStatusLevel();
		m_limitChoicesLevel = scene->GetLimitChoicesLevel();
		m_startGearLevel = scene->GetStartGearLevel();

		// 開始時の所持ギア設定
		AddItemCount(eItemType::Gear, (int)m_playerData.at(ePST::StartGear).at(m_startGearLevel));
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

		// アイテムエフェクトの生成
		m_itemFly = stagePtr->AddGameObject<FlyItemManager>(thisPtr);
		stagePtr->SetSharedGameObject(L"FlyItemManager", m_itemFly.lock());

		// アクションガイドの生成
		stagePtr->AddGameObject<ActionGuide>(GetThis<GamePlayer>(), m_indicator.lock());

		// クラフトマネージャの生成
		const auto& cWindow = stagePtr->AddGameObject<CraftWindow>();
		const auto& cQTE = stagePtr->AddGameObject<CraftingQTE>();
		m_craft.reset(new CraftManager(thisPtr, cWindow, cQTE));
		m_craft->OnCreate();
	}

	// インディケーターの取得に応じて処理
	void GamePlayer::IndicatorOrder()
	{
		// エラーチェック
		const auto& indicator = m_indicator.lock();
		if (!indicator) return;
		
		// インディケーターの方向に向く
		if (GetButtonRB()) SetRotateIndicatorAngle();

		// 採掘命令、採掘できたら終了
		if (GatheringOrder(indicator)) return;

		// レール設置命令、設置できたら終了
		if (AddRailOrder(indicator)) return;

		// 木の足場設置命令、設置できたら終了
		if (AddBridgeOrder(indicator)) return;

		// 踏切の設置命令、設置できたら終了
		if (AddCrossingOrder(indicator)) return;
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
		// 踏切を所持してたら
		if (GetItemCount(eItemType::Crossing))
		{
			if (indicator->CrossingOrder())
			{
				m_craft->UseItem(eItemType::Crossing);
				StartSE(L"ADDRAIL_SE", 1.0f);
				return true;
			}
		}

		return false;
	}

	// クラフト状態でのXボタン入力
	void GamePlayer::SwitchCraftWindow()
	{
		// 演出中じゃなければ
		bool isStaging = GetTypeStage<BaseStage>()->GetIsStaging();
		if (isStaging) return;

		// クラフト中じゃない時に、クラフト不可なら
		if (!m_status(ePlayerStatus::IsCrafting) && !GetCraftPosshible()) return;

		// クラフト状態を切り替える
		m_status.Set(ePlayerStatus::IsCrafting) = !m_status(ePlayerStatus::IsCrafting);

		// クラフトマネージャーにクラフト状態を送る
		m_craft->CraftingEnabled(m_status(ePlayerStatus::IsCrafting));

		// ステージの状態を変更する
		const auto& stagePtr = GetTypeStage<GameStage>();
		auto prog = stagePtr->GetGameProgress();
		if (OR(prog, eGameProgress::Playing, eGameProgress::CraftPause))
		{
			stagePtr->SetGameProgress(m_status(ePlayerStatus::IsCrafting) ? eGameProgress::CraftPause : eGameProgress::Playing);
		}
	}

	// クラフト中なら初期化する
	void GamePlayer::ResetCrafting()
	{
		// クラフトQTE中なら閉じる
		if (GetStatus(ePlayerStatus::IsCraftQTE))
		{
			m_craft->DestroyCraftQTE();
			m_status.Set(ePlayerStatus::IsCraftQTE) = false;
		}

		// クラフト中ならウィンドウを閉じる
		if (GetStatus(ePlayerStatus::IsCrafting))
		{
			SwitchCraftWindow();
		}
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

	// アイテム数の更新
	void GamePlayer::UpdateItemCount()
	{
		// アイテム数が越えない様に上書き
		m_craft->m_itemCount.at(eItemType::Stone) = min(m_craft->m_itemCount.at(eItemType::Stone), (unsigned char)m_playerData.at(ePST::StoneMax).at(m_limitChoicesLevel));
		m_craft->m_itemCount.at(eItemType::Wood) = min(m_craft->m_itemCount.at(eItemType::Wood), (unsigned char)m_playerData.at(ePST::WoodMax).at(m_limitChoicesLevel));
		m_craft->m_itemCount.at(eItemType::Gear) = min(m_craft->m_itemCount.at(eItemType::Gear), (unsigned char)m_playerData.at(ePST::GearMax).at(m_limitChoicesLevel));
		m_craft->m_itemCount.at(eItemType::Rail) = min(m_craft->m_itemCount.at(eItemType::Rail), (unsigned char)m_playerData.at(ePST::RailMax).at(m_limitChoicesLevel));
		m_craft->m_itemCount.at(eItemType::WoodBridge) = min(m_craft->m_itemCount.at(eItemType::WoodBridge), (unsigned char)m_playerData.at(ePST::BridgeMax).at(m_limitChoicesLevel));
		m_craft->m_itemCount.at(eItemType::Crossing) = min(m_craft->m_itemCount.at(eItemType::Crossing), (unsigned char)m_playerData.at(ePST::CrossingMax).at(m_limitChoicesLevel));
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
			Vec2 LStick = GetLStickValue();
			Vec3 stickValue = Vec3(LStick.x, 0.0f, LStick.y);

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
		result == GameClear ? SetState(GamePlayerSuccesState::Instance()) : SetState(GamePlayerFailedState::Instance());
	}
}