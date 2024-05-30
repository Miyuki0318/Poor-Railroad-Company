/*!
@file GameTrainCurveExitState.h
@brief �Q�[�����̗�Ԃ̃J�[�u�I�����̃X�e�[�g
@author ���V���M
*/

#pragma once
#include "stdafx.h"
#include "GameTrain.h"

namespace basecross
{
	/*!
	@brief �Q�[�����̃J�[�u���[���I�����̃X�e�[�g
	*/
	class GameTrainCurveExitState : public ObjState<GameTrain>
	{
		/*!
		@brief �R���X�g���N�^
		*/
		GameTrainCurveExitState() {}

	public:

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GameTrainCurveExitState() {}

		/*!
		@brief �C���X�^���X�֐�
		@return �V�����������ꂽthis�|�C���^
		*/
		static shared_ptr<GameTrainCurveExitState> Instance();

		/*!
		@brief �X�e�[�g���擾�֐�
		@return �X�e�[�g�̖��O
		*/
		wstring GetStateName() const override;

		/*!
		@brief �X�e�[�g�J�n���ɌĂяo�����֐�
		@param �X�e�[�g�}�V���Q�ƃI�u�W�F�N�g�̃|�C���^
		*/
		void Enter(const shared_ptr<GameTrain>& train) override;

		/*!
		@brief �X�e�[�g���s���ɌĂяo�����֐�
		@param �X�e�[�g�}�V���Q�ƃI�u�W�F�N�g�̃|�C���^
		*/
		void Execute(const shared_ptr<GameTrain>& train) override;

		/*!
		@brief �X�e�[�g�I�����ɌĂяo�����֐�
		@param �X�e�[�g�}�V���Q�ƃI�u�W�F�N�g�̃|�C���^
		*/
		void Exit(const shared_ptr<GameTrain>& train) override;
	};
}