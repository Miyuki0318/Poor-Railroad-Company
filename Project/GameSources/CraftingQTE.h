/*!
@file CraftingQTE.h
@brief �N���t�g����QTE
@author ���V���M
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	/*!
	@brief �N���t�gQTE
	*/
	class CraftingQTE : public Sprite
	{
		const float m_barSpeed; // QTE�o�[�̑��x
		float m_barRatio; // �o�[�̐i�񂾊���
		float m_qteRatio; // QTE�̐�������

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		CraftingQTE(const shared_ptr<Stage>& stagePtr) :
			Sprite(stagePtr, L"WHITE_TX", Vec2(0.0f), Vec3(0.0f)),
			m_barSpeed(1.0f)
		{
			m_barRatio = 0.0f;
			m_qteRatio = 0.0f;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~CraftingQTE() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief QTE�J�n�֐�
		*/
		void StartQTE();

		/*!
		@brief QTE��~��QTE���ʎ擾�֐�
		@return �����������̐^�U
		*/
		bool StopQTE();
	};
}