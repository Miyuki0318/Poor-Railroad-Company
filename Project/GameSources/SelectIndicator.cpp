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
		// プレイヤーのポインタがあれば、座標の更新を行う
		if (m_player.lock()) UpdatePosition();

		// 選択しているポイント(csv上での)を更新
		UpdateSelectPoint();
	}

	// 座標の更新処理
	void SelectIndicator::UpdatePosition()
	{	
		Vec3 position;

		// 隣接する場所に配置
		PlayerAdjoinPosition(position);

		// プレイヤーの座標+方向ベクトルで座標更新
		SetPosition(position);
	}

	// 選択しているポイントの更新
	void SelectIndicator::UpdateSelectPoint()
	{
		// ステージcsv配列の取得
		const auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();

		// サイズと列と行
		Vec3 pos = GetPosition();
		size_t row, col;
		row = ROW(pos.z);
		col = COL(pos.x);

		// 配列の範囲内かのエラーチェック
		if (!WithInElemRange(row, col, stageMap))
		{
			return;
		}

		// 更新
		m_selectPoint = Point2D<size_t>(row, col);
	}

	// 隣接点にインディケーターを移動
	void SelectIndicator::PlayerAdjoinPosition(Vec3& position)
	{
		// プレイヤーの回転角Y軸の中から90の倍数に一番近いのに設定
		const auto& player = dynamic_pointer_cast<GamePlayer>(m_player.lock());
		float rotY = DegToRad(GetClosest(RadToDeg(player->GetPastRotTarget()), m_rotArray));

		// 方向ベクトルを定義(小数点以下四捨五入)
		Vec3 velo = Vec3(cosf(rotY), 0.0f, -sinf(rotY));
		velo.x = round(velo.x);
		velo.z = round(velo.z);

		// プレイヤーの座標を四捨五入する
		Vec3 pos = player->GetPosition();
		pos.x = round(pos.x);
		pos.z = round(pos.z);
		pos.y = m_position.y;

		// 座標を上書き
		position = pos + velo;
	}

	// 採掘命令
	set<wstring> SelectIndicator::MiningOrder() const
	{
		// 採掘可能オブジェクト配列の取得
		if (const auto& group = GetStage()->GetSharedObjectGroup(L"MiningObject", false))
		{
			const auto& miningVec = group->GetGroupVector();

			// 配列の数ループ
			for (const auto& weakObj : miningVec)
			{
				// エラーチェック
				if (!weakObj.lock()) continue;
				if (!weakObj.lock()->IsUpdateActive()) continue;

				// 型キャスト
				const auto& miningObj = dynamic_pointer_cast<MiningObject>(weakObj.lock());
				if (!miningObj) continue;

				// CSV上の座標が一致したら採掘処理を送り、タグセットを返す
				if (m_selectPoint == miningObj->GetCSVPos())
				{
					miningObj->OnMining();
					return miningObj->GetTagSet();
				}
			}
		}

		// 対象のオブジェクトが無かったのでnullのタグセットを返す
		set<wstring> null;
		return null;
	}

	// レール設置命令
	bool SelectIndicator::RailedOrder() const
	{
		// 選択ポイントがガイドの位置と一致しているか
		const auto& railManager = GetStage()->GetSharedGameObject<RailManager>(L"RailManager");
		bool posshible = railManager->GetIsGuidePoint(m_selectPoint);

		// 一致してたらマネージャーにレール追加処理を送る
		if (posshible)
		{
			railManager->AddRail(m_selectPoint);
		}

		return posshible;
	}
}