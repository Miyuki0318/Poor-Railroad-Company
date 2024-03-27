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
		weak_ptr<Sprite> m_barFlame; // �o�[�̃t���[��
		weak_ptr<Sprite> m_qtePoint; // QTE�̃^�C�~���O�͈�

		const Vec2 m_barScale;  // QTE�o�[�̃X�P�[��
		const float m_barSpeed; // QTE�o�[�̑��x
		float m_barRatio; // �o�[�̐i�񂾊���
		float m_qteRatio; // QTE�̐�������

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		CraftingQTE(const shared_ptr<Stage>& stagePtr) :
			Sprite(stagePtr, L"WHITE_TX", Vec2(0.0f, 25.0f), Vec3(0.0f)),
			m_barScale(Vec2(200.0f, 10.0f)),
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
		@brief QTE�̍X�V�֐�
		*/
		void UpdateQTE();

		/*!
		@brief QTE�J�n�֐�
		@param �N���t�g�E�B���h�E�̍��W
		*/
		void StartQTE(const Vec3& windowPos);

		/*!
		@brief QTE��~��QTE���ʎ擾�֐�
		@return �����������̐^�U
		*/
		bool StopQTE();
	};
}