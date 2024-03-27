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
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_RED);

		// コリジョンOBBの追加
		m_ptrColl = AddComponent<CollisionObb>();

		// 重力の追加
		AddComponent<Gravity>();

		// ステートマシンの初期化
		m_stateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		m_stateMachine->ChangeState(PlayerMovingState::Instance());

		// ステージの取得
		const auto& stagePtr = GetStage();

		// セレクトインディケーターの生成
		m_indicator = stagePtr->AddGameObject<SelectIndicator>(GetThis<Player>());

		// クラフト機能の生成
		const auto& cWindow = stagePtr->AddGameObject<CraftWindow>();
		const auto& cQTE = stagePtr->AddGameObject<CraftingQTE>();
		m_craft = make_shared<CraftManager>(cWindow, cQTE);

		// タグの設定
		AddTag(L"Player");
	}

	// 毎フレーム更新処理
	void Player::OnUpdate()
	{
		// ステートマシンの更新処理を送る
		m_stateMachine->Update();

		// アイテム状態の更新
		UpdateItemStatus();

		// デバック用文字列
		Debug::Log(L"プレイヤーのステート : ", m_stateMachine->GetCurrentState()->GetStateName());
		Debug::Log(L"プレイヤーの座標 : ", GetPosition());
		Debug::Log(L"クラフトQTE中か : ", m_status(ePlayerStatus::IsCraftQTE));
		Debug::Log(L"木の所持状態は", m_status(ePlayerStatus::IsHaveWood) ? L"所持中 : " : L"未所持 : ", GetItemCount(eItemType::Wood), L"個");
		Debug::Log(L"石の所持状態は", m_status(ePlayerStatus::IsHaveStone) ? L"所持中 : " : L"未所持 : ", GetItemCount(eItemType::Stone), L"個");
		Debug::Log(L"レールの所持状態は", m_status(ePlayerStatus::IsHaveRail) ? L"所持中 : " : L"未所持 : ", GetItemCount(eItemType::Rail), L"個");
	}

	// 移動状態でのAボタン入力時
	void Player::MovingPushA()
	{
		// エラーチェック
		if (!m_indicator.lock()) return;

		// 採掘可能か、可能なら採掘可能オブジェクトのポインタを返す
		const auto& miningObj = m_indicator.lock()->GetMiningPossible();

		// 採掘可能オブジェクトのポインタがあれば
		if (miningObj)
		{
			// 採掘関数を返す
			MiningProcces(miningObj);
			return;
		}

		// レール設置用の座標を設定
		Vec3 railPos = m_indicator.lock()->GetPosition();
		railPos.y = 1.0f;

		// レールを設置可能かをインディケーターから取得
		if (m_indicator.lock()->GetRailedPossible(railPos))
		{
			// レールを所持してたら設置処理を送る
			if (GetItemCount(eItemType::Rail))
			{
				AddRailed(railPos);
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

		// タグに応じてアイテムを追加
		if (mining->FindTag(L"Tree"))
		{
			AddItemCount(eItemType::Wood, 1);
		}
		if (mining->FindTag(L"Rock"))
		{
			AddItemCount(eItemType::Stone, 1);
		}

		// 採掘状態にする
		m_status.Set(ePlayerStatus::IsMining) = true;
	}

	// レールの設置
	void Player::AddRailed(const Vec3& railPosition)
	{
		// 所属ステージの取得
		const auto& stagePtr = GetStage();

		// レールオブジェクトを生成してグループに追加
		const auto& rail = stagePtr->AddGameObject<Rail>(railPosition);
		stagePtr->GetSharedObjectGroup(L"Rails")->IntoGroup(rail);

		// レールの所持数を減らす
		AddItemCount(eItemType::Rail, -1);
	}
	
	// 採掘状態での待機処理
	void Player::MiningWaiting()
	{
		// 採掘時のアニメーション更新
		// UpdateAnimation(ePlayerStatus::IsMining);

		// 採掘中の待機時間
		// 本来ならアニメーション終了時間で状態遷移させるが
		// 現状はタイマーで待機時間を再現する
		if (SetTimer(0.1f))
		{
			m_status.Set(ePlayerStatus::IsMining) = false;
		}
	}

	// クラフト状態でのAボタン入力
	void Player::CraftingPushA()
	{
		// クラフトウィンドウが表示済みで、QTE中じゃなければ
		if (m_craft->GetShowCraftWindow() && !m_status(ePlayerStatus::IsCraftQTE))
		{
			// クラフト命令を送り、クラフト可能であればtrue
			if (m_craft->CraftOrder())
			{
				// QTE状態をオンにし、QTEを開始させる
				m_status.Set(ePlayerStatus::IsCraftQTE) = true;
				m_craft->StartQTE();
			}
			return;
		}

		// クラフトQTE
		if (m_status(ePlayerStatus::IsCraftQTE))
		{
			// クラフトマネージャにQTEのバーの停止を送る
			m_craft->StopQTE();
			m_status.Set(ePlayerStatus::IsCraftQTE) = false;
		}
	}

	// クラフト状態でのXボタン入力
	void Player::CraftingPushX()
	{
		// Xボタンが入力され、QTE状態じゃなければ
		if (!m_status(ePlayerStatus::IsCraftQTE))
		{
			// クラフト状態を切り替える
			m_status.Set(ePlayerStatus::IsCrafting) = !m_status(ePlayerStatus::IsCrafting);

			// クラフトマネージャーにクラフト状態を送る
			m_craft->CraftingEnabled(m_status(ePlayerStatus::IsCrafting));
		}
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

	// 移動更新
	void Player::UpdateMove()
	{
		// LStickの入力があり、クラフト中じゃなければ
		bool isMoving = IsInputLStick();
		if (isMoving)
		{
			// LStick入力量の取得
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			ControllerRotation(stickValue); // 回転関数
			ControllerMovement(stickValue);	// 移動関数
		}

		// 移動状態を設定
		m_status.Set(ePlayerStatus::IsMove) = isMoving;
		m_status.Set(ePlayerStatus::IsIdle) = !isMoving;
	}

	// アイテム状態の更新
	void Player::UpdateItemStatus()
	{
		// アイテム状態の更新(今後アイテムの追加があれば適宜追加)
		m_status.Set(ePlayerStatus::IsHaveRail) = GetItemCount(eItemType::Rail);
		m_status.Set(ePlayerStatus::IsHaveWood) = GetItemCount(eItemType::Wood);
		m_status.Set(ePlayerStatus::IsHaveStone) = GetItemCount(eItemType::Stone);
	}

	// コントローラーによる回転
	void Player::ControllerRotation(const Vec3& stickValue)
	{
		// スティックの傾きに合わせてオブジェクトを回転させる
		float rotY = atan2f(-stickValue.z, stickValue.x);
		SetRotation(Vec3(0.0f, rotY, 0.0f));
	}
	
	// コントローラーによる移動
	void Player::ControllerMovement(const Vec3& stickValue)
	{
		// 現在の座標に入力量×速度×デルタタイムで加算
		Vec3 pos = GetPosition();
		pos += stickValue * m_speed * DELTA_TIME;
		
		// 座標の更新
		SetPosition(pos);
	}
}