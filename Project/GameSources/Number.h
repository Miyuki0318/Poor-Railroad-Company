/*!
@file Number.h
@brief ������\������X�v���C�g
@author ���V���M
*/

#pragma once
#include "stdafx.h"
#include "Sprite.h"

// ���̌v�Z�p�}�N��
#define ONE_PLACE(num) num / 1 % 10
#define TENS_PLACE(num) num / 10 % 10

namespace basecross
{
	// ----------------------------------------------------------------------------------
	//	Number : public Sprite ������\������X�v���C�g�̌p����
	// ----------------------------------------------------------------------------------
	class Number : public Sprite
	{
		int m_number; // ����

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �X�P�[��
		@param �|�W�V����
		*/
		Number(const shared_ptr<Stage>& stagePtr,
			const Vec2& scale, const Vec3& position
		) :
			Sprite(stagePtr, L"NUMBER_TX", scale, position)
		{
			m_number = 0;
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �X�P�[��
		@param �|�W�V����
		@param �e�N�X�`��
		*/
		Number(const shared_ptr<Stage>& stagePtr,
			const Vec2& scale, const Vec3& position,
			const wstring& texture, const int& number
		) :
			Sprite(stagePtr, texture, scale, position)
		{
			m_number = number;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Number() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief �\�����鐔����ύX����֐�
		@param ����
		*/
		void SetNumber(const int number);
	};
}