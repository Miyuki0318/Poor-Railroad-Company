/*!
@file eShopState.h
@brief �V���b�v�̏�ԃX�e�[�g
@author �����
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	// ���O�p
	class Shop;

	/*!
	@brief �V���b�v�p�̃X�e�[�g
	*/
	class ShopState : public ObjState<Shop>
	{
	protected:
		// L�X�e�B�b�N���͂̉����l
		float m_LStickLowerLimit;
		// L�X�e�B�b�N���͗ʂƉ����l���r�����l��L�X�e�B�b�N�̓��̗͂L���Ƃ��ĕێ�
		bool m_isInputLStick;
		// �������e���ύX����Ă���̌o�ߎ���
		float m_totalTime;

	public:

		/*!
		@brief	�R���X�g���N�^
		*/
		ShopState() {
			// L�X�e�B�b�N���͂̉����l��ݒ�
			m_LStickLowerLimit = 0.3f;

			// �ϐ��̏�����
			m_isInputLStick = false;
			m_totalTime = 0.0f;
		}

		/*!
		@brief	�f�X�g���N�^
		*/
		virtual ~ShopState() {}

		/*!
		@brief �X�e�[�g�ɓ��������Ɏ��s�����
		@param �V���b�v�̃|�C���^
		*/
		virtual void Enter(const shared_ptr<Shop>& shop) = 0;

		/*!
		@brief Update�̎��Ɏ��s�����
		@param �V���b�v�̃|�C���^
		*/
		virtual void Execute(const shared_ptr<Shop>& shop) = 0;

		/*!
		@brief �X�e�[�g���o�鎞�Ɏ��s�����
		@param �V���b�v�̃|�C���^
		*/
		virtual void Exit(const shared_ptr<Shop>& shop) = 0;
	};

	/*!
	@brief �V���b�v�p�̃X�e�[�g�}�V��
	*/
	class ShopStateMachine : public StateMachine<Shop>
	{
	public:

		/*!
		@brief �R���X�g���N�^
		@param �V���b�v�̃|�C���^
		*/
		explicit ShopStateMachine(const shared_ptr<Shop>& owner) :
			StateMachine(owner)
		{
		}

		/*!
		@brief	�f�X�g���N�^
		*/
		virtual ~ShopStateMachine() {}

		/*!
		@brief �X�e�[�g��ύX����
		@param �ݒ肷��X�e�[�g
		*/
		void SetState(const shared_ptr<ShopState>& newState)
		{
			StateMachine::ChangeState(dynamic_pointer_cast<ObjState<Shop>>(newState));
		}
	};
}