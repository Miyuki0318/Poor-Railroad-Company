/*!
@file CraftWindow.h
@brief �N���t�g�E�B���h�E
@author ���V���M
*/
#pragma once
#include "Sprite.h"

namespace basecross
{
	// �E�B���h�E�`�����enum
	enum class eWindowRect
	{
		UpperRight,	// �E��
		UpperLeft,	// ����
		UnderRight,	// �E��
		UnderLeft,	// ����
	};

	// �E�B���h�E���\������Ă��邩enum
	enum class eWindowEnable : uint8_t
	{
		IsEnable,     // ���ݕ\������
		IsPastEnable, // �O��̏��
	};

	/*!
	@brief �N���t�g�E�B���h�E
	*/
	class CraftWindow : public Sprite
	{
		// �\������
		Bool8_t<eWindowEnable> m_enable;

		const Vec2 m_showScale;	 // �W�J���̃X�P�[��
		const float m_showSpeed; // �W�J���x
		float m_showRatio;		 // �W�J����

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		CraftWindow(const shared_ptr<Stage>& stagePtr) :
			Sprite(stagePtr, L"C_WINDOW_TX", Vec2(0.0f), Vec3(0.0f)),
			m_showScale(Vec2(200.0f)),
			m_showSpeed(0.25f)
		{
			m_enable = 0;
			m_showRatio = 0.0f;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~CraftWindow() {}

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
		void UpdateWindow();

		/*!
		@brief �\����\���ݒ�֐�
		@param bool
		*/
		void SetDrawEnable(bool enable);

		/*!
		@brief �`��ʒu�ݒ�֐�
		@param eWindowRect
		*/
		void SetWindowRect(eWindowRect rect);

		/*!
		@brief �E�B���h�E�`�悪�����������̐^�U�擾�֐�
		@return �������Ă���true�A����ȊO��false
		*/
		bool GetShowWindow() const
		{
			return m_enable(eWindowEnable::IsEnable, eWindowEnable::IsPastEnable);
		}
	};
}