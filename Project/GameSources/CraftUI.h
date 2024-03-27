/*!
@file CraftUI.h
@brief �N���t�gUI�̌p�����I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	// �E�B���h�E���\������Ă��邩enum
	enum class eWindowEnable : uint8_t
	{
		IsEnable,     // ���ݕ\������
		IsPastEnable, // �O��̏��
	};

	/*!
	@brief �N���t�g�Ɋւ���UI�̌p����
	*/
	class CraftUI : public Sprite
	{
	protected:

		// �\������
		Bool8_t<eWindowEnable> m_drawEnable;

		const Vec2 m_drawScale;	 // �W�J���̃X�P�[��
		const float m_drawSpeed; // �W�J���x
		float m_drawRatio;		 // �W�J����

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �e�N�X�`���L�[
		@param �W�J���̃X�P�[��
		@param �W�J���x
		*/
		CraftUI(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr,
			const Vec2& drawScale,
			float drawSpeed
		) :
			Sprite(stagePtr, texWstr, Vec2(0.0f), Vec3(0.0f)),
			m_drawScale(drawScale),
			m_drawSpeed(drawSpeed)
		{
			m_drawRatio = 0.0f;
			m_drawEnable = 0;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~CraftUI() {}

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief �E�B���h�E�T�C�Y�̍X�V�֐�
		*/
		virtual void DrawWindow();

		/*!
		@brief �\����\���ݒ�֐�
		@param bool
		*/
		virtual void SetDrawEnable(bool enable);

		/*!
		@brief �E�B���h�E�`�悪�����������̐^�U�擾�֐�
		@return �������Ă���true�A����ȊO��false
		*/
		virtual bool GetShowWindow() const
		{
			return m_drawEnable(eWindowEnable::IsEnable, eWindowEnable::IsPastEnable);
		}
	};
}