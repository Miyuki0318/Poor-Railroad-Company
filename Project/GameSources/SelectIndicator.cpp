/*!
@file SelectIndicator.cpp
@brief セレクトインディケーター
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void SelectIndicator::OnCreate()
	{
		// 継承元の生成時の処理を実行
		TemplateObject::OnCreate();

		// 頂点データの設定
		const float HELF = 0.55f;
		vector<Vec3> point = {
			Vec3(HELF, 0.0f, HELF),
			Vec3(HELF, 0.0f, -HELF),
			Vec3(-HELF, 0.0f, -HELF),
			Vec3(-HELF, 0.0f, HELF),
			Vec3(HELF, 0.0f, HELF),
		};

		// リボン型の頂点データを作成
		RibonVerticesIndices(point, m_vertex, Vec3(0.0f, 0.0f, 0.5f), HELF, 1);

		// 頂点データの数ループ
		for (auto& vertices : m_vertex.vertices)
		{
			// 座標Y軸が上側であれば、透明色にする
			if (vertices.position.y > 0.0f)
			{
				vertices.color = COL_ALPHA;
			}
		}

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PCTStaticDraw>();
		m_ptrDraw->SetOriginalMeshUse(true);
		m_ptrDraw->CreateOriginalMesh(m_vertex);

		// 透明色の描画を可能に
		SetAlphaActive(true);
	}

	// 毎フレーム更新処理
	void SelectIndicator::OnUpdate()
	{
		// プレイヤーのポインタがあるか
		if (m_player.lock())
		{
			const auto& player = m_player.lock();

			// プレイヤーの回転角Y軸の中から90の倍数に一番近いのに設定
			float rotY = GetClosest(player->GetRotation().y, 0.0f, XM_PIDIV2, XM_PI, -XM_PIDIV2, -XM_PI);
			
			// 方向ベクトルを定義
			Vec3 velo = Vec3(cosf(rotY), 0.0f, -sinf(rotY));
			velo.x = round(velo.x);
			velo.z = round(velo.z);

			// プレイヤーの座標を四捨五入する
			Vec3 pos = player->GetPosition();
			pos.x = round(pos.x);
			pos.z = round(pos.z);
			pos.y = 1.5f;

			// プレイヤーの座標+方向ベクトルで座標更新
			SetPosition(pos + velo);
		}
	}

	// 採掘可能オブジェクトか、採掘可能オブジェクトポインタの取得
	const shared_ptr<TemplateObject> SelectIndicator::GetMiningPossible() const
	{
		// 自身の座標を取得
		Vec3 indiPos = GetPosition();
		indiPos.y = 1.5f;

		// 採掘可能オブジェクト配列の取得
		const auto& miningVec = GetStage()->GetSharedObjectGroup(L"MiningObject")->GetGroupVector();

		// 配列の数ループ
		for (const auto& weakObj : miningVec)
		{
			// エラーチェック
			if (!weakObj.lock()) continue;
			if (!weakObj.lock()->IsUpdateActive()) continue;

			// 型キャスト
			const auto& miningObj = dynamic_pointer_cast<TemplateObject>(weakObj.lock());
			if (!miningObj) continue;

			// 座標が一致したらポインタを返す
			if (indiPos == miningObj->GetPosition())
			{
				return miningObj;
			}
		}

		// 可能オブジェクトが無かったのでnullptrを返す
		return nullptr;
	}

	// レール設置できるか、できない場合は置かれているレールを取得
	bool SelectIndicator::GetRailedPossible(const Vec3& checkPos) const
	{
		// ステージマップ配列の取得
		const int guideRailID = static_cast<int>(eStageID::GuideRail);
		const auto& guideMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetGuideMap();

		// 一致しなかったら設置不可
		const auto& checkNum = guideMap.at(size_t(checkPos.z + 7.0f)).at(size_t(checkPos.x + 7.0f));

		return checkNum == guideRailID;
	}
}