#include "stdafx.h"
#include "Project.h"

#define RAIL_ID static_cast<size_t>(basecross::eStageID::Rail)
#define DERAIL_ID static_cast<size_t>(basecross::eStageID::DeRail)
#define GUIDE_ID static_cast<size_t>(basecross::eStageID::GuideRail)

const float stageDiff = 0.0f;
const Vec3 scale = Vec3(1.0f, 0.2f, 1.0f);

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

		// ローテーション用のクォータニオン
		Quat quat;
		quat.rotationRollPitchYawFromVector(Vec3(0.0f));

		// 二重ループ
		for (int i = 0; i < stageMap.size(); i++)
		{
			for (int j = 0; j < stageMap.at(i).size(); j++)
			{
				// レールIDと先端レールID以外は無視
				if (!Utility::GetBetween(stageMap.at(i).at(j), RAIL_ID, DERAIL_ID)) continue;

				// 行列の宣言
				Mat4x4 matrix, mtxT, mtxR, mtxS;

				// クォータニオンからローテーションを設定
				mtxR.rotation(quat);
				mtxS.scale(scale); // スケールの設定
				mtxT.translation(Vec3(float(j), 1.0f, float(i))); // 座標の設定

				// 行列の設定と追加
				matrix = mtxS * mtxR * mtxT;
				m_ptrDraw->AddMatrix(matrix);

				// 先端レールならガイドIDを設定
				if (stageMap.at(i).at(j) == DERAIL_ID)
				{
					SetGuideID(i, j);
				}
			}
		}
	}

	// レールの追加
	void RailManager::AddRail(const Vec3& addPos)
	{
		// ステージcsvの取得
		auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();

		// 列と行を座標から求める
		size_t i, j;
		j = size_t(addPos.x);
		i = stageMap.size() - size_t(addPos.z) - 1;

		// ローテーション用
		Quat quat;
		quat.rotationRollPitchYawFromVector(Vec3(0.0f));

		// 行列の宣言
		Mat4x4 matrix, mtxT, mtxR, mtxS;

		// クォータニオンからローテーションを設定
		mtxR.rotation(quat);
		mtxS.scale(scale); // スケールの設定
		mtxT.translation(Vec3(addPos.x, 1.0f, addPos.z)); // 座標の設定

		// 行列の設定と追加
		matrix = mtxS * mtxR * mtxT;
		m_ptrDraw->AddMatrix(matrix);

		// csvの書き換え
		stageMap.at(i).at(j) = DERAIL_ID;
		m_guideMap = stageMap;
		SetGuideID(i, j);
		SetRailID(i, j);
	}

	// 先端レールの書き換え
	void RailManager::SetRailID(size_t i, size_t j) const
	{
		// csvの取得
		auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();
		
		// 要素数分ループ
		for (const auto& elem : GetMapElems(i, j, stageMap))
		{
			// 要素数が範囲内なら
			if (elem.isRange)
			{
				// 先端レールなら通常のレールに、それ以外ならそのまま
				int& num = stageMap.at(elem.row).at(elem.col);
				num = num == DERAIL_ID ? RAIL_ID : num;
			}
		}
	}

	// ガイドの追加
	void RailManager::SetGuideID(size_t i, size_t j)
	{
		// 要素数分ループ
		for (const auto& elem : GetMapElems(i, j, m_guideMap))
		{
			// 要素数が範囲内なら
			if (elem.isRange)
			{
				// 何も無いならガイドにする、それ以外ならそのまま
				int& num = m_guideMap.at(elem.row).at(elem.col);
				num = num == 0 ? GUIDE_ID : num;
			}
		}
	}
}