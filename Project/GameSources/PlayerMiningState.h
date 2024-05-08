/*!
@file PlayerMiningState.h
@brief �v���C���[�̍̌@��ԃX�e�[�g
@author ���V���M
*/

#pragma once
#include "PlayerState.h"

namespace basecross
{
	/*!
	@brief �̌@��Ԃ̃v���C���[�X�e�[�g
	*/
	class PlayerMiningState : public PlayerState
	{
		/*!
		@brief �R���X�g���N�^
		*/
		PlayerMiningState() {}

	public:

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~PlayerMiningState() {}

		/*!
		@brief �C���X�^���X�֐�
		@return �V�����������ꂽthis�|�C���^
		*/
		static shared_ptr<PlayerMiningState> Instance();

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
	};
}