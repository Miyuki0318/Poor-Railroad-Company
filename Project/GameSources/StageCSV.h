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
		Rock = 10,	// ��
		Tree,		// ��
		Water = -1,	// ��
	};

	class StageCSV : public Stage
	{
	protected:

		// �X�e�[�W�}�b�v
		vector<vector<int>> m_stageMap;
		vector<vector<Vec3>> m_positionMap;

		/*!
		@brief �X�e�[�W��csv�Ő���
		@param csv�̃t�@�C����
		*/
		virtual void CreateStageCSV(string csvPath = "TestStage") = 0;

		/*!
		@brief �X�e�[�W�}�b�v��csv�ŏ�������
		@param csv�̃t�@�C����
		*/
		void WriteCSVMap(string csvPath = "TestStage")
		{
			m_stageMap = CSVLoader::ReadDataToInt(CSVLoader::LoadFile(csvPath));

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
		@brief �X�e�[�W�}�b�v�̎擾
		@return m_stageMap
		*/
		vector<vector<Vec3>>& GetStagePosMap()
		{
			return m_positionMap;
		}
	};
}