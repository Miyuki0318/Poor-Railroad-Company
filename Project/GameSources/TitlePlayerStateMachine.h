/*!
@file TitlePlayerStateMachine.h
@brief �v���C���[�̏�ԃX�e�[�g
@author ���V���M
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	// ���O�p
	class TitlePlayer;

	/*!
	@brief �v���C���[�p�̃X�e�[�g
	*/
	class TitlePlayerState : public ObjState<TitlePlayer>
	{
	public:

		/*!
		@brief	�R���X�g���N�^
		*/
		TitlePlayerState() {}

		/*!
		@brief	�f�X�g���N�^
		*/
		virtual ~TitlePlayerState() {}

		/*!
		@brief �X�e�[�g�ɓ��������Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void Enter(const shared_ptr<TitlePlayer>& player) = 0;

		/*!
		@brief Update�̎��Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void Execute(const shared_ptr<TitlePlayer>& player) = 0;

		/*!
		@brief �X�e�[�g���o�鎞�Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void Exit(const shared_ptr<TitlePlayer>& player) = 0;

		/*!
		@brief A�{�^�����͂����ꂽ���Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		virtual void OnPushB(const shared_ptr<TitlePlayer>& player) = 0;

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
	class TitlePlayerStateMachine : public StateMachine<TitlePlayer>
	{
	public:

		/*!
		@brief �R���X�g���N�^
		@param �v���C���[�̃|�C���^
		*/
		explicit TitlePlayerStateMachine(const shared_ptr<TitlePlayer>& owner) :
			StateMachine(owner)
		{
		}

		/*!
		@brief	�f�X�g���N�^
		*/
		virtual ~TitlePlayerStateMachine() {}

		/*!
		@brief �X�e�[�g��ύX����
		@param �ݒ肷��X�e�[�g
		*/
		void SetState(const shared_ptr<TitlePlayerState>& newState)
		{
			StateMachine::ChangeState(dynamic_pointer_cast<ObjState<TitlePlayer>>(newState));
		}

		/*!
		@brief	A�{�^�����͂����ꂽ��
		*/
		void PushB() const
		{
			// �v���C���[�ƃX�e�[�g�̎擾
			const auto& player = m_Owner.lock();
			const auto& state = dynamic_pointer_cast<TitlePlayerState>(m_CurrentState.lock());
			if (state && player)
			{
				// �X�e�[�g��A�{�^�����͎��̏����𑗂�
				state->OnPushB(player);
			}
		}
	};
}