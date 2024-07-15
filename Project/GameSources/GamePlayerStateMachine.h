/*!
@file GamePlayerStateMachine.h
@brief �v���C���[�̏�ԃX�e�[�g
@author ���V���M
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	// ���O�p
	class GamePlayer;

	/*!
	@brief �v���C���[�p�̃X�e�[�g
	*/
	class GamePlayerState : public ObjState<GamePlayer>
	{
	public:

		/*!
		@brief	�R���X�g���N�^
		*/
		GamePlayerState() {}

		/*!
		@brief	�f�X�g���N�^
		*/
		virtual ~GamePlayerState() {}

		/*!
		@brief �X�e�[�g�ɓ��������Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void Enter(const shared_ptr<GamePlayer>& player) = 0;

		/*!
		@brief Update�̎��Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void Execute(const shared_ptr<GamePlayer>& player) = 0;

		/*!
		@brief �X�e�[�g���o�鎞�Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void Exit(const shared_ptr<GamePlayer>& player) = 0;

		/*!
		@brief A�{�^�����͂����ꂽ���Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void OnPushA(const shared_ptr<GamePlayer>& player) = 0;

		/*!
		@brief A�{�^�����͂����ꂽ���Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void OnPushB(const shared_ptr<GamePlayer>& player) = 0;

		/*!
		@brief X�{�^�����͂����ꂽ���Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void OnPushX(const shared_ptr<GamePlayer>& player) = 0;

		/*!
		@brief X�{�^�����͂����ꂽ���Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void OnPushY(const shared_ptr<GamePlayer>& player) = 0;

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
	class GamePlayerStateMachine : public StateMachine<GamePlayer>
	{
	public:

		/*!
		@brief �R���X�g���N�^
		@param �v���C���[�̃|�C���^
		*/
		explicit GamePlayerStateMachine(const shared_ptr<GamePlayer>& owner) :
			StateMachine(owner)
		{
		}

		/*!
		@brief	�f�X�g���N�^
		*/
		virtual ~GamePlayerStateMachine() {}
		
		/*!
		@brief �X�e�[�g��ύX����
		@param �ݒ肷��X�e�[�g
		*/
		void SetState(const shared_ptr<GamePlayerState>& newState)
		{
			StateMachine::ChangeState(dynamic_pointer_cast<ObjState<GamePlayer>>(newState));
		}

		/*!
		@brief	A�{�^�����͂����ꂽ��
		*/
		void PushA() const
		{
			// �v���C���[�ƃX�e�[�g�̎擾
			const auto& player = m_Owner.lock();
			const auto& state = dynamic_pointer_cast<GamePlayerState>(m_CurrentState.lock());
			if (state && player)
			{
				// �X�e�[�g��A�{�^�����͎��̏����𑗂�
				state->OnPushA(player);
			}
		}

		/*!
		@brief	A�{�^�����͂����ꂽ��
		*/
		void PushB() const
		{
			// �v���C���[�ƃX�e�[�g�̎擾
			const auto& player = m_Owner.lock();
			const auto& state = dynamic_pointer_cast<GamePlayerState>(m_CurrentState.lock());
			if (state && player)
			{
				// �X�e�[�g��A�{�^�����͎��̏����𑗂�
				state->OnPushB(player);
			}
		}

		/*!
		@brief	X�{�^�����͂����ꂽ��
		*/
		void PushX() const
		{
			// �v���C���[�ƃX�e�[�g�̎擾
			const auto& player = m_Owner.lock();
			const auto& state = dynamic_pointer_cast<GamePlayerState>(m_CurrentState.lock());
			if (state && player)
			{
				// �X�e�[�g��A�{�^�����͎��̏����𑗂�
				state->OnPushX(player);
			}
		}

		/*!
		@brief	X�{�^�����͂����ꂽ��
		*/
		void PushY() const
		{
			// �v���C���[�ƃX�e�[�g�̎擾
			const auto& player = m_Owner.lock();
			const auto& state = dynamic_pointer_cast<GamePlayerState>(m_CurrentState.lock());
			if (state && player)
			{
				// �X�e�[�g��A�{�^�����͎��̏����𑗂�
				state->OnPushY(player);
			}
		}
	};
}