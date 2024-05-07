/*!
@file GamePlayer.h
@brief �Q�[�����̃v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "Player.h"
#include "SelectIndicator.h"
#include "CraftManager.h"
#include "PlayerState.h"

namespace basecross
{
	// �v���C���[�̏��
	enum class ePlayerStatus : uint16_t
	{
		IsIdle,		// �ҋ@���
		IsMove,		// �ړ����
		IsRotate,	// ��]���
		IsMining,	// �̌@���
		IsHaveWood,	// �؂�������
		IsHaveStone,// �΂�������
		IsCrafting,	// �N���t�g��
		IsCraftQTE,	// �N���t�gQTE��
		IsHaveRail,	// ���H������
	};

	// �v���C���[�̏�ԃX�e�[�g�N���X(���O�̂ݐ錾)
	class PlayerIdleState;		// �ҋ@���
	class PlayerMovingState;	// �ړ����
	class PlayerMiningState;	// �̌@���
	class PlayerCraftingState;	// �N���t�g���

	/*!
	@brief �Q�[�����̃v���C���[
	*/
	class GamePlayer : public Player
	{
		weak_ptr<SelectIndicator> m_indicator; // �Z���N�g�C���f�B�P�[�^�[
		unique_ptr<CraftManager> m_craft; // �N���t�g�}�l�[�W���[

		map<wstring, eItemType> m_miningMap; // �̌@�ΏۂƎ擾�A�C�e���^�C�v
		Bool16_t<ePlayerStatus> m_status; // �t���O�Ǘ��N���X

		// �X�e�[�g�}�V��
		unique_ptr<PlayerStateMachine> m_playerState;

		// �t�����h��(�X�e�[�g�}�V�����烁���o�֐����Ăяo������)
		friend PlayerIdleState;
		friend PlayerMovingState;
		friend PlayerMiningState;
		friend PlayerCraftingState;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		GamePlayer(const shared_ptr<Stage>& stagePtr) :
			Player(stagePtr)
		{
			m_status = 0; // ��ԃt���O��0�ŏ�����

			// �̌@�I�u�W�F�N�g�̃^�O�ƍ̌@���ɉ��Z����A�C�e���̃^�C�v
			m_miningMap.insert(make_pair(L"Tree", eItemType::Wood));	// �^�O���؂Ȃ�A�C�e���^�C�v�͖؍�
			m_miningMap.insert(make_pair(L"Rock", eItemType::Stone)); // �^�O����Ȃ�A�C�e���^�C�v�͐΍�

			// �ړ��s��ID�̓o�^
			m_impassableSet.insert(eStageID::Rail);
			m_impassableSet.insert(eStageID::DeRail);
			m_impassableSet.insert(eStageID::GoalRail);
			m_impassableSet.insert(eStageID::Water);
			m_impassableSet.insert(eStageID::Tree);
			m_impassableSet.insert(eStageID::Rock);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GamePlayer()
		{
			// �@�\�̉��
			m_craft.reset();
			m_playerState.reset();
		}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

	private:

		/*!
		@brief �R���|�[�l���g�̐����֐�
		*/
		void CreateComponent() override;

		/*!
		@brief �v���C���[�ɕt������@�\�����֐�
		*/
		void CreatePlayerFeatures();

		/*!
		@brief �C���f�B�P�[�^�[�ւ̎擾�ƌĂяo���֐�
		*/
		void IndicatorOrder();

		/*!
		@brief �̌@���ɌĂяo�����֐�
		@param �̌@�����I�u�W�F�N�g�̃|�C���^
		*/
		void MiningProcces(const set<wstring>& tagSet);

		/*!
		@brief �N���t�g��ʐ؂�ւ��֐�
		*/
		void SwitchCraftWindow();

		/*!
		@brief �N���t�gQTE���I����Ă��邩�̊m�F�֐�
		*/
		void CheckedCraftQTE();

		/*!
		@brief �A�C�e����Ԃ̍X�V�֐�
		*/
		void UpdateStatus();

		/*!
		@brief �ړ��X�V�֐�
		*/
		void UpdateMove() override;

		/*!
		@brief State�ύX�֐�
		@param �V�����X�e�[�g�̃|�C���^
		*/
		void SetState(const shared_ptr<PlayerState>& newState)
		{
			m_playerState->SetState(newState);
		}

		/*!
		@brief �A�C�e�����ǉ��֐�
		@param �A�C�e���^�C�venum
		@param �ǉ���(�f�t�H1)
		*/
		void AddItemCount(eItemType type, int addNum = 1)
		{
			m_craft->AddItemCount(type, addNum);
		}

	public:

		/*!
		@brief �A�C�e�����擾�֐�
		@param �A�C�e���^�C�venum
		@return �A�C�e����
		*/
		int GetItemCount(eItemType type)
		{
			return m_craft->GetItemCount(type);
		}

		/*!
		@brief ��Ԏ擾�֐�
		@param �v���C���[�̏��enum
		@return ���̏�ԂɂȂ��Ă��邩�̐^�U
		*/
		bool GetStatus(ePlayerStatus status) const
		{
			return m_status(status);
		}

		/*!
		@brief �N���t�g�ł���̎擾�֐�
		@param �N���t�g�A�C�e��enum
		@return �N���t�g�ł��邩�̐^�U
		*/
		bool GetCraftPosshible(eCraftItem item = eCraftItem::Rail) const
		{
			return m_craft->CraftOrder();
		}

		/*!
		@brief �R���g���[���[�Ō����������擾�֐�
		@return atan2f(m_rotTarget.z, m_rotTarget.x)
		*/
		float GetPastRotTarget() const
		{
			return atan2f(m_rotTarget.z, m_rotTarget.x);
		}
	};
}