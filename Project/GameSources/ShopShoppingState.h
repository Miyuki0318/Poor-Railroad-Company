/*!
@file ShopShoppingState.h
@brief �V���b�v�̖��I����ԃX�e�[�g
@author �����
*/

#pragma once
#include "ShopState.h"

namespace basecross
{
	class ShopShoppingState : public ShopState
	{
	private:
		// �������e�̌��݂̔�p
		vector<int> m_cost;
		// ������p�̌���
		vector<int> m_digit;

	public:
		/*!
		@brief	�R���X�g���N�^
		*/
		ShopShoppingState() {
			m_cost.resize(Shop::eEnhancements::EnhancementsSize);
			m_digit.resize(Shop::eEnhancements::EnhancementsSize);
		}

		/*!
		@brief	�f�X�g���N�^
		*/
		virtual ~ShopShoppingState() {}

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
		static shared_ptr<ShopShoppingState> Instance();

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
