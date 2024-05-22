/*!
@file RailManager.cpp
@brief レール管理クラス
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

// レールに関するID
#define RAIL_ID static_cast<size_t>(basecross::eStageID::Rail)
#define DERAIL_ID static_cast<size_t>(basecross::eStageID::DeRail)
#define GUIDE_ID static_cast<size_t>(basecross::eStageID::GuideRail)
#define GOALRAIL_ID static_cast<size_t>(basecross::eStageID::GoalRail)

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// lineからrowとcolを抽出
	void GetLineStringToRowCol(size_t& row, size_t& col, string line)
	{
		// 文字列を '-' を境に分割
		istringstream iss(line);
		string token;

		// tokenを数値に変換してrowに格納
		getline(iss, token, '-');
		row = stoul(token);

		// tokenを数値に変換してcolに格納
		getline(iss, token);
		col = stoul(token);
	}

	// 生成時の処理
	void RailManager::OnCreate()
	{
		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"RAIL");
		m_ptrDraw->SetTextureResource(L"RAIL_TX");
		m_ptrDraw->SetSpecular(COL_WHITE);
		m_ptrDraw->SetDiffuse(COL_WHITE);

		// csvの取得とスケール
		const auto& stagePtr = GetTypeStage<BaseStage>();
		const auto& stageMap = stagePtr->GetStageMap();
		m_guideMap = stageMap;

		// 二重ループ
		for (size_t row = 0; row < stageMap.size(); row++)
		{
			for (size_t col = 0; col < stageMap.at(row).size(); col++)
			{
				// レールIDと先端レールID以外は無視
				int id = stageMap.at(row).at(col);
				if (!Utility::GetBetween(id, RAIL_ID, GOALRAIL_ID)) continue;

				// インスタンス描画を追加
				AddInstanceRail(row, col);

				// 先端レールならガイドIDを設定
				if (STAGE_ID(id) == eStageID::DeRail)
				{
					SetGuideID(row, col);
				}

				// ゴールレールなら
				if (STAGE_ID(id) == eStageID::GoalRail)
				{
					stagePtr->AddGameObject<Station>(LINE2POS(row - 1, col));
				}
			}
		}
	}

	// レールの追加
	void RailManager::AddRail(const Point2D<size_t>& point)
	{
		// ステージcsvの取得
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();

		// インスタンス描画を追加
		AddInstanceRail(point.x, point.y);

		// csvの書き換え
		stageMap.at(point.x).at(point.y) = DERAIL_ID;
		m_guideMap = stageMap;
		SetGuideID(point.x, point.y);
		SetRailID(point.x, point.y);

		// カウンタを増やす
		m_railNum++;
	}

	// 先端レールの書き換え
	void RailManager::SetRailID(size_t row, size_t col) const
	{
		// csvの取得
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		
		// 要素数が範囲内で、先端レールなら通常のレールにする
		for (const auto& elem : CSVElementCheck::GetElemsCheck(row, col, stageMap))
		{
			if (elem.isRange)
			{
				int& num = stageMap.at(elem.row).at(elem.col);
				num = num == DERAIL_ID ? RAIL_ID : num;
			}
		}
	}

	// ガイドの追加
	void RailManager::SetGuideID(size_t row, size_t col)
	{
		m_guidePoints.clear(); // 初期化
		m_pastDeRailPos = LINE2POS(row, col);

		// 要素数が範囲内で、何も無いならガイドにする
		auto& elems = CSVElementCheck::GetElemsCheck(row, col, m_guideMap);
		for (auto& elem : elems)
		{
			if (elem.isRange)
			{
				int& num = m_guideMap.at(elem.row).at(elem.col);
				if (num == 0 || num == GUIDE_ID)
				{
					num = GUIDE_ID; // ガイドに書き換え

					// ガイドの列と行の番号を保持
					m_guidePoints.push_back(Point2D<size_t>(elem.row, elem.col));
				}
			}
		}
	}

	// 指定の座標にガイドがあるか
	bool RailManager::GetIsGuidePoint(const Point2D<size_t>& point) const
	{
		// 配列の範囲内かのエラーチェック
		if (!WithInElemRange(point.x, point.y, m_guideMap))
		{
			return false;
		}

		// ガイドIDかどうか
		int num = m_guideMap.at(point.x).at(point.y);
		return num == GUIDE_ID;
	}

	// ガイドの再計算処理
	void RailManager::GuideRecalculation()
	{
		// ステージcsvの取得
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		m_guideMap = stageMap;

		// サイズと列と行
		Vec3 pos = m_pastDeRailPos;
		size_t row, col;
		row = ROW(pos.z);
		col = COL(pos.x);

		// ガイドの再設定
		SetGuideID(row, col);
	}
}