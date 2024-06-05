/*!
@file ActionGuide.cpp
@brief 行動のガイドアイコン描画
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

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
		}

		SetDrawActive(false);
	}

	// 毎フレーム更新処理
	void ActionGuide::OnUpdate()
	{
		// アクティブを初期化
		SetSpriteActive(false);

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
		// バッファの初期化
		m_iconBuffer.clear();

		// プレイヤーとインディケーターが存在するか
		const auto& player = m_player.lock();
		const auto& indicator = m_indicator.lock();
		if (!player || !indicator) return;

		// クラフト可能ならクラフトアイコン
		if (player->GetCraftPosshible()) m_iconBuffer.insert(make_pair(m_texMap.at(eActionIcon::Craft).layerIndex, eActionIcon::Craft));

		// ステージcsvIDに応じて設定
		const auto& stageMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetGuideMap();
		const auto& selectPoint = indicator->GetSelectPoint();
		if (!Utility::WithInElemRange(selectPoint.x, selectPoint.y, stageMap)) return;

		// IDに応じてアイコンタイプを設定
		eStageID id = STAGE_ID(stageMap.at(selectPoint.x).at(selectPoint.y));

		// ガイドレールかの真偽と採掘オブジェクトかの真偽
		bool isGuide = (id == eStageID::GuideRail && player->GetStatus(ePlayerStatus::IsHaveRail));
		bool isMining = (id != eStageID::GuideRail);

		for (const auto& type : m_iconMap)
		{
			if (id != type.first) continue;

			// idとタイプが一致するなら
			if (isGuide) m_iconBuffer.insert(make_pair(m_texMap.at(type.second).layerIndex, type.second));
			if (isMining) m_iconBuffer.insert(make_pair(m_texMap.at(type.second).layerIndex, type.second));
		}
	}

	// アイコンをスプライトに設定
	void ActionGuide::UpdateIconSet()
	{
		// 優先度インデックス配列の設定
		vector<int> indices;
		size_t buffSize = m_iconBuffer.size();
		for (const auto& buff : m_iconBuffer)
		{
			// アクションアイコンと一致したら追加
			if (m_texMap.find(buff.second) != m_texMap.end())
			{
				indices.push_back(m_texMap.at(buff.second).layerIndex);
			}
		}

		// レイヤーのソート
		sort(indices.begin(), indices.end());
		if (indices.empty()) return;

		// 表示数に応じてアイコンの設定方法を変更
		int loopNum = 0;
		for (auto& sprite : m_spriteMap)
		{
			bool isCenterPivot = (sprite.pivot == eIconPivot::Center); // センターかどうか
			bool isNotBalloon = (sprite.type != eSpriteType::Balloon); // 吹き出しじゃないか

			// 表示数が１ならセンターに、複数ならセンター以外に
			if ((buffSize == 1 && isCenterPivot) || (buffSize > 1 && !isCenterPivot))
			{
				if (isNotBalloon)
				{
					auto textureIter = (buffSize == 1) ? m_iconBuffer.begin()->second : std::next(m_iconBuffer.begin(), loopNum)->second;
					sprite.sprite.lock()->SetTexture(m_texMap.at(textureIter).textureStr);
					loopNum++;
				}
				sprite.active = true;
			}
		}
	}

	// アイコン描画更新
	void ActionGuide::UpdateIconDraw()
	{
		bool isCrafting = m_player.lock()->GetStatus(ePlayerStatus::IsCrafting);

		for (auto& sprite : m_spriteMap)
		{
			// アクティブなら
			if (sprite.active)
			{
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

			// 描画真偽設定
			sprite.sprite.lock()->SetDrawActive(sprite.active && !isCrafting);
		}
	}
}