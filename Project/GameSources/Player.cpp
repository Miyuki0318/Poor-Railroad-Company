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

		// ステージの取得
		const auto& stagePtr = GetStage();

		// セレクトインディケーターの生成
		m_indicator = stagePtr->AddGameObject<SelectIndicator>(GetThis<Player>());

		// クラフト機能の生成
		const auto& cWindow = stagePtr->AddGameObject<CraftWindow>();
		m_craft = make_shared<CraftManager>(cWindow);

		// タグの設定
		AddTag(L"Player");
	}

	// 毎フレーム更新処理
	void Player::OnUpdate()
	{
		// Aボタンが入力されたら
		if (GetPushA())
		{
			// Aボタン入力時の処理をクラフト状態で分岐させる
			m_status(ePlayerStatus::IsCrafting) ? OnCraft() : OnPushA();
		}

		// Xボタンが入力されたら
		if (GetPushX())
		{
			// クラフト状態を切り替える
			m_status.Set(ePlayerStatus::IsCrafting) = !m_status(ePlayerStatus::IsCrafting);

			// クラフトマネージャーにクラフト状態を送る
			m_craft->CraftingEnabled(m_status(ePlayerStatus::IsCrafting));
		}

		// 移動更新
		UpdateMove();

		// アイテム状態の更新
		m_status.Set(ePlayerStatus::IsHaveRail) = GetItemCount(eItemType::Rail);
		m_status.Set(ePlayerStatus::IsHaveWood) = GetItemCount(eItemType::Wood);
		m_status.Set(ePlayerStatus::IsHaveStone) = GetItemCount(eItemType::Stone);

		// デバック用文字列
		Debug::Log(L"プレイヤーの座標 : ", GetPosition());
		Debug::Log(L"移動中か : ", m_status(ePlayerStatus::IsMove));
		Debug::Log(L"クラフト中か : ", m_status(ePlayerStatus::IsCrafting));
		Debug::Log(L"木の所持状態は", m_status(ePlayerStatus::IsHaveWood) ? L"所持中 : " : L"未所持 : ", GetItemCount(eItemType::Wood), L"個");
		Debug::Log(L"石の所持状態は", m_status(ePlayerStatus::IsHaveStone) ? L"所持中 : " : L"未所持 : ", GetItemCount(eItemType::Stone), L"個");
		Debug::Log(L"レールの所持状態は", m_status(ePlayerStatus::IsHaveRail) ? L"所持中 : " : L"未所持 : ", GetItemCount(eItemType::Rail), L"個");
	}

	// Aボタン入力時
	void Player::OnPushA()
	{
		// エラーチェック
		if (!m_indicator.lock()) return;

		// 採掘可能か、可能なら採掘可能オブジェクトのポインタを返す
		const auto& miningObj = m_indicator.lock()->GetMiningPossible();

		// 採掘可能オブジェクトのポインタがあれば
		if (miningObj)
		{
			// 採掘関数を返す
			OnMining(miningObj);
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
				OnRailed(railPos);
			}
			return;
		}
	}

	// クラフト呼び出し
	void Player::OnCraft()
	{
		// クラフトのみ送っているが、α版でQTEに移行させる
		m_craft->Crafting(GetThis<Player>());
	}

	// 採掘呼び出し
	void Player::OnMining(const shared_ptr<TemplateObject>& miningObj)
	{
		// 採掘可能オブジェクトに型キャストして
		// 採掘呼び出し関数を送り
		// タグに応じてアイテムカウンタを増加
	}

	// レールの設置呼び出し
	void Player::OnRailed(const Vec3& railPosition)
	{
		// 所属ステージの取得
		const auto& stagePtr = GetStage();

		// レールオブジェクトを生成してグループに追加
		const auto& rail = stagePtr->AddGameObject<Rail>(railPosition);
		stagePtr->GetSharedObjectGroup(L"Rails")->IntoGroup(rail);

		// レールの所持数を減らす
		AddItemCount(eItemType::Rail, -1);
	}
	
	// 移動更新
	void Player::UpdateMove()
	{
		// LStickの入力があり、クラフト中じゃなければ
		bool isMoving = IsInputLStick() && !m_status(ePlayerStatus::IsCrafting);
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