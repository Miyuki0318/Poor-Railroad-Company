/*!
@file BaseStage.h
@brief �p�����̃X�e�[�W�I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "stdafx.h"

// �X�e�[�WID�L���X�g�}�N��
#define STAGE_ID(id) static_cast<eStageID>(id)

namespace basecross
{
	// �X�e�[�W�I�u�W�F�N�gID
	enum class eStageID
	{
		None,			// �����Ȃ�
		Rail,			// ���[��
		DeRail,			// �E������
		GuideRail,		// �K�C�h���[��
		GoalRail,		// �S�[�����[��
		Stone = 10,		// ��
		Tree,			// ��
		Grass = 101,	// �n�ʂ̑�
		Sand = 102,		// �n�ʂ̍�
		Rock = 103,		// �n�ʂ̐�
		Air = 111,		// ��C(�Ȃ���Ȃ�)
		Water = 112,	// ����
	};

	/*!
	@brief �X�e�[�W�̌p����
	*/
	class BaseStage : public Stage
	{
	protected:

		// �X�e�[�W�}�b�v
		vector<vector<int>> m_stageMap;
		vector<vector<int>> m_groundMap;
		vector<vector<Vec3>> m_positionMap;

		// SE�}�l�[�W���[
		unique_ptr<SEManager> m_seManager;

		// �^�C�}�[�I�u�W�F�N�g
		weak_ptr<Timer> m_timer;

		/*!
		@brief �X�e�[�W��csv�Ő���
		@param csv�̃t�@�C����
		*/
		virtual void CreateStageCSV(string csvPath = "Test") = 0;

		/*!
		@brief �X�e�[�W�}�b�v��csv�ŏ�������
		@param csv�̃t�@�C����
		*/
		void WriteCSVMap(string csvPath = "Test");

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		BaseStage() : Stage() {}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~BaseStage() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief �j������鎞�Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnDestroy() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief SE�}�l�[�W���[�̐����֐�
		*/
		virtual void CreateSEManager();

		/*!
		@brief SE�̍Đ��֐�
		@param SE�L�[
		@param ����
		*/
		virtual shared_ptr<SoundItem> CreateSE(const wstring& seKey, float volume);

		/*!
		@brief SE�̍Đ��֐�
		@param SE�L�[
		@param ����
		@param �I�u�W�F�N�g�̃|�C���^
		*/
		virtual shared_ptr<SoundItem> CreateSE(const wstring& seKey, float volume, const void* objPtr);

		/*!
		@brief SE�̒�~�֐�
		@param SE�L�[
		*/
		virtual void StopSE(const wstring& seKey);

		/*!
		@brief SE�̒�~�֐�
		@param SE�L�[
		@param �I�u�W�F�N�g�̃|�C���^
		*/
		virtual void StopSE(const wstring& seKey, const void* objPtr);

		/*!
		@brief �^�C�}�[�N���X�擾�֐�
		@return const shared_ptr<Timer>
		*/
		const shared_ptr<Timer> GetTimer() const
		{
			return m_timer.lock();
		}

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