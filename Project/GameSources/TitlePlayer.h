/*!
@file TitlePlayer.h
@brief �^�C�g���p�v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "Player.h"
#include "TitlePlayerStateMachine.h"

namespace basecross
{
	// �v���C���[�̏�ԃX�e�[�g�N���X(���O�̂ݐ錾)
	class TitlePlayerIdleState;			// �ҋ@���
	class TitlePlayerMovingState;		// �ړ����
	class TitlePlayerPauseState;		// ��~���
	class TitlePlayerGatheringState;	// �̌@���

	/*!
	@brief �^�C�g���p�v���C���[
	*/
	class TitlePlayer : public Player
	{
		// �X�e�[�g�}�V��
		unique_ptr<TitlePlayerStateMachine> m_playerState;

		friend TitlePlayerIdleState;
		friend TitlePlayerMovingState;
		friend TitlePlayerPauseState;
		friend TitlePlayerGatheringState;

	public:
		
		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		TitlePlayer(const shared_ptr<Stage>& stagePtr) :
			Player(stagePtr)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~TitlePlayer() 
		{
			m_playerState.reset();
		}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

	private:

		/*!
		@brief �v���C���[�ɕt������@�\�����֐�
		*/
		void CreatePlayerFeatures() override;

		/*!
		@brief �C���f�B�P�[�^�[�ւ̎擾�ƌĂяo���֐�
		*/
		void IndicatorOrder() override;

		/*!
		@brief �ړ��X�V�֐�
		*/
		void UpdateMove() override;

		/*!
		@brief State�ύX�֐�
		@param �V�����X�e�[�g�̃|�C���^
		*/
		void SetState(const shared_ptr<TitlePlayerState>& newState)
		{
			m_playerState->SetState(newState);
		}
	};
}