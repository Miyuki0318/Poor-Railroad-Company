/*!
@file GamePlayerGatheringState.h
@brief �v���C���[�̍̎��ԃX�e�[�g
@author ���V���M
*/

#pragma once
#include "GamePlayerStateMachine.h"

namespace basecross
{
	/*!
	@brief �̎��Ԃ̃v���C���[�X�e�[�g
	*/
	class GamePlayerGatheringState : public GamePlayerState
	{
		bool m_isFlyEffect;	// �A�C�e���G�t�F�N�g���o������
		float m_animeHelfTime; // �A�j���[�V�����̔����̎���

		/*!
		@brief �R���X�g���N�^
		*/
		GamePlayerGatheringState() 
		{
			m_isFlyEffect = false;
			m_animeHelfTime = 0.0f;
		}

	public:

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GamePlayerGatheringState() {}

		/*!
		@brief �C���X�^���X�֐�
		@return �V�����������ꂽthis�|�C���^
		*/
		static shared_ptr<GamePlayerGatheringState> Instance();

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
		@brief B�{�^�����͂����ꂽ���Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		void OnPushB(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief X�{�^�����͂����ꂽ���Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		void OnPushX(const shared_ptr<GamePlayer>& player) override;

		/*!
		@brief Y�{�^�����͂����ꂽ���Ɏ��s�����
		@param �v���C���[�̃|�C���^
		*/
		void OnPushY(const shared_ptr<GamePlayer>& player) override;
	};
}