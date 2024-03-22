/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	/*!
	@brief �Q�[�����̃X�e�[�W
	*/
	class GameStage : public Stage
	{
		/*!
		@brief �r���[�ƃ��C�g�̐���
		*/
		void CreateViewLight();

		/*!
		@brief �X�e�[�W�{�b�N�X�̐���
		*/
		void CreateGroundBox();

	public:

		//�\�z�Ɣj��
		GameStage() :Stage() {}

		virtual ~GameStage() {}

		//������
		virtual void OnCreate() override;
	};


}
//end basecross

