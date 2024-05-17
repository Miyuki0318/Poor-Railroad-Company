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

		// ステートマシンにXボタン入力時の処理を送る
		if (GetPushX()) m_playerState->PushX();

		// アイテム状態の更新
		UpdateStatus();

		// デバック用文字列
		Debug::Log(L"プレイヤーのステート : ", m_playerState->GetCurrentState()->GetStateName());
		Debug::Log(L"プレイヤーの座標 : ", GetPosition());
		Debug::Log(L"移動中か : ", m_status(ePlayerStatus::IsMove));
		Debug::Log(L"待機中か : ", m_status(ePlayerStatus::IsIdle));
		Debug::Log(L"ローテーション中か : ", m_status(ePlayerStatus::IsRotate));
		Debug::Log(L"クラフトQTE中か : ", m_status(ePlayerStatus::IsCraftQTE));
		Debug::Log(L"木の所持状態は", m_status(ePlayerStatus::IsHaveWood) ? L"所持中 : " : L"未所持 : ", GetItemCount(eItemType::Wood), L"個");
		Debug::Log(L"石の所持状態は", m_status(ePlayerStatus::IsHaveStone) ? L"所持中 : " : L"未所持 : ", GetItemCount(eItemType::Stone), L"個");
		Debug::Log(L"レールの所持状態は", m_status(ePlayerStatus::IsHaveRail) ? L"所持中 : " : L"未所持 : ", GetItemCount(eItemType::Rail), L"個");
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
		SetAnimationMesh(ePAKey::Wait);

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

		// 採掘命令を送り、採掘できたらタグセットを受け取る
		const auto& miningTag = indicator->MiningOrder();

		// 採掘オブジェクトのタグセットが空じゃなければ採掘処理を送る
		if (!miningTag.empty())
		{
			MiningProcces(miningTag);
			return;
		}

		// レールを所持してたら
		if (GetItemCount(eItemType::Rail))
		{
			// 設置命令を送り、設置できたら所持数を減らす
			if (indicator->RailedOrder())
			{
				m_craft->UseItem(eItemType::Rail);
				StartSE(L"ADDRAIL_SE", 1.0f);
			}
		}
	}

	// 採掘処理
	void GamePlayer::MiningProcces(const set<wstring>& tagSet)
	{
		// ツールの採掘力に応じた取得数を設定
		//int addNum = GetToolsMiningValue();
		int addNum = 1; // ツールレベル概念が無い為一旦1で固定

		// 採掘対象マップを用いて採掘数を追加
		for (const auto& miningMap : m_miningMap)
		{
			if (tagSet.find(miningMap.first) != tagSet.end())
			{
				AddItemCount(miningMap.second, addNum);
				StartSE(miningMap.first + L"_SE", 1.0f);
			}
		}

		// 採掘状態にする
		m_status.Set(ePlayerStatus::IsMining) = true;
	}

	// クラフト状態でのXボタン入力
	void GamePlayer::SwitchCraftWindow()
	{
		// クラフト状態を切り替える
		m_status.Set(ePlayerStatus::IsCrafting) = !m_status(ePlayerStatus::IsCrafting);

		// クラフトマネージャーにクラフト状態を送る
		m_craft->CraftingEnabled(m_status(ePlayerStatus::IsCrafting));
	}

	// クラフトQTEが終わっているかの確認
	void GamePlayer::CheckedCraftQTE()
	{
		// QTEが終わったら
		if (m_craft->GetEndedQTE())
		{
			// QTE終了時の処理を送り、QtE状態を解除
			m_status.Set(ePlayerStatus::IsCraftQTE) = false;
			bool succes = m_craft->StopQTE();
			SetAnimationMesh(succes ? ePAKey::QTESucces : ePAKey::QTEFailed);
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

	// 移動更新
	void GamePlayer::UpdateMove()
	{
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
}