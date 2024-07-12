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
	
	// �����\���X�v���C�g
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


	// �����p�\����
	struct NumberCount
	{
		int goal = 0;
		int degit = 0;
		int index = 0;
		vector<weak_ptr<Number>> sprite;

		/*!
		@brief �A�Ԑ��l�\���֐�
		@param ���l�\����
		*/
		static void ConsecutiveNumberDraw(NumberCount& count, vector<float>& totalTime)
		{
			// �����̔�r�l��wss�ɕϊ�
			wstringstream time;
			time << count.goal;
			wstringstream total;
			total << static_cast<int>(totalTime.at(count.degit));

			// �z��̗v�f����ݒ�(���̌���-���݂̌���)
			int index = count.index - count.degit;

			// �������o�ߎ��Ԃ̌��݂̌����ԖڂōX�V
			count.sprite.at(index).lock()->SetDiffuseColor(COL_BLACK);
			count.sprite.at(index).lock()->SetNumber(static_cast<int>(totalTime.at(count.degit)));

			// �ڕW���̌��݂̌����ԖڂƁA�o�ߎ��Ԃ�0�Ԗڂ������Ȃ�
			bool notZero = (time.str()[count.degit] == '0'); // �ڕW����0�Ȃ�10�܂Ő����Ă���؂�ւ���
			if (time.str()[count.degit] == total.str()[int(notZero)])
			{
				// ���݂̌��������炷
				count.degit--;

				// 0��菬�����Ȃ�����
				if (count.degit < 0)
				{
					// �o�ߎ��Ԃ����������ďI��
					for (auto& time : totalTime)
					{
						time = 0.0f;
					}
					return;
				}
			}

			// ���݂̌����Ԗڂ̌o�ߎ��Ԃ����Z		
			const float fpsParTime = 0.6f;
			totalTime.at(count.degit) += fpsParTime;
		}
	};
}