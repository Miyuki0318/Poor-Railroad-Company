/*!
@file ShopState.h
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
	public:

		/*!
		@brief	�R���X�g���N�^
		*/
		ShopState() {}

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