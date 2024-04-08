/*!
@file Player.h
@brief �v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "TemplateObject.h"
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
	@brief �v���C���[
	*/
	class Player : public TemplateObject
	{
		weak_ptr<SelectIndicator> m_indicator; // �Z���N�g�C���f�B�P�[�^�[
		unique_ptr<CraftManager> m_craft;      // �N���t�g�}�l�[�W���[

		shared_ptr<BcPNTBoneModelDraw> m_ptrDraw;   // �`��R���|�[�l���g
		shared_ptr<CollisionCapsule> m_ptrColl;    // �R���W����OBB�R���|�[�l���g
		Bool16_t<ePlayerStatus> m_status;	   // �t���O�Ǘ��N���X
		
		map<wstring, eItemType> m_miningMap;     // �̌@�ΏۂƎ擾�A�C�e���^�C�v

		// �X�e�[�g�}�V��
		unique_ptr<PlayerStateMachine> m_playerState; 

		const float m_moveSpeed; // �ړ����x
		const float m_rotSpeed;  // ��]���x

		Vec3 m_rotTarget;	// ��]��
		Vec3 m_currentRot;  // �O��̉�]��
		Mat4x4 m_modelMat;	// ���f���ƃg�����X�t�H�[�������s��

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
		Player(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 3.0f, 0.0f), Vec3(0.0f), Vec3(1.0f)),
			m_moveSpeed(5.0f), // ����CSV���瑬�x���̃X�e�[�^�X���擾�\��
			m_rotSpeed(0.5f)  // ����CSV���瑬�x���̃X�e�[�^�X���擾�\��
		{
			m_status = 0; // ��ԃt���O��0�ŏ�����
			m_rotTarget.zero(); // ��]���0.0f�ŏ�����
			m_currentRot.zero(); // ��]���0.0f�ŏ�����

			// �X�P�[�������AY��������2�{�ɂ���
			m_modelMat.affineTransformation(
				Vec3(1.0f, 2.0f, 1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);

			// �̌@�I�u�W�F�N�g�̃^�O�ƍ̌@���ɉ��Z����A�C�e���̃^�C�v
			m_miningMap.insert(make_pair(L"Tree", eItemType::Wood));	// �^�O���؂Ȃ�A�C�e���^�C�v�͖؍�
			m_miningMap.insert(make_pair(L"Rock", eItemType::Stone)); // �^�O����Ȃ�A�C�e���^�C�v�͐΍�
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Player() 
		{
			m_craft.reset();
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
		@brief �C���f�B�P�[�^�[�ւ̎擾�ƌĂяo���֐�
		*/
		void IndicatorOrder();

		/*!
		@brief �̌@���ɌĂяo�����֐�
		@param �̌@�����I�u�W�F�N�g�̃|�C���^
		*/
		void MiningProcces(const shared_ptr<TemplateObject>& miningObj);

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
		void UpdateMove();

		/*!
		@brief ��]�X�V�֐�
		*/
		void UpdateRotation();

		/*!
		@brief ��]��ݒ�֐�
		@param L�X�e�B�b�N���͗�
		*/
		void SetRotateTarget(const Vec3& stickValue);

		/*!
		@brief �R���g���[���[�ړ��֐�
		@param L�X�e�B�b�N���͗�
		*/
		void ControllerMovement(const Vec3& stickValue);

		/*!
		@brief State�ύX�֐�
		@param �V�����X�e�[�g�̃|�C���^
		*/
		void SetState(const shared_ptr<PlayerState>& newState)
		{
			m_playerState->SetState(newState);
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
		@brief ��Ԏ擾�֐�
		@param �v���C���[�̏��enum
		@return ���̏�ԂɂȂ��Ă��邩�̐^�U
		*/
		bool GetStatus(ePlayerStatus status) const
		{
			return m_status(status);
		}
	};
}