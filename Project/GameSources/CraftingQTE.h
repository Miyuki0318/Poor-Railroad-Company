/*!
@file CraftingQTE.h
@brief �N���t�g����QTE
@author ���V���M
*/

#pragma once
#include "CraftUI.h"

namespace basecross
{
	// QTE���͂���{�^��
	enum class eInputButton
	{
		ButtonB, // �{�^��B
		ButtonA, // �{�^��A
		ButtonY, // �{�^��Y
	};

	/*!
	@brief �N���t�gQTE
	*/
	class CraftingQTE : public CraftUI
	{
		weak_ptr<CraftUI> m_barFlame;	// �o�[�̃t���[��
		weak_ptr<CraftUI> m_qtePoint;	// QTE�̃^�C�~���O�͈�
		weak_ptr<CraftUI> m_qteButton;	// QTE�œ��͂���{�^��
		weak_ptr<CraftUI> m_qteBack;	// QTE�̔w�i

		map<eRectType, Vec3> m_rectDiff;
		map<eInputButton, wstring> m_buttonTexMap;
		map<eInputButton, Col4> m_buttonBarColor;

		const float m_posDiff;  // �E�B���h�E�Ƃ̍��W����
		const float m_qteRatio; // QTE�̐����ʒu�̊���
		const float m_qteRange;	// QTE�͈̔�
		const float m_barSpeed; // QTE�o�[�̑��x
		float m_barRatio; // QTE�o�[�̊���
		bool m_qteEnable; // QTE�����̐^�U

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		CraftingQTE(const shared_ptr<Stage>& stagePtr) :
			CraftUI(stagePtr, L"WHITE_TX", Vec2(300.0f, 15.0f), 0.25f),
			m_posDiff(195.0f),
			m_qteRatio(0.75f),
			m_qteRange(0.1f),
			m_barSpeed(1.0f)
		{
			m_barRatio = 0.0f;
			m_qteEnable = false;

			m_rectDiff.emplace(eRectType::UpRight, Vec3(1.0f, 1.0f, 1.0f));
			m_rectDiff.emplace(eRectType::UpLeft, Vec3(-1.0f, 1.0f, 1.0f));
			m_rectDiff.emplace(eRectType::DownRight, Vec3(1.0f, -1.0f, 1.0f));
			m_rectDiff.emplace(eRectType::DownLeft, Vec3(-1.0f, -1.0f, 1.0f));

			m_buttonTexMap.emplace(eInputButton::ButtonB, L"BUTTON_B_TX");
			m_buttonTexMap.emplace(eInputButton::ButtonA, L"BUTTON_A_TX");
			m_buttonTexMap.emplace(eInputButton::ButtonY, L"BUTTON_Y_TX");

			m_buttonBarColor.emplace(eInputButton::ButtonB, Col4(1.0f, 0.23f, 0.18f, 1.0f));
			m_buttonBarColor.emplace(eInputButton::ButtonA, Col4(0.0f, 1.0f, 0.6f, 1.0f));
			m_buttonBarColor.emplace(eInputButton::ButtonY, Col4(0.9f, 0.84f, 0.0f, 1.0f));
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
		@param window�̍��W
		*/
		void SetDrawEnable(bool enable, Vec3 windowPos);

		/*!
		@brief �`��ύX�ݒ�֐�
		@param eRectType
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

		/*!
		@brief QTE���͂���{�^���e�N�X�`���ݒ�֐�
		@param �{�^�����̓^�C�v
		*/
		void SetButtonTexture(eInputButton button)
		{
			SetDiffuseColor(m_buttonBarColor.at(button));
			m_qteButton.lock()->SetTexture(m_buttonTexMap.at(button));
		}
	};
}