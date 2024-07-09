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
		// �������邩�̉�
		bool m_isEnhancements;
		// �������e�̌��݂̃��x��
		int m_currentLv;
		// �������e�̌��݂̃��x��
		int m_cost;

	public:
		/*!
		@brief	�R���X�g���N�^
		*/
		ShopConfirmationState() {
			// �ϐ��̏�����
			m_isEnhancements = false;
			m_currentLv = 0;
			m_cost = 0;
		}

		/*!
		@brief	�f�X�g���N�^
		*/
		virtual ~ShopConfirmationState() {}

		/*!
		@brief �X�e�[�g�ɓ��������Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		void Enter(const shared_ptr<Shop>& shop) override;

		/*!
		@brief Update�̎��Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		void Execute(const shared_ptr<Shop>& shop) override;

		/*!
		@brief �X�e�[�g���o�鎞�Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		void Exit(const shared_ptr<Shop>& shop) override;

		/*!
		@brief �C���X�^���X�֐�
		@return �V�����������ꂽthis�|�C���^
		*/
		static shared_ptr<ShopConfirmationState> Instance();
	};
}
