/*!
@file GamePlayerCraftingState.h
@brief �v���C���[�̃N���t�g��ԃX�e�[�g
@author ���V���M
*/

#pragma once
#include "GamePlayerStateMachine.h"

namespace basecross
{
	/*!
	@brief �N���t�g��Ԃ̃v���C���[�X�e�[�g
	*/
	class GamePlayerCraftingState : public GamePlayerState
	{
		// �N���t�g�J�n�{�^������
		enum class eCurrentCraftInput : unsigned char
		{
			None,		// ������
			PushStartA,	// A�{�^�����͂ŊJ�n����
			PushStartB,	// B�{�^�����͂ŊJ�n����
			PushStartY,	// Y�{�^�����͂ŊJ�n����
		};

		// �N���t�g�J�n�{�^��
		eCurrentCraftInput m_currentInput;
		eCurrentCraftInput m_pastInput;

		bool m_isStartCraft; // �N���t�g�J�n�ł��邩�̐^�U

		/*!
		@brief �R���X�g���N�^
		*/
		GamePlayerCraftingState() 
		{
			m_isStartCraft = true;
			m_currentInput = eCurrentCraftInput::None;
			m_pastInput = eCurrentCraftInput::None;
		}


	public:

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GamePlayerCraftingState() {}

		/*!
		@brief �C���X�^���X�֐�
		@return �V�����������ꂽthis�|�C���^
		*/
		static shared_ptr<GamePlayerCraftingState> Instance();

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

		/*!
		@brief �{�^�����͂��s���AQTE���~������֐�
		*/
		void PushedQTE(const shared_ptr<GamePlayer>& player);

		/*!
		@brief �N���t�gQTE���I����Ă��邩�̊m�F�֐�
		*/
		void CheckedCraftQTE(const shared_ptr<GamePlayer>& player);

		/*!
		@brief �N���t�gQTE��~���Ɏ��s����֐�
		*/
		void StoppedCraftQTE(const shared_ptr<GamePlayer>& player);
	};
}