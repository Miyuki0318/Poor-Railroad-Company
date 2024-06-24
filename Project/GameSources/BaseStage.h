/*!
@file BaseStage.h
@brief �p�����̃X�e�[�W�I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "stdafx.h"
#include "ResourcesManager.h"

// �X�e�[�WID�L���X�g�}�N��
#define STAGE_ID(id) static_cast<eStageID>(id)
#define UnSTAGE_ID(id) static_cast<int>(id)

namespace basecross
{
	// �X�e�[�W�I�u�W�F�N�gID
	enum class eStageID : int
	{
		None,				// �����Ȃ�
		Rail,				// ���[��
		DeRail,				// �E������
		GuideRail,			// �K�C�h���[��
		GoalRail,			// �S�[�����[��
		StartRail,			// �J�n���[��
		DrawRail,			// �`��݂̂̃��[��
		Stone1 = 10,		// ��0
		Stone2,				// ��1
		Stone3,				// ��2
		Tree1,				// ��0
		Tree2,				// ��1
		UnBreakRock = 20,	// �󂹂Ȃ���
		CrossingOpen = 22,	// ����(�ʂ��)
		CrossingCross,		// ����(�ʂ�Ȃ�)
		Gear = 25,			// �M�A
		Water = 31,			// ��
		WoodBridge = 40,	// �؂̑���	
		PlayerStart = 50,	// �v���C���[�̊J�n�ʒu
		PlayerGoal,			// �v���C���[�̃S�[�����o�ʒu
		Grass = 101,		// �n�ʂ̑�
		Sand = 102,			// �n�ʂ̍�
		Rock = 103,			// �n�ʂ̐�
		Air = 111,			// ��C(�Ȃ���Ȃ�)
		Watering = 112,		// ����
		UnGrass = 121,		// �`��̂ݒʉߕs��
	};

	/*!
	@brief �X�e�[�W�̌p����
	*/
	class BaseStage : public Stage, public ResourcesManager
	{
	protected:

		// �X�e�[�W�}�b�v
		vector<vector<int>> m_stageMap;
		vector<vector<int>> m_groundMap;
		vector<vector<Vec3>> m_positionMap;

		// SE�}�l�[�W���[
		unique_ptr<SoundManager> m_soundManager;

		// �^�C�}�[�I�u�W�F�N�g
		weak_ptr<Timer> m_timer;

		// �t�F�[�h�p�X�v���C�g
		shared_ptr<Sprite> m_fadeSprite;

		// �X�e�[�W�̃t�@�C����
		string m_stagePath;

		float m_stageDistanceX;

		/*!
		@brief ���\�[�X�̓Ǎ�
		*/
		virtual void CreateResourses();

		/*!
		@brief ���\�[�X�̉��
		*/
		virtual void ReleasedResourses();

		/*
		@brief �t�F�[�h�p�X�v���C�g�̐���
		*/
		virtual void CreateFadeSprite();

		/*!
		@brief �X�e�[�W��csv�Ő���
		@param csv�̃t�@�C����
		*/
		virtual void CreateStageCSV(string csvPath) = 0;

		/*!
		@brief �X�e�[�W�}�b�v��csv�ŏ�������
		@param csv�̃t�@�C����
		*/
		void WriteCSVMap(string csvPath);

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		BaseStage(const string stagePath) : Stage(), m_stagePath(stagePath) 
		{
			m_stageDistanceX = 0.0f;
		}

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
		@brief �`��X�V�֐�
		*/
		virtual void OnDraw() override;

		/*!
		@brief SE�}�l�[�W���[�̐����֐�
		*/
		virtual void CreateSoundManager();

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

		/*!
		@brief �����̎擾
		@return money
		*/
		int GetMoney() const
		{
			// Scene���擾
			const weak_ptr<Scene> scene = App::GetApp()->GetScene<Scene>();
			// Scene����money���擾
			int money = scene.lock()->GetMoney();
			// money�̒l��Ԃ�
			return money;
		}

		/*!
		@brief �����̉��Z
		@param[in] ���Z����l
		*/
		void AddMoney(int value) const
		{
			// Scene���擾
			const weak_ptr<Scene> scene = App::GetApp()->GetScene<Scene>();
			// Scene����money���擾
			int money = scene.lock()->GetMoney();
			//money�ɒl�����Z
			money += value;
			// Scene��money���X�V
			scene.lock()->SetMoney(money);
		}

		/*!
		@brief �����̎g�p
		@param[in] �g�p����l
		*/
		void UseMoney(int value) const
		{
			// Scene���擾
			const weak_ptr<Scene> scene = App::GetApp()->GetScene<Scene>();
			// Scene����money���擾
			int money = scene.lock()->GetMoney();
			//money����l�����Z
			money -= value;
			// Scene��money���X�V
			scene.lock()->SetMoney(money);
		}
	};
}