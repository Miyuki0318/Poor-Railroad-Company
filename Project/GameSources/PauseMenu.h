#pragma once
#include "stdafx.h"

namespace basecross
{
	class PauseMenu : public GameObject
	{
		enum State {
			Open,	// �I�[�v��
			Close,	// �N���[�Y
			Opened, // �I�[�v����
			Closed	// �N���[�Y��
		};
		enum eButtons {
			Retry,
			BackTitle
		};
		State m_state;	// ���݂̏��
		eButtons m_currentButton;
		eButtons m_pastButton;

		const Vec3 m_CloseMenuPos;	 // �N���[�Y�����j���[�ʒu
		const Vec3 m_OpenMenuPos;	 // �I�[�v�������j���[�ʒu
		const Vec2 m_CloseMenuScale; // �N���[�Y���T�C�Y
		const Vec2 m_OpenMenuScale;	 // �I�[�v�����T�C�Y

		const Vec3 m_DefaultRetryButtonPos; // ���g���C�{�^���\���ʒu
		const Vec3 m_DefaultTitleButtonPos; // �^�C�g���{�^���\���ʒu
		const Vec2 m_DefaultButtonScale;	// �{�^���T�C�Y

		const float m_LerpSpeed; // ���`��ԑ��x
		const float m_ScaleSpeed; // �傫���ύX���x
		const float m_MaxScaleRatio; 
		const float m_MinScaleRatio;

		float m_lerpRatio;	// ���`��Ԋ���
		float m_scaleRatio; // �傫���ύX����
		float m_pastStickVal;

		weak_ptr<Sprite> m_menuSprite;		// ���j���[�摜
		weak_ptr<Sprite> m_retrySprite;		// ���g���C�{�^���摜
		weak_ptr<Sprite> m_titleBackSprite;	// �^�C�g���{�^���摜
		map<eButtons, weak_ptr<Sprite>> m_buttonSprites; // �{�^���p�X�v���C�g

		/// <summary>
		/// ��Ԃ��Ƃ̏���
		/// </summary>
		/// <param name="state">���݂̏��</param>
		void StateProcess(State state); 

	public:
		PauseMenu(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_CloseMenuPos(Vec3(1920.0f, 1080.0f, 0.0f)),
			m_OpenMenuPos(Vec3(0.0f)),
			m_CloseMenuScale(Vec2(0.0f)),
			m_OpenMenuScale(Vec2(1920.0f, 1080.0f)),
			m_DefaultRetryButtonPos(Vec3(-350.0f, -380.0f, 0.0f)),
			m_DefaultTitleButtonPos(Vec3(350.0f, -380.0f, 0.0f)),
			m_DefaultButtonScale(Vec2(275.0f, 100.0f)),
			m_LerpSpeed(3.0f),
			m_ScaleSpeed(2.0f),
			m_MaxScaleRatio(1.25f),
			m_MinScaleRatio(1.0f),
			m_state(State::Closed),
			m_lerpRatio(0.0f)
		{
		}
		~PauseMenu() {}

		void OnCreate() override;
		void OnUpdate() override;

		/// <summary>
		/// ���j���[�I�[�v�����ɌĂяo���֐�
		/// </summary>
		void OnOpen();

		/// <summary>
		/// ���j���[�N���[�Y���ɌĂяo���֐�
		/// </summary>
		void OnClose();

		void ButtonSelect();

		void ButtonAction();
	};
}