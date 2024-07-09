/*!
@file PauseMenu.h
@brief �|�[�Y���j���[
@prod ��I�t
*/

#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "OzawaUtility.h"

namespace basecross
{
	class PauseMenu : public GameObject
	{
		enum State {
			Open,	// �I�[�v��
			Close,	// �N���[�Y
			Opened, // �I�[�v����
			Closed,	// �N���[�Y��
			Selected // �{�^���Z���N�g��
		};
		enum eMenuTypes {
			KeyBoard,
			XBoxPad,
			MenuNum
		};
		enum eButtons {
			Retry,		// ���g���C
			BackTitle,	// �^�C�g���ɖ߂�
			ButtonNum
		};
		State m_state;	// ���݂̏��
		eMenuTypes m_currentMenuType; // ���t���[���̃��j���[�^�C�v
		eMenuTypes m_pastMenuType;	  // �O�t���[���̃��j���[�^�C�v
		eButtons m_currentButton;	  // ���t���[���őI�𒆂̃{�^��
		eButtons m_pastButton;		  // �O�t���[���őI�����Ă����{�^��

		const Vec3 m_CloseMenuPos;	 // �N���[�Y�����j���[�ʒu
		const Vec3 m_OpenMenuPos;	 // �I�[�v�������j���[�ʒu
		const Vec2 m_CloseMenuScale; // �N���[�Y���T�C�Y
		const Vec2 m_OpenMenuScale;	 // �I�[�v�����T�C�Y

		const Vec3 m_DefaultRetryButtonPos; // ���g���C�{�^���\���ʒu
		const Vec3 m_DefaultTitleButtonPos; // �^�C�g���{�^���\���ʒu
		const Vec2 m_DefaultButtonScale;	// �{�^���T�C�Y

		const int m_MenuLayerNum;	// ���j���[�`�惌�C���[�ԍ�
		const int m_ButtonLayerNum;	// �{�^���`�惌�C���[�ԍ�

		const float m_LerpSpeed;		// ���`��ԑ��x
		const float m_ButtonScaleSpeed;	// �傫���ύX���x
		const float m_MaxScaleRatio;	// �傫���ύX���
		const float m_MinScaleRatio;	// �傫���ύX����

		float m_lerpRatio;	  // ���`��Ԋ���
		float m_scaleRatio;   // �傫���ύX����
		float m_pastStickVal; // ���O��L�X�e�B�b�N�̗�

		map<eMenuTypes, weak_ptr<Sprite>> m_menuSprites; // ���j���[�摜
		map<eButtons, weak_ptr<Sprite>> m_buttonSprites; // �{�^���p�X�v���C�g

		/// <summary>
		/// ��Ԃ��Ƃ̏���
		/// </summary>
		/// <param name="state">���݂̏��</param>
		void StateProcess(State state); 

		/// <summary>
		/// ���j���[�^�C�v���菈��
		/// </summary>
		void MenuTypeDecision();

		/// <summary>
		/// ���j���[�`��؂�ւ�
		/// </summary>
		/// <param name="drawFlag"></param>
		/// <param name="mType"></param>
		void SetDrawActiveMenu(bool drawFlag, eMenuTypes mType);

	public:
		PauseMenu(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_CloseMenuPos(Vec3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f)),
			m_OpenMenuPos(Vec3(0.0f, 0.0f, 0.0f)),
			m_CloseMenuScale(Vec2(0.0f)),
			m_OpenMenuScale(Vec2(WINDOW_WIDTH, WINDOW_HEIGHT)),
			m_DefaultRetryButtonPos(Vec3(-350.0f, -380.0f, 0.0f)),
			m_DefaultTitleButtonPos(Vec3(350.0f, -380.0f, 0.0f)),
			m_DefaultButtonScale(Vec2(275.0f, 100.0f)),
			m_MenuLayerNum(8),
			m_ButtonLayerNum(9),
			m_LerpSpeed(3.0f),
			m_ButtonScaleSpeed(2.0f),
			m_MaxScaleRatio(1.25f),
			m_MinScaleRatio(1.0f),
			m_state(State::Closed),
			m_currentMenuType(eMenuTypes::KeyBoard),
			m_pastMenuType(eMenuTypes::XBoxPad),
			m_currentButton(eButtons::Retry),
			m_pastButton(eButtons::BackTitle),
			m_lerpRatio(0.0f),
			m_scaleRatio(0.0f),
			m_pastStickVal(0.0f)
		{
		}
		~PauseMenu() {}

		void OnCreate() override;
		void OnUpdate() override;

		/// <summary>
		/// ���j���[�I�[�v�����ɌĂяo���֐�
		/// </summary>
		bool OnOpen();

		/// <summary>
		/// ���j���[�N���[�Y���ɌĂяo���֐�
		/// </summary>
		bool OnClose();

		/// <summary>
		/// �{�^���Z���N�g���̏���	
		/// </summary>
		void ButtonSelect();

		/// <summary>
		/// �^�C�g���{�^���Z���N�g��̏���
		/// </summary>
		void TitleButton();

		/// <summary>
		/// ���g���C�{�^���Z���N�g��̏���
		/// </summary>
		void RetryButton();

		/// <summary>
		/// �{�^����DrawActive��؂�ւ��鏈��
		/// </summary>
		/// <param name="drawFlag">DrawActive�̃I���I�t</param>
		void SetDrawActiveButtons(bool drawFlag);
	};
}