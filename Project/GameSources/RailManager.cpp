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

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void RailManager::OnCreate()
	{
		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_GRAY);

		// csvの取得とスケール
		const auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();
		m_guideMap = stageMap;

		// 二重ループ
		for (size_t row = 0; row < stageMap.size(); row++)
		{
			for (size_t col = 0; col < stageMap.at(row).size(); col++)
			{
				// レールIDと先端レールID以外は無視
				if (!Utility::GetBetween(stageMap.at(row).at(col), RAIL_ID, DERAIL_ID)) continue;

				// インスタンス描画を追加
				AddInstanceRail(row, col);

				// 先端レールならガイドIDを設定
				if (stageMap.at(row).at(col) == DERAIL_ID)
				{
					SetGuideID(row, col);
				}
			}
		}
	}

	// レールの追加
	void RailManager::AddRail(const Point2D<size_t>& point)
	{
		// ステージcsvの取得
		auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();

		// インスタンス描画を追加
		AddInstanceRail(point.x, point.y);

		// csvの書き換え
		stageMap.at(point.x).at(point.y) = DERAIL_ID;
		m_guideMap = stageMap;
		SetGuideID(point.x, point.y);
		SetRailID(point.x, point.y);
	}

	// 先端レールの書き換え
	void RailManager::SetRailID(size_t row, size_t col) const
	{
		// csvの取得
		auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();
		
		// 要素数が範囲内で、先端レールなら通常のレールにする
		for (const auto& elem : GetElemsCheck(row, col, stageMap))
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

		// 要素数が範囲内で、何も無いならガイドにする
		for (auto& elem : GetElemsCheck(row, col, m_guideMap))
		{
			if (elem.isRange)
			{
				int& num = m_guideMap.at(elem.row).at(elem.col);
				if (num == 0)
				{
					num = GUIDE_ID; // ガイドに書き換え

					// ガイドの列と行の番号を保持
					m_guidePoints.push_back(Point2D<size_t>(elem.row, elem.col));
				}
			}
		}
	}

	// 前後左右の配列チェッカー取得
	vector<CSVElementCheck> RailManager::GetElemsCheck(size_t row, size_t col, const vector<vector<int>>& csvMap) const
	{
		vector<CSVElementCheck> elems; // 前後左右の結果保存用配列
		if (csvMap.empty()) return elems; // 参照する配列が空なら空を返す

		elems = {
			{row - 1, col, WithInElemRange(row - 1, csvMap.size())},		// 前
			{row + 1, col, WithInElemRange(row + 1, csvMap.size())},		// 後
			{row, col - 1, WithInElemRange(col - 1, csvMap.at(row).size())},// 左
			{row, col + 1, WithInElemRange(col + 1, csvMap.at(row).size())},// 右
		};

		return elems;
	}

	// 指定の座標にガイドがあるか
	bool RailManager::GetIsGuidePoint(const Point2D<size_t>& point) const
	{
		// 配列の範囲内かのエラーチェック
		if (!WithInElemRange(point.y, point.y, m_guideMap))
		{
			return false;
		}

		// ガイドIDかどうか
		int num = m_guideMap.at(point.y).at(point.y);
		return num == GUIDE_ID;
	}
}