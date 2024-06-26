/*!
@file CraftItemIcon.h
@brief �N���t�g����A�C�e���A�C�R��
@author ���V���M
*/

#pragma once
#include "CraftUI.h"

namespace basecross
{
	/*!
	@brief �N���t�g�A�C�e���A�C�R��
	*/
	class CraftItemIcon : public CraftUI
	{
		bool m_craftPosshible; // �N���t�g�ł��邩�̐^�U

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �e�N�X�`���L�[
		*/
		CraftItemIcon(const shared_ptr<Stage>& stagePtr,
			const wstring texture
		) :
			CraftUI(stagePtr, texture, Vec2(200.0f), 0.25f)
		{
			m_craftPosshible = false;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~CraftItemIcon() {}

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �N���t�g�ł��邩�̐^�U�ݒ�֐�
		@param �N���t�g�ł��邩
		*/
		void SetCraftPosshible(bool posshible)
		{
			m_craftPosshible = posshible;
		}
	};
}