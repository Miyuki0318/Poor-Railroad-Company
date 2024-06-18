/*!
@file TitlePlayerPauseState.h
@brief �v���C���[�̒�~��ԃX�e�[�g
@author ���V���M
*/

#pragma once
#include "TitlePlayerStateMachine.h"

namespace basecross
{
	/*!
	@brief �ړ���Ԃ̃v���C���[�X�e�[�g
	*/
	class TitlePlayerPauseState : public TitlePlayerState
	{
		/*!
		@brief �R���X�g���N�^
		*/
		TitlePlayerPauseState() {}

	public:

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~TitlePlayerPauseState() {}

		/*!
		@brief �C���X�^���X�֐�
		@return �V�����������ꂽthis�|�C���^
		*/
		static shared_ptr<TitlePlayerPauseState> Instance();

		/*!
		@brief �X�e�[�g���擾�֐�
		@return �X�e�[�g�̖��O
		*/
		wstring GetStateName() const override;

		/*!
		@brief �X�e�[�g�J�n���ɌĂяo�����֐�
		@param �X�e�[�g�}�V���Q�ƃI�u�W�F�N�g�̃|�C���^
		*/
		void Enter(const shared_ptr<TitlePlayer>& player) override;

		/*!
		@brief �X�e�[�g���s���ɌĂяo�����֐�
		@param �X�e�[�g�}�V���Q�ƃI�u�W�F�N�g�̃|�C���^
		*/
		void Execute(const shared_ptr<TitlePlayer>& player) override;

		/*!
		@brief �X�e�[�g�I�����ɌĂяo�����֐�
		@param �X�e�[�g�}�V���Q�ƃI�u�W�F�N�g�̃|�C���^
		*/
		void Exit(const shared_ptr<TitlePlayer>& player) override;

		/*!
		@brief B�{�^�����͂����ꂽ���Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		void OnPushB(const shared_ptr<TitlePlayer>& player) override;
	};
}