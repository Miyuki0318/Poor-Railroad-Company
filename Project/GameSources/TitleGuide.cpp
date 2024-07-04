/*!
@file TitleGuide.cpp
@brief タイトルステージのガイドアイコン描画
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void TitleGuide::OnCreate()
	{
		// 生成時の処理
		Sprite::OnCreate();

		// 描画設定
		SetDrawLayer(1);
		SetDrawEnable(false);
		SetVerticesRect(eRectType::UpLeft);

		// マップにタイトルステージオブジェクトを追加
		const auto& stagePtr = GetTypeStage<TitleStage>();
		m_titleObjMap.emplace(L"ROUTEMAP_TX", stagePtr->GetSharedGameObject<TemplateObject>(L"RouteMap"));
		m_titleObjMap.emplace(L"START_TRAIN_TX", stagePtr->GetSharedGameObject<TemplateObject>(L"TitleTrain"));
		m_titleObjMap.emplace(L"COMPANIY_TX", stagePtr->GetSharedGameObject<TemplateObject>(L"Company"));

		// プレイヤーの保持
		m_playerPtr = stagePtr->GetSharedGameObject<TemplateObject>(L"Player");
	}

	// 毎フレーム更新処理
	void TitleGuide::OnUpdate()
	{
		// 継承元の更新処理
		CraftUI::OnUpdate();

		// タイトルステージがイベント中なら非表示にして終了
		bool isStaging = GetTypeStage<TitleStage>()->GetIsStaging();
		if (isStaging)
		{
			SetDrawEnable(false);
			return;
		}

		// 距離で比較し、テクスチャを設定
		if (!SetRangeTexture()) return;

		// 座標の更新処理
		UpdatePosition();
	}

	// テクスチャの設定
	bool TitleGuide::SetRangeTexture()
	{
		// プレイヤーの取得
		const auto& player = m_playerPtr.lock();
		if (!player) return false;

		// プレイヤーとの距離を比較し、範囲内ならテクスチャを設定
		bool isRange = false;
		Vec3 playerPos = player->GetPosition();
		for (auto& obj : m_titleObjMap)
		{
			// オブジェクトの座標を取得し、距離を求める
			Vec3 objPos = obj.second.lock()->GetPosition();
			float length = (playerPos - objPos).length();

			// 範囲内じゃなければ無視
			if (length >= m_drawRange) continue;

			// テクスチャを設定し、表示させる
			isRange = true;
			SetTexture(obj.first);
			SetDrawEnable(true);
		}

		// 範囲内に存在しなければ非表示にする
		if (!isRange) SetDrawEnable(false);

		// 範囲内に存在したかを返す
		return isRange;
	}

	// 座標の更新
	void TitleGuide::UpdatePosition()
	{
		// プレイヤーの取得
		const auto& player = m_playerPtr.lock();
		if (!player) return;

		// ビューとプレイヤーの座標から2D座標を取得
		const auto& view = GetStage()->GetView();
		Vec3 playerPos = player->GetPosition();
		Vec3 spritePos = Utility::ConvertToWorldPosition(view, playerPos);

		// Z軸を0に矯正し、差分と合わせた座標で更新
		spritePos.z = 0.0f;
		SetPosition(spritePos + m_diffPosition);
	}
}