/*!
@file PlayerState.h
@brief �v���C���[�̏�ԃX�e�[�g
@author ���V���M
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	// ���O�p
	class Player;

	/*!
	@brief �v���C���[�p�̃X�e�[�g
	*/
	class PlayerState : public ObjState<Player>
	{
	public:

		/*!
		@brief	�R���X�g���N�^
		*/
		PlayerState() {}

		/*!
		@brief	�f�X�g���N�^
		*/
		virtual ~PlayerState() {}

		/*!
		@brief �X�e�[�g�ɓ��������Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void Enter(const shared_ptr<Player>& player) = 0;

		/*!
		@brief Update�̎��Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void Execute(const shared_ptr<Player>& player) = 0;

		/*!
		@brief �X�e�[�g���o�鎞�Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void Exit(const shared_ptr<Player>& player) = 0;

		/*!
		@brief A�{�^�����͂����ꂽ���Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void OnPushA(const shared_ptr<Player>& player) = 0;

		/*!
		@brief X�{�^�����͂����ꂽ���Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void OnPushX(const shared_ptr<Player>& player) = 0;

		/*!
		@brief	�X�e�[�g���𓾂�
		@param[in]	�X�e�[�g��
		@return	�Ȃ�
		*/
		virtual wstring GetStateName()const { return L""; }
	};

	/*!
	@brief �v���C���[�p�̃X�e�[�g�}�V��
	*/
	class PlayerStateMachine : public StateMachine<Player>
	{
	public:

		/*!
		@brief �R���X�g���N�^
		@param �v���C���[�̃|�C���^
		*/
		explicit PlayerStateMachine(const shared_ptr<Player>& owner) :
			StateMachine(owner)
		{
		}

		/*!
		@brief	�f�X�g���N�^
		*/
		virtual ~PlayerStateMachine() {}
		
		/*!
		@brief �X�e�[�g��ύX����
		@param �ݒ肷��X�e�[�g
		*/
		void SetState(const shared_ptr<PlayerState>& newState)
		{
			StateMachine::ChangeState(dynamic_pointer_cast<ObjState<Player>>(newState));
		}

		/*!
		@brief	A�{�^�����͂����ꂽ��
		*/
		void PushA() const
		{
			// �v���C���[�ƃX�e�[�g�̎擾
			const auto& player = m_Owner.lock();
			const auto& state = dynamic_pointer_cast<PlayerState>(m_CurrentState.lock());
			if (state && player)
			{
				// �X�e�[�g��A�{�^�����͎��̏����𑗂�
				state->OnPushA(player);
			}
		}

		/*!
		@brief	X�{�^�����͂����ꂽ��
		*/
		void PushX() const
		{
			// �v���C���[�ƃX�e�[�g�̎擾
			const auto& player = m_Owner.lock();
			const auto& state = dynamic_pointer_cast<PlayerState>(m_CurrentState.lock());
			if (state && player)
			{
				// �X�e�[�g��A�{�^�����͎��̏����𑗂�
				state->OnPushX(player);
			}
		}
	};
}