/*!
@file BaseStage.cpp
@brief 継承元のステージオブジェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// CSVの読み込み用
	void BaseStage::WriteCSVMap(string csvPath)
	{
		m_stageMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Stage"));
		m_groundMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Ground"));

		// 二重ループ
		for (int i = 0; i < m_stageMap.size(); i++)
		{
			vector<Vec3> tempVec;
			for (int j = 0; j < m_stageMap.at(i).size(); j++)
			{
				tempVec.push_back(Vec3(float(j), 0.0f, -float(i)));
			}
			m_positionMap.push_back(tempVec);
		}
	}

	// SEマネージャーの生成
	void BaseStage::CreateSEManager()
	{
		// SEマネージャーがNULLなら
		if (!m_seManager)
		{
			// SEマネージャーをmake_sharedで生成
			m_seManager.reset(new SEManager);
		}
	}

	// SEの再生
	void BaseStage::CreateSE(const wstring& seKey, float volume)
	{
		// SEマネージャーがNULLなら
		if (!m_seManager)
		{
			// SEマネージャーの生成を行う
			CreateSEManager();
		}

		// SEマネージャーからSEの再生を送る
		m_seManager->StartSE(seKey, volume, ThisPtr);
	}

	// SEの再生
	void BaseStage::CreateSE(const wstring& seKey, float volume, const void* objPtr)
	{
		// SEマネージャーがNULL
		if (!m_seManager)
		{
			// SEマネージャーの生成を行う
			CreateSEManager();
		}

		// SEマネージャーからSEの再生を送る
		m_seManager->StartSE(seKey, volume, objPtr);
	}

	// SEの停止
	void BaseStage::StopSE(const wstring& seKey)
	{
		// SEマネージャーがあるなら
		if (m_seManager)
		{
			// SEマネージャーからSEの停止を送る
			m_seManager->StopSE(seKey, ThisPtr);
		}
	}

	// SEの停止
	void BaseStage::StopSE(const wstring& seKey, const void* objPtr)
	{
		// SEマネージャーがあるなら
		if (m_seManager)
		{
			// SEマネージャーからSEの停止を送る
			m_seManager->StopSE(seKey, objPtr);
		}
	}

	// 生成時の処理
	void BaseStage::OnCreate()
	{
		try
		{
			// タイマーオブジェクトの生成
			m_timer = AddGameObject<Timer>();
		}
		catch (...)
		{
		}
	}

	// 破棄時の処理
	void BaseStage::OnDestroy()
	{
		try
		{
			// 解放
			m_seManager.reset();
		}
		catch (...)
		{
		}
	}

	// 破棄時の処理
	void BaseStage::OnUpdate()
	{
		try
		{
			// SEリストの確認
			m_seManager->CheckSEList();
		}
		catch (...)
		{
		}
	}
}