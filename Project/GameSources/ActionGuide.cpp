/*!
@file ActionGuide.cpp
@brief 行動のガイドアイコン描画
@author 小澤博貴
*/

#include "stdafx.h"
#include "ActionGuide.h"
#include "RailManager.h"

namespace basecross
{
	// 生成時の処理を実行
	void ActionGuide::OnCreate()
	{
		// スプライトの生成
		const auto& stagePtr = GetStage();
		m_spriteMap.push_back(SpriteType(eIconPivot::Center, eSpriteType::Icon,    stagePtr->AddGameObject<Sprite>()));
		m_spriteMap.push_back(SpriteType(eIconPivot::Center, eSpriteType::Balloon, stagePtr->AddGameObject<Sprite>(L"I_BALOON_CENTER_TX")));
		m_spriteMap.push_back(SpriteType(eIconPivot::Right,  eSpriteType::Icon,    stagePtr->AddGameObject<Sprite>()));
		m_spriteMap.push_back(SpriteType(eIconPivot::Right,  eSpriteType::Balloon, stagePtr->AddGameObject<Sprite>(L"I_BALOON_LEFT_TX")));
		m_spriteMap.push_back(SpriteType(eIconPivot::Left,   eSpriteType::Icon,    stagePtr->AddGameObject<Sprite>()));
		m_spriteMap.push_back(SpriteType(eIconPivot::Left,   eSpriteType::Balloon, stagePtr->AddGameObject<Sprite>(L"I_BALOON_RIGHT_TX")));

		// スケールの設定
		for (auto& sprite : m_spriteMap)
		{
			sprite.sprite.lock()->SetScale(m_scale);
			sprite.sprite.lock()->SetDrawLayer(3);
		}

		SetDrawActive(false);
	}

	// 毎フレーム更新処理
	void ActionGuide::OnUpdate()
	{
		// アクティブを初期化
		SetSpriteActive(false);

		// コントローラーの接続状態を保持
		m_currentDevice = Input::GetPadConected();

		// アイコンタイプの更新
		UpdateIconType();

		// アイコンの描画更新
		UpdateIconSet();

		// ビルボードと座標の更新
		UpdatePosition();

		// 描画の更新
		UpdateIconDraw();
	}

	// 座標の更新
	void ActionGuide::UpdatePosition()
	{
		// プレイヤーが存在するかのエラーチェック
		const auto& player = m_player.lock();
		if (!player) return;

		// プレイヤーの座標+頭上分の座標
		Vec3 headPos = player->GetPosition() + m_diffPosition;
		
		// 2D座標に変換して座標を更新
		const auto& view = GetStage()->GetView();
		m_position = Utility::ConvertToWorldPosition(view, headPos);
		m_position.z = 0.0f;
	}

	// アイコンタイプの更新
	void ActionGuide::UpdateIconType()
	{
		m_iconBuffer.clear(); // バッファの初期化

		// プレイヤーとインディケーターが存在するか
		const auto& player = m_player.lock();
		const auto& indicator = m_indicator.lock();
		if (!player || !indicator) return;

		// クラフト可能ならクラフトアイコン
		auto& texMap = m_texMap.at(m_currentDevice);
		if (player->GetCraftPosshible()) m_iconBuffer.insert(make_pair(texMap.at(eActionIcon::Craft).layerIndex, eActionIcon::Craft));

		// アクションできるポイントを選択してなければ無視
		if (!IsActionSelectPoint(player, indicator)) return;

		// IDに応じてアイコンタイプを設定
		eStageID id = STAGE_ID(GetIntValueStageID(indicator));

		// アイコンバッファに追加
		for (const auto& type : m_iconMap)
		{
			if (id != type.first) continue;

			m_iconBuffer.insert(make_pair(texMap.at(type.second).layerIndex, type.second));
		}
	}

	// アイコンをスプライトに設定
	void ActionGuide::UpdateIconSet()
	{
		// 優先度インデックス配列の設定
		vector<int> indices;
		SortedIndicesBuffer(indices);
		if (indices.empty()) return;

		// 表示数に応じてアイコンの設定方法を変更
		SetIndicesIconTexture(indices);
	}

