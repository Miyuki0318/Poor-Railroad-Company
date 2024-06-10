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
		IsIdle,			// �ҋ@���
		IsMove,			// �ړ����
		IsRotate,		// ��]���
		IsGathering,	// �̌@���
		IsHaveWood,		// �؂�������
		IsHaveStone,	// �΂�������
		IsCrafting,		// �N���t�g��
		IsCraftQTE,		// �N���t�gQTE��
		IsHaveRail,		// ���H������
	};

	// �v���C���[�̏�ԃX�e�[�g�N���X(���O�̂ݐ錾)
	class PlayerIdleState;		// �ҋ@���
	class PlayerMovingState;	// �ړ����
	class PlayerMiningState;	// �̌@���
	class PlayerCraftingState;	// �N���t�g���
	class PlayerSuccesState;	// �N���t�g���
	class PlayerFailedState;	// �N���t�g���

	/*!
	@brief �Q�[�����̃v���C���[
	*/
	class GamePlayer : public Player
	{
		weak_ptr<SelectIndicator> m_indicator; // �Z���N�g�C���f�B�P�[�^�[
		unique_ptr<CraftManager> m_craft; // �N���t�g�}�l�[�W���[

		map<eStageID, pair<eItemType, wstring>> m_gatherMap; // �̎�ΏۂƎ擾�A�C�e���^�C�v
		Bool16_t<ePlayerStatus> m_status; // �t���O�Ǘ��N���X

		// �X�e�[�g�}�V��
		unique_ptr<PlayerStateMachine> m_playerState;

		const Vec3 m_startPosition;			// �J�n���̍��W
		const Vec3 m_goalStagingPosition;	// �S�[�����o���̍��W
		Vec3 m_goalPosition;				// �S�[�����̍��W

		// �t�����h��(�X�e�[�g�}�V�����烁���o�֐����Ăяo������)
		friend PlayerIdleState;
		friend PlayerMovingState;
		friend PlayerMiningState;
		friend PlayerCraftingState;
		friend PlayerSuccesState;
		friend PlayerFailedState;

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
			m_status = 0; // ��ԃt���O��0�ŏ�����

			// �̎�I�u�W�F�N�g��ID�ƍ̎掞�Ɉ����f�[�^
			m_gatherMap.emplace(eStageID::Stone1, make_pair(eItemType::Stone, L"ROCK"));
			m_gatherMap.emplace(eStageID::Stone2, make_pair(eItemType::Stone, L"ROCK"));
			m_gatherMap.emplace(eStageID::Stone3, make_pair(eItemType::Stone, L"ROCK"));
			m_gatherMap.emplace(eStageID::Tree1, make_pair(eItemType::Wood, L"TREE"));
			m_gatherMap.emplace(eStageID::Tree2, make_pair(eItemType::Wood, L"TREE"));
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
		@brief �̌@���ߊ֐�
		@param �C���f�B�P�[�^�[�̃|�C���^
		@return �̌@�ł�����
		*/
		bool GatheringOrder(const shared_ptr<SelectIndicator>& indicator);

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
		@brief �̌@���ɌĂяo�����֐�
		@param �̌@�����I�u�W�F�N�g�̃^�O
		*/
		void GatheringProcces(int stageID);

		/*!
		@brief �A�N�V�������ɃC���f�B�P�[�^�[�̕��։�]�ݒ肷��֐�
		*/
		void SetRotateIndicatorAngle();

		/*!
		@brief �N���t�g��ʐ؂�ւ��֐�
		*/
		void SwitchCraftWindow();

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
		void AddItemCount(eItemType type, int addNum = 1)
		{
			m_craft->AddItemCount(type, addNum);
		}

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
		bool GetCraftPosshible() const
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