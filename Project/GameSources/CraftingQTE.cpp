/*!
@file CraftingQTE.cpp
@brief クラフト中のQTE
@author 小澤博貴
*/

#include "stdafx.h"
#include "CraftingQTE.h"

namespace basecross
{
	// 生成時の処理
	void CraftingQTE::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Sprite::OnCreate();

		// ステージの取得
		const auto& stagePtr = GetStage();

		// フレームとQTEポイントとボタン入力を生成
		m_barFlame = stagePtr->AddGameObject<CraftUI>(L"C_QTE_FLAME_TX", m_drawScale, m_drawSpeed);
		m_qtePoint = stagePtr->AddGameObject<CraftUI>(L"WHITE_TX", Vec2(40.0f, 10.0f), m_drawSpeed);
		m_qteButton = stagePtr->AddGameObject<CraftUI>(L"BUTTON_A_TX", Vec2(50.0f), m_drawSpeed);

		// レイヤー設定
		SetDrawLayer(5);
		m_barFlame.lock()->SetDrawLayer(7);
		m_qtePoint.lock()->SetDrawLayer(6);
		m_qteButton.lock()->SetDrawLayer(7);

		// 色を変更
		m_qtePoint.lock()->SetDiffuseColor(Col4(0.0f, 0.0f, 1.0f, 0.75f));
	}

	// 毎フレーム更新処理
	void CraftingQTE::OnUpdate()
	{
		// 継承元の毎フレーム更新処理を実行
		CraftUI::OnUpdate();

		// QTE中ならQTEの更新処理を実行
		if (m_qteEnable)
		{
			UpdateQTE();
		}
	}

	// ウィンドウの更新
	void CraftingQTE::DrawWindow()
	{
		// 継承元のウィンドウの更新処理を実行
		CraftUI::DrawWindow();

		// Y軸のスケールは0の状態にする
		SetScale(0.0f, m_scale.y);
	}

	// 表示切り替え
	void CraftingQTE::SetDrawEnable(bool enable, Vec3 windowPos)
	{
		// 継承元の表示切替え処理を実行
		CraftUI::SetEnable(enable);

		// 各種ポインタ
		auto& flame = m_barFlame.lock();
		auto& point = m_qtePoint.lock();
		auto& button = m_qteButton.lock();

		// フレームとQTEポイントにも表示切替えの処理を送る
		flame->SetEnable(enable);
		point->SetEnable(enable);

		// 座標を設定
		Vec3 diff = m_rectDiff.at(m_rectType);
		Vec3 pos = windowPos + Vec3(0.0f, (m_posDiff + m_drawScale.y) * diff.y, 0.0f);
		SetPosition(Vec3(pos.x, pos.y, 0.2f));
		flame->SetPosition(Vec3(pos.x, pos.y, 0.0f));
		point->SetPosition(Vec3(pos.x + (m_drawScale.x * (m_qteRatio - m_qteRange)) * diff.x, pos.y, 0.1f));
		button->SetPosition(point->GetPosition() + Vec3(point->m_drawScale.x / 2.0f * diff.x, button->m_drawScale.y / 1.25f * diff.y, 0.0f));
	}
	
	// 描画変更設定
	void CraftingQTE::SetVerticesRect(eRectType rect)
	{
		// 継承元の描画変更設定処理を送る
		CraftUI::SetVerticesRect(rect);

		// フレームとQTEポイントにも描画変更設定の処理を送る
		m_barFlame.lock()->SetVerticesRect(rect);
		m_qtePoint.lock()->SetVerticesRect(rect);
	}

	// QTEの更新処理
	void CraftingQTE::UpdateQTE()
	{
		// 割合をデルタタイムとバーの速度で加算
		m_barRatio += DELTA_TIME / m_barSpeed;
		m_barRatio = min(m_barRatio, 1.0f);
		m_barRatio = max(m_barRatio, 0.0f);

		// スケールXを割合を使ってLerpで求める
		float scaleX = Utility::Lerp(0.0f, m_drawScale.x, m_barRatio);
		SetScale(scaleX, m_drawScale.y);

		// 始点or終点になったのならQTEを終了
		if (m_barRatio == 1.0f)
		{
			m_barRatio = 0.0f;
			m_qteEnable = false;
		}
	}

	// QTEの開始処理
	void CraftingQTE::StartQTE(const Vec3& windowPos)
	{
		// 開始時の初期化を行う
		m_barRatio = 0.0f;
		m_qteEnable = true;
		m_qteButton.lock()->SetEnable(true);
	}

	// QTEの停止処理
	bool CraftingQTE::StopQTE()
	{
		// QTEを停止
		m_qteEnable = false;
		m_qteButton.lock()->SetEnable(false);

		// Y軸のスケールは0の状態にする
		SetScale(0.0f, m_scale.y);

		// QTEの範囲を設定
		const float upper = m_qteRatio + m_qteRange;
		const float under = m_qteRatio - m_qteRange;

		// バーの割合がQTEの範囲内かを返す
		return Utility::GetBetween(m_barRatio, upper, under);
	}
}