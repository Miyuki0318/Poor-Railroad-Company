/*!
@file MouseCursor.h
@brief �L�[�}�E���쎞�̃}�E�X�J�[�\��
@author ���V���M
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	/*!
	@brief �}�E�X�J�[�\��
	*/
	class MouseCursor : public Sprite
	{
	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		MouseCursor(const shared_ptr<Stage>& stagePtr) :
			Sprite(stagePtr, L"CURSOR_TX", Vec2(50.0f))
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~MouseCursor() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;
	};
}