	// アイコン描画更新
	void ActionGuide::UpdateIconDraw()
	{
		bool isCrafting = m_player.lock()->GetStatus(ePlayerStatus::IsCrafting);
		bool isStaging = GetTypeStage<BaseStage>()->GetIsStaging();

		// スプライト配列を全ループ
		for (auto& sprite : m_spriteMap)
		{
			// 描画するかの真偽設定
			sprite.sprite.lock()->SetDrawActive(sprite.active && !isCrafting && !isStaging);

			// 非アクティブなら無視
			if (!sprite.active) continue;

			//複数なら
			if (m_iconBuffer.size() > 1)
			{
				Vec3 diss = (sprite.pivot != eIconPivot::Left) ? Vec3(m_distanceX, 0.0f, 0.0f) : Vec3(-m_distanceX, 0.0f, 0.0f);
				sprite.sprite.lock()->SetPosition(m_position - diss + m_typePos.at(sprite.type));
			}
			else // 1つなら
			{
				sprite.sprite.lock()->SetPosition(m_position + m_typePos.at(sprite.type));
			}
		}
	}

	// アクションできるポイントを選択しているか
	bool ActionGuide::IsActionSelectPoint(const shared_ptr<Player>& player, const shared_ptr<SelectIndicator>& indicator)
	{
		// 真偽
		bool isGuide = (indicator->IsGuideRailPoint() && player->GetItemCount(eItemType::Rail));
		bool isBridge = (indicator->IsWaterPoint() && player->GetItemCount(eItemType::WoodBridge));
		bool isCross = (indicator->IsStraightRailPoint() && player->GetItemCount(eItemType::Crossing));
		bool isGathering = indicator->IsGatheringPoint();

		// どれか1つでも選択しているなら
		return (isGuide || isBridge || isCross || isGathering);
	}

	// ステージIDを取得
	int ActionGuide::GetIntValueStageID(const shared_ptr<SelectIndicator>& indicator)
	{
		// ステージcsvIDに応じて設定
		const auto& stageMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetGuideMap();
		const auto& selectPoint = indicator->GetSelectPoint();
		if (!Utility::WithInElemRange(selectPoint.x, selectPoint.y, stageMap)) return 0;

		return stageMap.at(selectPoint.x).at(selectPoint.y);
	}

	// インデックスのソート
	void ActionGuide::SortedIndicesBuffer(vector<int>& indices)
	{
		auto& texMap = m_texMap.at(m_currentDevice);
		for (const auto& buff : m_iconBuffer)
		{
			// アクションアイコンと一致したら追加
			if (texMap.find(buff.second) != texMap.end())
			{
				indices.push_back(texMap.at(buff.second).layerIndex);
			}
		}

		// レイヤーのソート
		sort(indices.begin(), indices.end());
	}

	// 表示数に応じてアイコンの設定方法を変更
	void ActionGuide::SetIndicesIconTexture(vector<int>& indices)
	{
		int loopNum = 0; // ループ数
		size_t buffSize = m_iconBuffer.size(); // バッファサイズ

		// スプライト配列を全ループ
		for (auto& sprite : m_spriteMap)
		{
			bool isCenterPivot = (sprite.pivot == eIconPivot::Center); // センターかどうか
			bool isNotBalloon = (sprite.type != eSpriteType::Balloon); // 吹き出しじゃないか

			// 表示数が１ならセンターに、複数ならセンター以外に
			if ((buffSize == 1 && isCenterPivot) || (buffSize > 1 && !isCenterPivot))
			{
				sprite.active = true; // アクティブに

				// 吹き出しならこれ以降は無視
				if (!isNotBalloon) continue;
				
				// バッファが1つなら先頭の、複数ならループ数分先頭から進んだイテレータを取得し設定
				auto textureIter = (buffSize == 1) ? m_iconBuffer.begin()->second : next(m_iconBuffer.begin(), loopNum)->second;
				sprite.sprite.lock()->SetTexture(m_texMap.at(m_currentDevice).at(textureIter).textureStr);
				loopNum++;
			}
		}
	}
}