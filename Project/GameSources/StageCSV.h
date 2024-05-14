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
	};
}