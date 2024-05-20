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
	void BaseStage::CreateSoundManager()
	{
		// SEマネージャーがNULLなら
		if (!m_soundManager)
		{
			// SEマネージャーをmake_sharedで生成
			m_soundManager.reset(new SoundManager);
		}
	}

	// SEの再生
	shared_ptr<SoundItem> BaseStage::CreateSE(const wstring& seKey, float volume)
	{
		// SEマネージャーがNULLなら
		if (!m_soundManager)
		{
			// SEマネージャーの生成を行う
			CreateSoundManager();
		}

		// SEマネージャーからSEの再生を送る
		return m_soundManager->StartSE(seKey, volume, ThisPtr);
	}

	// SEの再生
	shared_ptr<SoundItem> BaseStage::CreateSE(const wstring& seKey, float volume, const void* objPtr)
	{
		// SEマネージャーがNULL
		if (!m_soundManager)
		{
			// SEマネージャーの生成を行う
			CreateSoundManager();
		}

		// SEマネージャーからSEの再生を送る
		return m_soundManager->StartSE(seKey, volume, objPtr);
	}

	// SEの停止
	void BaseStage::StopSE(const wstring& seKey)
	{
		// SEマネージャーがあるなら
		if (m_soundManager)
		{
			// SEマネージャーからSEの停止を送る
			m_soundManager->StopSE(seKey, ThisPtr);
		}
	}

	// SEの停止
	void BaseStage::StopSE(const wstring& seKey, const void* objPtr)
	{
		// SEマネージャーがあるなら
		if (m_soundManager)
		{
			// SEマネージャーからSEの停止を送る
			m_soundManager->StopSE(seKey, objPtr);
		}
	}

	// 生成時の処理
	void BaseStage::OnCreate()
	{
		try
		{
			// タイマーオブジェクトの生成
			m_timer = AddGameObject<Timer>();

			// サウンドマネージャーの生成
			CreateSoundManager();
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
			m_soundManager.reset();
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
			m_soundManager->CheckSoundList();
		}
		catch (...)
		{
		}
	}
}