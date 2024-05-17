/*!
@file PlayerMovingState.h
@brief �v���C���[�̈ړ���ԃX�e�[�g
@author ���V���M
*/

#pragma once
#include "PlayerState.h"

namespace basecross
{
	/*!
	@brief �ړ���Ԃ̃v���C���[�X�e�[�g
	*/
	class PlayerMovingState : public PlayerState
	{
		/*!
		@brief �R���X�g���N�^
		*/
		PlayerMovingState() {}

	public:

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~PlayerMovingState() {}

		/*!
		@brief �C���X�^���X�֐�
		@return �V�����������ꂽthis�|�C���^
		*/
		static shared_ptr<PlayerMovingState> Instance();

		/*!
		@brief �X�e�[�g���擾�֐�
		@return �X�e�[�g�̖��O
		*/
		wstring GetStateName() const override;

		/*!
		@brief �X�e�[�g�J�n���ɌĂяo�����֐�
		@param �X�e�[�g�}�V���Q�ƃI�u�W�F�N�g�̃|�C���^
		*/
		void Enter(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief �X�e�[�g���s���ɌĂяo�����֐�
		@param �X�e�[�g�}�V���Q�ƃI�u�W�F�N�g�̃|�C���^
		*/
		void Execute(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief �X�e�[�g�I�����ɌĂяo�����֐�
		@param �X�e�[�g�}�V���Q�ƃI�u�W�F�N�g�̃|�C���^
		*/
		void Exit(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief A�{�^�����͂����ꂽ���Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		void OnPushA(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief X�{�^�����͂����ꂽ���Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		void OnPushX(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief �ړ��X�V
		@param �v���C���[�̃|�C���^
		*/
		void UpdateMoving(const shared_ptr<GamePlayer>& player);
	};
}