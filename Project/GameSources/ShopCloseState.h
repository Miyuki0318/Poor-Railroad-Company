/*!
@file ShopCloseState.h
@brief �V���b�v�̖��I����ԃX�e�[�g
@author �����
*/

#pragma once
#include "ShopState.h"

namespace basecross
{
	class ShopCloseState : public ShopState
	{
	public:
		/*!
		@brief	�R���X�g���N�^
		*/
		ShopCloseState() {	}

		/*!
		@brief	�f�X�g���N�^
		*/
		virtual ~ShopCloseState() {}

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
		static shared_ptr<ShopCloseState> Instance();
	};
}
