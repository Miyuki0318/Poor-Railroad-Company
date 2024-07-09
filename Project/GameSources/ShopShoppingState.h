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

	public:
		/*!
		@brief	�R���X�g���N�^
		*/
		ShopShoppingState() {
		}

		/*!
		@brief	�f�X�g���N�^
		*/
		virtual ~ShopShoppingState() {}

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
		static shared_ptr<ShopShoppingState> Instance();
	};
}
