/*!
@file CraftingQTE.h
@brief �N���t�g����QTE
@author ���V���M
*/

#pragma once
#include "CraftUI.h"

namespace basecross
{
	/*!
	@brief �N���t�gQTE
	*/
	class CraftingQTE : public CraftUI
	{
		weak_ptr<CraftUI> m_barFlame; // �o�[�̃t���[��
		weak_ptr<CraftUI> m_qtePoint; // QTE�̃^�C�~���O�͈�

		map<eRectType, Vec3> m_rectDiff;

		const float m_posDiff;  // �E�B���h�E�Ƃ̍��W����
		const float m_qteRatio; // QTE�̐����ʒu�̊���
		const float m_barSpeed; // QTE�o�[�̑��x
		float m_barRatio; // QTE�o�[�̊���
		bool m_qteEnable; // QTE�����̐^�U

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		CraftingQTE(const shared_ptr<Stage>& stagePtr) :
			CraftUI(stagePtr, L"WHITE_TX", Vec2(200.0f, 10.0f), 0.25f),
			m_posDiff(130.0f),
			m_qteRatio(0.75f),
			m_barSpeed(1.0f)
		{
			m_barRatio = 0.0f;
			m_qteEnable = false;

			m_rectDiff.emplace(eRectType::UpRight, Vec3(1.0f, 1.0f, 1.0f));
			m_rectDiff.emplace(eRectType::UpLeft, Vec3(-1.0f, 1.0f, 1.0f));
			m_rectDiff.emplace(eRectType::DownRight, Vec3(1.0f, -1.0f, 1.0f));
			m_rectDiff.emplace(eRectType::DownLeft, Vec3(-1.0f, -1.0f, 1.0f));
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
		@brief �E�B���h�E�T�C�Y�̍X�V�֐�
		*/
		void DrawWindow() override;

		/*!
		@brief �\����\���ݒ�֐�
		@param bool
		*/
		void SetDrawEnable(bool enable, Vec3 windowPos);

		/*!
		@brief �`��ύX�ݒ�֐�
		@param eVerticesRect
		*/
		void SetVerticesRect(eRectType rect) override;

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

		/*!
		@brief QTE�����̐^�U�擾�֐�
		*/
		bool GetEnableQTE() const
		{
			return m_qteEnable;
		}
	};
}