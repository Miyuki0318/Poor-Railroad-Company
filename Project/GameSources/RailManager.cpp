#include "stdafx.h"
#include "Project.h"

#define STAGE_DIFF 7.0f

namespace basecross
{
	void RailManager::OnCreate()
	{
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_GRAY);

		const auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();
		const int railID = static_cast<int>(eStageID::Rail);
		const int deRailID = static_cast<int>(eStageID::DeRail);
		const Vec3 scale = Vec3(1.0f, 0.2f, 1.0f);

		float x, z;

		Quat quat;
		quat.rotationRollPitchYawFromVector(Vec3(0.0f, 0.0f, 0.0f));

		for (int i = 0; i < stageMap.size(); i++)
		{
			for (int j = 0; j < stageMap.at(i).size(); j++)
			{
				if (!Utility::GetBetween(stageMap.at(i).at(j), railID, deRailID)) continue;
				
				x = j - STAGE_DIFF;
				z = -i + STAGE_DIFF;

				// 行列の宣言
				Mat4x4 matrix, mtxT, mtxR, mtxS; 

				// クォータニオンからローテーションを設定
				mtxR.rotation(quat);
				mtxS.scale(scale); // スケールの設定
				mtxT.translation(Vec3(x, 1.0f, z)); // 座標の設定

				// 行列の設定と追加
				matrix = mtxS * mtxR * mtxT;
				m_ptrDraw->AddMatrix(matrix);
			}
		}
	}

	void RailManager::AddRail(const Vec3& addPos)
	{
		auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();
		const int railID = static_cast<int>(eStageID::Rail);
		const int deRailID = static_cast<int>(eStageID::DeRail);
		const Vec3 scale = Vec3(1.0f, 0.2f, 1.0f);

		int i, j;
		j = int(addPos.x + STAGE_DIFF);
		i = int(addPos.z + STAGE_DIFF);

		Quat quat;
		quat.rotationRollPitchYawFromVector(Vec3(0.0f, 0.0f, 0.0f));

		// 行列の宣言
		Mat4x4 matrix, mtxT, mtxR, mtxS;

		// クォータニオンからローテーションを設定
		mtxR.rotation(quat);
		mtxS.scale(scale); // スケールの設定
		mtxT.translation(Vec3(addPos.x, 1.0f, addPos.z)); // 座標の設定

		// 行列の設定と追加
		matrix = mtxS * mtxR * mtxT;
		m_ptrDraw->AddMatrix(matrix);

		stageMap.at(i).at(j) = deRailID;

		if (stageMap.at(i).at(j - 1) == deRailID) stageMap.at(i).at(j - 1) = railID;
		if (stageMap.at(i).at(j + 1) == deRailID) stageMap.at(i).at(j + 1) = railID;
		if (stageMap.at(i - 1).at(j) == deRailID) stageMap.at(i - 1).at(j) = railID;
		if (stageMap.at(i + 1).at(j) == deRailID) stageMap.at(i + 1).at(j) = railID;
	}
}