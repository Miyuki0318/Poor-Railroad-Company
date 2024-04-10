/*!
@file TitlePlayer.h
@brief �^�C�g���p�v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "Player.h"

namespace basecross
{
	/*!
	@brief �^�C�g���p�v���C���[
	*/
	class TitlePlayer : public Player
	{
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
		virtual ~TitlePlayer() {}

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

	private:

		/*!
		@brief �R���|�[�l���g�̐����֐�
		*/
		void CreateComponent() override;

		/*!
		@brief �ړ��X�V�֐�
		*/
		void UpdateMove() override;
	};
}