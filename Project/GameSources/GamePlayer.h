/*!
@file GamePlayer.h
@brief �Q�[�����̃v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "Player.h"
#include "ItemFly.h"
#include "GamePlayerStateMachine.h"

namespace basecross
{
	// �v���C���[�̏�ԃX�e�[�g�N���X(���O�̂ݐ錾)
	class GamePlayerIdleState;		// �ҋ@���
	class GamePlayerMovingState;	// �ړ����
	class GamePlayerGatheringState;	// �̌@���
	class GamePlayerCraftingState;	// �N���t�g���
	class GamePlayerSuccesState;	// �N���t�g���
	class GamePlayerFailedState;	// �N���t�g���

	/*!
	@brief �Q�[�����̃v���C���[
	*/
	class GamePlayer : public Player
	{
		weak_ptr<FlyItemManager> m_itemFly;	// �A�C�e�������ł����G�t�F�N�g
		unique_ptr<CraftManager> m_craft; // �N���t�g�}�l�[�W���[

		// �X�e�[�g�}�V��
		unique_ptr<GamePlayerStateMachine> m_playerState;

		const Vec3 m_startPosition;			// �J�n���̍��W
		const Vec3 m_goalStagingPosition;	// �S�[�����o���̍��W
		Vec3 m_goalPosition;				// �S�[�����̍��W

		// �t�����h��(�X�e�[�g�}�V�����烁���o�֐����Ăяo������)
		friend GamePlayerIdleState;
		friend GamePlayerMovingState;
		friend GamePlayerGatheringState;
		friend GamePlayerCraftingState;
		friend GamePlayerSuccesState;
		friend GamePlayerFailedState;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		GamePlayer(const shared_ptr<Stage>& stagePtr, 
			const Vec3& start, const Vec3& goal) :
			Player(stagePtr),
			m_startPosition(start),
			m_goalStagingPosition(goal)
		{
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

		/*!
		@brief ���Z�b�g�����֐�
		*/
		void ResetPlayer();

	private:

		/*!
		@brief �v���C���[�ɕt������@�\�����֐�
		*/
		void CreatePlayerFeatures() override;

		/*!
		@brief �C���f�B�P�[�^�[�ւ̎擾�ƌĂяo���֐�
		*/
		void IndicatorOrder() override;

		/*!
		@brief ���[���ǉ����ߊ֐�
		@param �C���f�B�P�[�^�[�̃|�C���^
		@return �ݒu�ł�����
		*/
		bool AddRailOrder(const shared_ptr<SelectIndicator>& indicator);

		/*!
		@brief �؂̑���ǉ����ߊ֐�
		@param �C���f�B�P�[�^�[�̃|�C���^
		@return �ݒu�ł�����
		*/
		bool AddBridgeOrder(const shared_ptr<SelectIndicator>& indicator);

		/*!
		@brief ���ؒǉ����ߊ֐�
		@param �C���f�B�P�[�^�[�̃|�C���^
		@return �ݒu�ł�����
		*/
		bool AddCrossingOrder(const shared_ptr<SelectIndicator>& indicator);

		/*!
		@brief �N���t�g��ʐ؂�ւ��֐�
		*/
		void SwitchCraftWindow();

		/*!
		@brief �A�C�e����Ԃ̍X�V�֐�
		*/
		void UpdateStatus();

		/*!
		@brief �A�C�e�����̍X�V�֐�
		*/
		void UpdateItemCount();

		/*!
		@brief �ړ��X�V�֐�
		*/
		void UpdateMove() override;

		/*!
		@brief State�ύX�֐�
		@param �V�����X�e�[�g�̃|�C���^
		*/
		void SetState(const shared_ptr<GamePlayerState>& newState)
		{
			m_playerState->SetState(newState);
		}

	public:

		/*!
		@brief �Q�[�����ʎ��p�̃X�e�[�g�ݒ�֐�
		@param eGameProgress
		*/
		void SetGameResult(eGameProgress result);

		/*!
		@brief �A�C�e�����ǉ��֐�
		@param �A�C�e���^�C�venum
		@param �ǉ���(�f�t�H1)
		*/
		void AddItemCount(eItemType type, int addNum = 1) override
		{
			m_craft->AddItemCount(type, addNum);
		}

		/*!
		@brief �A�C�e�����擾�֐�
		@param �A�C�e���^�C�venum
		@return �A�C�e����
		*/
		int GetItemCount(eItemType type) override
		{
			return m_craft->GetItemCount(type);
		}

		int GetLimitCount(eItemType type)
		{
			int retVal = 0;
			switch (type)
			{
			case eItemType::Wood:
				retVal = (int)m_playerData.at(ePST::WoodMax).at(m_backPackLevel);
				break;

			case eItemType::Stone:
				retVal = (int)m_playerData.at(ePST::StoneMax).at(m_backPackLevel);
				break;

			case eItemType::Gear:
				retVal = (int)m_playerData.at(ePST::GearMax).at(m_backPackLevel);
				break;

			case eItemType::Rail:
				retVal = (int)m_playerData.at(ePST::RailMax).at(m_backPackLevel);
				break;

			case eItemType::WoodBridge:
				retVal = (int)m_playerData.at(ePST::BridgeMax).at(m_backPackLevel);
				break;

			case eItemType::Crossing:
				retVal = (int)m_playerData.at(ePST::CrossingMax).at(m_backPackLevel);
				break;

			default:
				break;
			}

			return retVal;
		}

		/*!
		@brief �N���t�g�ł���̎擾�֐�
		@param �N���t�g�A�C�e��enum
		@return �N���t�g�ł��邩�̐^�U
		*/
		bool GetCraftPosshible() const override
		{
			bool rail = m_craft->CraftOrder(eCraftItem::Rail);
			bool bridge = m_craft->CraftOrder(eCraftItem::WoodBridge);
			bool crossing = m_craft->CraftOrder(eCraftItem::Crossing);

			return rail || bridge || crossing;
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