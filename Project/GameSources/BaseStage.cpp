/*!
@file BaseStage.cpp
@brief �p�����̃X�e�[�W�I�u�W�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// CSV�̓ǂݍ��ݗp
	void BaseStage::WriteCSVMap(string csvPath)
	{
		m_stageMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Stage"));
		m_groundMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath + "Ground"));

		// ��d���[�v
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

	// SE�}�l�[�W���[�̐���
	void BaseStage::CreateSoundManager()
	{
		// SE�}�l�[�W���[��NULL�Ȃ�
		if (!m_soundManager)
		{
			// SE�}�l�[�W���[��make_shared�Ő���
			m_soundManager.reset(new SoundManager);
		}
	}

	// SE�̍Đ�
	shared_ptr<SoundItem> BaseStage::CreateSE(const wstring& seKey, float volume)
	{
		// SE�}�l�[�W���[��NULL�Ȃ�
		if (!m_soundManager)
		{
			// SE�}�l�[�W���[�̐������s��
			CreateSoundManager();
		}

		// SE�}�l�[�W���[����SE�̍Đ��𑗂�
		return m_soundManager->StartSE(seKey, volume, ThisPtr);
	}

	// SE�̍Đ�
	shared_ptr<SoundItem> BaseStage::CreateSE(const wstring& seKey, float volume, const void* objPtr)
	{
		// SE�}�l�[�W���[��NULL
		if (!m_soundManager)
		{
			// SE�}�l�[�W���[�̐������s��
			CreateSoundManager();
		}

		// SE�}�l�[�W���[����SE�̍Đ��𑗂�
		return m_soundManager->StartSE(seKey, volume, objPtr);
	}

	// SE�̒�~
	void BaseStage::StopSE(const wstring& seKey)
	{
		// SE�}�l�[�W���[������Ȃ�
		if (m_soundManager)
		{
			// SE�}�l�[�W���[����SE�̒�~�𑗂�
			m_soundManager->StopSE(seKey, ThisPtr);
		}
	}

	// SE�̒�~
	void BaseStage::StopSE(const wstring& seKey, const void* objPtr)
	{
		// SE�}�l�[�W���[������Ȃ�
		if (m_soundManager)
		{
			// SE�}�l�[�W���[����SE�̒�~�𑗂�
			m_soundManager->StopSE(seKey, objPtr);
		}
	}

	// �������̏���
	void BaseStage::OnCreate()
	{
		try
		{
			// �^�C�}�[�I�u�W�F�N�g�̐���
			m_timer = AddGameObject<Timer>();

			// �T�E���h�}�l�[�W���[�̐���
			CreateSoundManager();
		}
		catch (...)
		{
		}
	}

	// �j�����̏���
	void BaseStage::OnDestroy()
	{
		try
		{
			// ���
			m_soundManager.reset();
		}
		catch (...)
		{
		}
	}

	// �j�����̏���
	void BaseStage::OnUpdate()
	{
		try
		{
			// SE���X�g�̊m�F
			m_soundManager->CheckSoundList();
		}
		catch (...)
		{
		}
	}
}