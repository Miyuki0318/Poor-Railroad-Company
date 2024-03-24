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
		shared_ptr<Sprite> m_gameClearLogo;
		shared_ptr<Sprite> m_gameOverLogo;

		/*!
		@brief �r���[�ƃ��C�g�̐���
		*/
		void CreateViewLight();

		/*!
		@brief �X�e�[�W�{�b�N�X�̐���
		*/
		void CreateGroundBox();

		/*!
		@brief �v���C���[����
		*/
		void CreatePlayer();

		/*!
		@brief ���\�[�X�̓Ǎ�
		*/
		void CreateResourses();

		/*!
		@brief �X�v���C�g�̐���
		*/
		void CreateSpriteObject();

		/*!
		@brief �X�v���C�g�̐���
		*/
		void LogoActive();

	public:

		//�\�z�Ɣj��
		GameStage() :Stage() {}

		virtual ~GameStage() {}

		//������
		virtual void OnCreate() override;
	};


}
//end basecross

