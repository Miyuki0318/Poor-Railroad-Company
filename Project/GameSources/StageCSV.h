#pragma once
#include "stdafx.h"

#define STAGE_ID(id) static_cast<eStageID>(id)

namespace basecross
{
	// �X�e�[�W�I�u�W�F�N�gID
	enum class eStageID
	{
		None,		// �����Ȃ�
		Rail,		// ���[��
		DeRail,		// �E������
		GuideRail,	// �K�C�h���[��
		GoalRail,	// �S�[�����[��
		Stone = 10,	// ��
		Tree,		// ��

		Grass = 101,	// �n�ʂ̑�
		Rock = 102,		// �n�ʂ̐�
		Air = 111,		// ��C(�Ȃ���Ȃ�)
		Water = 112,	// ����
	};

	class StageCSV : public Stage
	{
	protected:

		// �X�e�[�W�}�b�v
		vector<vector<int>> m_stageMap;
		vector<vector<int>> m_groundMap;
		vector<vector<Vec3>> m_positionMap;

		/*!
		@brief �X�e�[�W��csv�Ő���
		@param csv�̃t�@�C����
		*/
		virtual void CreateStageCSV(string csvPath = "Test") = 0;

		/*!
		@brief �X�e�[�W�}�b�v��csv�ŏ�������
		@param csv�̃t�@�C����
		*/
		void WriteCSVMap(string csvPath = "Test")
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

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		StageCSV() : Stage() {}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~StageCSV() {}

		/*!
		@brief �X�e�[�W�}�b�v�̎擾
		@return m_stageMap
		*/
		vector<vector<int>>& GetStageMap()
		{
			return m_stageMap;
		}

		/*!
		@brief �O���E���h�}�b�v�̎擾
		@return m_groundMap
		*/
		vector<vector<int>>& GetGroundMap()
		{
			return m_groundMap;
		}

		/*!
		@brief �X�e�[�W�}�b�v�̎擾
		@return m_stageMap
		*/
		vector<vector<Vec3>>& GetStagePosMap()
		{
			return m_positionMap;
		}
	};
}