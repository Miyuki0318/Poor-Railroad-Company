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
	using namespace Input;
	using namespace Utility;

	// 生成時の処理
	void SelectIndicator::OnCreate()
	{
		// 継承元の生成時の処理を実行
		TemplateObject::OnCreate();

		// 頂点データの設定
		const float HELF = 0.55f;
		vector<VertexPositionColor> vertices = {
			{{-HELF, -HELF, -HELF}, COL_WHITE}, // 0
			{{-HELF,  HELF, -HELF}, COL_ALPHA}, // 1
			{{ HELF,  HELF, -HELF}, COL_ALPHA}, // 2
			{{ HELF, -HELF, -HELF}, COL_WHITE}, // 3
			{{-HELF, -HELF,  HELF}, COL_WHITE}, // 4
			{{-HELF,  HELF,  HELF}, COL_ALPHA}, // 5
			{{ HELF,  HELF,  HELF}, COL_ALPHA}, // 6
			{{ HELF, -HELF,  HELF}, COL_WHITE}, // 7		
		};
		vector<uint16_t> indices = {
			0, 1, 2, 0, 2, 3, // Front
			4, 6, 5, 4, 7, 6, // Back
			0, 4, 5, 0, 5, 1, // Left
			3, 2, 6, 3, 6, 7, // Right
		};

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PCStaticDraw>();
		m_ptrDraw->SetOriginalMeshUse(true);
		m_ptrDraw->CreateOriginalMesh(vertices, indices);

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
		UpdateCursolPosition(position);

		// プレイヤーの座標+方向ベクトルで座標更新
		SetPosition(position);
	}

	// 選択しているポイントの更新
	void SelectIndicator::UpdateSelectPoint()
	{
		// ステージcsv配列の取得
		const auto& stagePtr = GetTypeStage<BaseStage>();
		const auto& stageMap = stagePtr->GetStageMap();

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

		// 演出中じゃなければ表示する
		SetDrawActive(!stagePtr->GetIsStaging());
	}

	// 隣接点にインディケーターを移動
	void SelectIndicator::UpdateCursolPosition(Vec3& position)
	{
		// プレイヤーの回転角Y軸の中から90の倍数に一番近いのに設定
		const auto& player = dynamic_pointer_cast<Player>(m_player.lock());

		// プレイヤーの座標を四捨五入する
		Vec3 pos = player->GetPosition();
		pos.x = round(pos.x);
		pos.z = round(pos.z);
		pos.y = m_position.y;

		// RBボタン入力が無ければ
		if (!GetButtonRB())
		{
			// コントローラー入力
			Vec3 cntlVec = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);
			if (cntlVec.length() > 0.0f)
			{
				m_cursolPosition += cntlVec * DELTA_TIME * 4.0f;
				m_cursolPosition.clamp(Vec3(-1.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 1.0f));
				if (GetBetween(cntlVec.x, m_deadZone, -m_deadZone)) m_cursolPosition.x = 0.0f;
				if (GetBetween(cntlVec.z, m_deadZone, -m_deadZone)) m_cursolPosition.z = 0.0f;
			}
		}

		Vec3 cursol = m_cursolPosition;
		cursol.x = round(cursol.x);
		cursol.z = round(cursol.z);

		// 座標を上書き
		position = pos + cursol;
	}

	// 採取オブジェクトを選択しているか
	bool SelectIndicator::IsGatheringPoint() const
	{
		// 採取オブジェクトマネージャーの取得
		const auto& gatheringManager = GetStage()->GetSharedGameObject<GatheringManager>(L"GatheringManager", false);
		if (!gatheringManager) return false;

		return gatheringManager->GetIsGatheringPoint(m_selectPoint);
	}

	// ガイドレールを選択しているか
	bool SelectIndicator::IsGuideRailPoint() const
	{
		// レールマネージャーの取得
		const auto& railManager = GetStage()->GetSharedGameObject<RailManager>(L"RailManager", false);
		if (!railManager) return false;

		return railManager->GetIsGuidePoint(m_selectPoint);
	}

	// 水場を選択しているか
	bool SelectIndicator::IsWaterPoint() const
	{
		// 木の足場マネージャーを取得
		const auto& bridgeManager = GetStage()->GetSharedGameObject<BridgeManager>(L"BridgeManager", false);
		if (!bridgeManager) return false;

		return bridgeManager->GetIsWaterPoint(m_selectPoint);
	}

	// 直線レールを選択しているか
	bool SelectIndicator::IsStraightRailPoint() const
	{
		// 選択ポイントが踏切を設置できるレールの位置と一致しているか
		const auto& crossingManager = GetStage()->GetSharedGameObject<CrossingManager>(L"CrossingManager", false);
		if (!crossingManager) return false;

		return crossingManager->GetIsRailPoint(m_selectPoint);
	}

	// 採取命令
	int SelectIndicator::GatheringOrder() const
	{
		// 採取オブジェクトマネージャーの取得
		const auto& gatheringManager = GetStage()->GetSharedGameObject<GatheringManager>(L"GatheringManager", false);
		if (!gatheringManager) return UnSTAGE_ID(eStageID::None);

		// 選択ポイントがガイドの位置と一致しているか
		int id = UnSTAGE_ID(eStageID::None);
		if (gatheringManager->GetIsGatheringPoint(m_selectPoint))
		{
			// 一致してたらマネージャーにレール追加処理を送る
			id = gatheringManager->Gathering(m_selectPoint);
		}

		return id;
	}

	// レール設置命令
	bool SelectIndicator::RailedOrder() const
	{
		// レールマネージャーの取得
		const auto& railManager = GetStage()->GetSharedGameObject<RailManager>(L"RailManager", false);
		if (!railManager) return false;

		// 選択ポイントがガイドの位置と一致しているか
		bool posshible = railManager->GetIsGuidePoint(m_selectPoint);
		if (posshible)
		{
			// 一致してたらマネージャーにレール追加処理を送る
			railManager->AddRail(m_selectPoint);
		}

		return posshible;
	}

	// レール設置命令
	bool SelectIndicator::BridgeOrder() const
	{
		// 選択ポイントがガイドの位置と一致しているか
		const auto& bridgeManager = GetStage()->GetSharedGameObject<BridgeManager>(L"BridgeManager", false);
		if (!bridgeManager) return false;

		bool posshible = bridgeManager->GetIsWaterPoint(m_selectPoint);

		// 一致してたらマネージャーにレール追加処理を送る
		if (posshible)
		{
			bridgeManager->AddBridge(m_selectPoint);
		}

		return posshible;
	}

	// 踏切設置命令
	bool SelectIndicator::CrossingOrder() const
	{
		// 選択ポイントがガイドの位置と一致しているか
		const auto& crossingManager = GetStage()->GetSharedGameObject<CrossingManager>(L"CrossingManager", false);
		if (!crossingManager) return false;

		bool posshible = crossingManager->GetIsRailPoint(m_selectPoint);

		// 一致してたらマネージャーに踏切追加処理を送る
		if (posshible)
		{
			crossingManager->AddCrossing(m_selectPoint);
		}

		return posshible;
	}
}