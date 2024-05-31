/*!
@file GameTrainCurvingState.h
@brief �Q�[�����̗�Ԃ̃J�[�u���̃X�e�[�g
@author ���V���M
*/

#pragma once
#include "stdafx.h"
#include "GameTrain.h"

namespace basecross
{
	/*!
	@brief �Q�[�����̃J�[�u���[�����s���̃X�e�[�g
	*/
	class GameTrainCurvingState : public ObjState<GameTrain>
	{
		float m_pastRotY;	// �ߋ��̉�]Y��
		CurvePoints m_curvePoints; // �J�[�u�Ɏg�����W

		/*!
		@brief �R���X�g���N�^
		*/
		GameTrainCurvingState() 
		{
			m_pastRotY = 0.0f;
			m_curvePoints = {};
		}

	public:

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GameTrainCurvingState() {}

		/*!
		@brief �C���X�^���X�֐�
		@return �V�����������ꂽthis�|�C���^
		*/
		static shared_ptr<GameTrainCurvingState> Instance();

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