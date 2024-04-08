/*!
@file Player.cpp
@brief プレイヤー
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
	void Player::OnCreate()
	{
		// 継承元の生成時の処理を実行
		TemplateObject::OnCreate();
		SetTransParam();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->SetMultiMeshResource(L"PLAYER");
		m_ptrDraw->SetSpecularColor(COL_BLACK);
		m_ptrDraw->SetOwnShadowActive(true);
		m_ptrDraw->SetLightingEnabled(false);

		// コリジョンOBBの追加
		m_ptrColl = AddComponent<CollisionCapsule>();

		// 重力の追加
		AddComponent<Gravity>();

		// ステートマシンの初期化
		m_playerState.reset(new PlayerStateMachine(GetThis<Player>()));
		m_playerState->SetState(PlayerIdleState::Instance());

		// ステージの取得(shared_ptrをconstで取得)
		const auto& stagePtr = GetStage();

		// セレクトインディケーターの生成
		m_indicator = stagePtr->AddGameObject<SelectIndicator>(GetThis<Player>());

		// クラフトマネージャの生成
		const auto& cWindow = stagePtr->AddGameObject<CraftWindow>();
		const auto& cQTE = stagePtr->AddGameObject<CraftingQTE>();
		m_craft.reset(new CraftManager(cWindow, cQTE));

		// タグの設定
		AddTag(L"Player");
	}

	// 毎フレーム更新処理
	void Player::OnUpdate()
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

	// インディケーターの取得に応じて処理
	void Player::IndicatorOrder()
	{
		// エラーチェック
		const auto& indicator = m_indicator.lock();
		if (!indicator) return;

		// 採掘可能か、可能なら採掘可能オブジェクトのポインタを返す
		const auto& miningObj = indicator->GetMiningPossible();

		// 採掘可能オブジェクトのポインタがあれば
		if (miningObj)
		{
			// 採掘関数を返す
			MiningProcces(miningObj);
			return;
		}

		// レールを所持してたら設置処理を送る
		if (GetItemCount(eItemType::Rail))
		{
			// レールを設置可能かをインディケーターから取得
			if (indicator->GetRailedPossible())
			{
				// レールの所持数を減らす
				m_craft->UseItem(eItemType::Rail);
			}
		}
	}
		
	// 採掘処理
	void Player::MiningProcces(const shared_ptr<TemplateObject>& miningObj)
	{
		// 採掘可能オブジェクトに型キャスト
		const auto& mining = dynamic_pointer_cast<MiningObject>(miningObj);
		if (!mining) return;
		
		// 採掘オブジェクトに採掘処理を送る
		mining->OnMining();

		// ツールの採掘力に応じた取得数を設定
		//int addNum = GetToolsMiningValue();
		int addNum = 1; // ツールレベル概念が無い為一旦1で固定

		// 採掘対象マップを用いて採掘数を追加
		for (const auto& miningMap : m_miningMap)
		{
			if (mining->FindTag(miningMap.first))
			{
				AddItemCount(miningMap.second, addNum);
			}
		}

		// 採掘状態にする
		m_status.Set(ePlayerStatus::IsMining) = true;
	}

	// クラフト状態でのXボタン入力
	void Player::SwitchCraftWindow()
	{
		// クラフト状態を切り替える
		m_status.Set(ePlayerStatus::IsCrafting) = !m_status(ePlayerStatus::IsCrafting);

		// クラフトマネージャーにクラフト状態を送る
		m_craft->CraftingEnabled(m_status(ePlayerStatus::IsCrafting));
	}

	// クラフトQTEが終わっているかの確認
	void Player::CheckedCraftQTE()
	{
		// QTEが終わったら
		if (m_craft->GetEndedQTE())
		{
			// QTE終了時の処理を送り、QtE状態を解除
			m_craft->StopQTE();
			m_status.Set(ePlayerStatus::IsCraftQTE) = false;
		}
	}

	// アイテム状態の更新
	void Player::UpdateStatus()
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
	void Player::UpdateMove()
	{
		// LStickの入力があるなら
		bool isLStick = IsInputLStick();
		if (isLStick)
		{
			// LStick入力量の取得
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			SetRotateTarget(stickValue); // 回転関数
			ControllerMovement(stickValue);	// 移動関数
		}

		// 移動状態を設定
		m_status.Set(ePlayerStatus::IsMove) = isLStick;
	}

	// 回転先設定
	void Player::SetRotateTarget(const Vec3& stickValue)
	{
		// コントローラーの入力から回転方向ベクトルを設定
		float rotY = atan2f(-stickValue.z, stickValue.x);
		m_rotTarget = Vec3(cos(rotY), 0.0f, sin(rotY));
	}

	// 回転更新処理
	void Player::UpdateRotation()
	{
		// 回転方向ベクトルと現在の回転ベクトルの差分から回転量を設定
		Vec3 rot = m_rotTarget - m_currentRot;
		float rotate = rot.length() * (DELTA_TIME / m_rotSpeed);
		m_currentRot += Vec3(rot.normalize() * rotate);

		// 回転量を加算された現在の回転ベクトルのラジアンをY軸として設定
		float rotY = atan2f(m_currentRot.z, m_currentRot.x);
		SetRotation(Vec3(0.0f, rotY, 0.0f));
	}
	
	// コントローラーによる移動
	void Player::ControllerMovement(const Vec3& stickValue)
	{
		// 現在の座標に入力量×速度×デルタタイムで加算
		Vec3 pos = GetPosition();
		pos += stickValue * m_moveSpeed * DELTA_TIME;
		
		// 座標の更新
		SetPosition(pos);
	}
}