/*!
@file GatherEffect.cpp
@brief お宝機能
@author 小澤博貴
*/

#include "stdafx.h"
#include "OzawaUtility.h"
#include "GatherTreasure.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void GatherEffect::OnCreate()
	{
		// 継承元の生成時の処理を送る
		Billboard::OnCreate();

		// 非アクティブに
		SetDrawActive(false);
		SetUpdateActive(false);
	}

	// 毎フレーム更新処理
	void GatherEffect::OnUpdate()
	{
		// サインカーブ等に使うラジアン
		float rad = m_totalTime * XM_2PI;

		// カメラを取得
		const auto& camera = GetStage()->GetView()->GetTargetCamera();

		// トランスフォームの計算
		Vec3 pos = SinCurve(rad, m_startPos, m_startPos + m_boundVal);
		Quat rot = GetBillboardRotateQuat(camera->GetAt() - camera->GetEye(), Vec3(0.0f, rad, 0.0f));
		Vec2 scale = Utility::Lerp(Vec2(0.0f), m_drawScale, m_totalTime / m_effectTime);

		// 経過時間を加算
		m_totalTime += DELTA_TIME;

		// 描画時間以上になったら非アクティブに
		if (m_totalTime >= m_effectTime)
		{
			SetDrawActive(false);
			SetUpdateActive(false);
		}

		// トランスフォームの更新
		SetScale(scale);
		SetPosition(pos);
		SetQuaternion(rot);

		// 色の更新処理
		SetDiffuseColor(Col4(1.0f, 1.0f, 1.0f, m_totalTime / m_effectTime));
	}

	// 開始処理
	void GatherEffect::StartDraw(const Vec3& position)
	{
		// 座標を設定
		SetPosition(position);
		m_startPos = position;
		
		// 経過時間を初期化
		m_totalTime = 0.0f;

		// アクティブに
		SetDrawActive(true);
		SetUpdateActive(true);

		// SEの再生
		StartSE(L"C_SUCCES_SE", 1.0f);
	}

	// 宝箱が出るか、出たらアイテムデータを返す
	pair<eItemType, int> GatherTreasure::TreasureCheck(const Vec3& position)
	{
		// 1割りの確率を設ける
		int random = rand() % 10;
		if (m_probability > random) return make_pair(eItemType::None, 0);

		// アイテムタイプと加算数を設定
		int max = sizeof(m_items) / sizeof(eItemType);
		eItemType addType = m_items[rand() % max];
		int addVal = m_addVal.at(addType);
		auto ret = make_pair(addType, addVal);

		// 座標を設定
		Vec3 effectPos = position + UP_VEC;

		// スプライト配列に非アクティブがあるならそれを扱う
		for (auto& weakPtr : m_effectVec)
		{
			// エラーチェック
			auto& sprite = weakPtr.lock();
			if (!sprite) continue;

			// 非アクティブなら開始処理を送り終了
			if (!sprite->GetUpdateActive())
			{
				sprite->StartDraw(effectPos);
				return ret;
			}
		}

		// 非アクティブが無かったので生成して開始処理を送る
		auto& newSprite = GetStage()->AddGameObject<GatherEffect>();
		newSprite->StartDraw(effectPos);

		// 配列に追加
		m_effectVec.push_back(newSprite);
		return ret;
	}
}