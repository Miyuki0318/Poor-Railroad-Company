/*!
@file ShopConfirmationState.h
@brief �V���b�v�̖��I����ԃX�e�[�g
@author �����
*/

#pragma once
#include "ShopState.h"

namespace basecross
{
	class ShopConfirmationState : public ShopState
	{
	private:
		// �I���
		Shop::eConfirmState m_confirmation;
		// �������e�̌��݂̃��x��
		int m_currentLv;
		// �������e�̌��݂̔�p
		int m_cost;
		// ������p�̌���
		int m_digit;

	public:
		/*!
		@brief	�R���X�g���N�^
		*/
		ShopConfirmationState() {
			// �ϐ��̏�����
			m_confirmation = Shop::eConfirmState::No;
			m_currentLv = 0;
			m_cost = 0;
			m_digit = 0;
		}

		/*!
		@brief	�f�X�g���N�^
		*/
		virtual ~ShopConfirmationState() {}

		/*!
		@brief �X�e�[�g�ɓ��������Ɏ��s�����
		@param �V���b�v�̃|�C���^
		*/
		void Enter(const shared_ptr<Shop>& shop) override;

		/*!
		@brief Update�̎��Ɏ��s�����
		@param �V���b�v�̃|�C���^
		*/
		void Execute(const shared_ptr<Shop>& shop) override;

		/*!
		@brief �X�e�[�g���o�鎞�Ɏ��s�����
		@param �V���b�v�̃|�C���^
		*/
		void Exit(const shared_ptr<Shop>& shop) override;

		/*!
		@brief �C���X�^���X�֐�
		@return �V�����������ꂽthis�|�C���^
		*/
		static shared_ptr<ShopConfirmationState> Instance();

		/*!
		@brief �R���g���[���[�p����֐�
		@param
		*/
		void  ControllerSelect(const shared_ptr<Shop>& shop);

		/*!
		@brief �R���g���[���[�p����֐�
		*/
		void  MouseSelect(const shared_ptr<Shop>& shop);

	};
}
