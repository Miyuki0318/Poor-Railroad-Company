/*!
@file GameClearState.h
@brief �Q�[���N���A���̏����X�e�[�g
@author ���V���M
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	// �N���A���̃X�e�[�genum
	enum class eGameClearState : char
	{
		RailFadeIn,		// ���[���̃t�F�[�h
		SelectFadeIn,	// �I�����̃t�F�[�h�C��
		SelectStage,	// �X�e�[�W�I��
		SelectFadeOut,	// �I�����̃t�F�[�h�A�E�g
		StandBy,		// �ҋ@���
	};

	// �I�������X�e�[�W�^�C�v
	enum class eSelectStage : char
	{
		NextStage,	// ���̃X�e�[�W
		TitleBack,	// �^�C�g���X�e�[�W
	};

	// �Q�[���X�e�[�W�̖��O�錾
	class GameStage;

	/*!
	@brief �Q�[���N���A���̃X�e�[�g
	*/
	class GameClearState
	{
		// �����X�e�[�W�|�C���^
		weak_ptr<GameStage> m_stage;

		// �N���A���̑I�����X�v���C�g
		weak_ptr<Sprite> m_nextStageSprite;	// �l�N�X�g�X�e�[�W
		weak_ptr<Sprite> m_clearBackSprite;	// �N���A���̃^�C�g���o�b�N
		weak_ptr<Sprite> m_railLineSprite;	// �N���A���̃^�C�g���o�b�N

		const Vec2 m_defScale;	// �X�P�[��
		const Vec3 m_leftPos;	// �I�������̍��W
		const Vec3 m_rightPos;	// �I�����E�̍��W
		const Vec3 m_railPos;	// ���[���̍��W
		const Vec3 m_moveVal;	// ������

		const float m_boundScale;		// �o�E���h���̃X�P�[��
		const float m_railFadeTime;		// ���[���̃t�F�[�h����
		const float m_selectFadeTime;	// �I�����̃t�F�[�h����

		float m_totalTime;	// �o�ߎ���
		float m_pastStick;	// �O��̃X�e�B�b�N����
		
		eSelectStage m_selectStage; // �I�������X�e�[�W
		eSelectStage m_pastSelect;	// �O��I�������X�e�[�W
		eGameClearState m_clearState; // �X�e�[�g

		// �X�e�[�g�ɉ������֐��Ăяo���}�b�v
		map<eGameClearState, function<void()>> m_stateFunc;

		// �I�����ɉ������X�v���C�g�}�b�v
		map<eSelectStage, weak_ptr<Sprite>> m_selectSprite;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		GameClearState(const shared_ptr<GameStage>& stagePtr) :
			m_stage(stagePtr),
			m_boundScale(1.25f),
			m_railFadeTime(3.0f),
			m_selectFadeTime(5.0f),
			m_defScale(275.0f, 100.0f),
			m_leftPos(-300.0f, -200.0f, 0.0f),
			m_rightPos(300.0f, -200.0f, 0.0f),
			m_railPos(-1920.0f, -250.0f, 0.2f),
			m_moveVal(1920.0f, 0.0f, 0.0f)
		{
			m_totalTime = 0.0f;
			m_pastStick = 0.0f;

			m_selectStage = eSelectStage::NextStage;
			m_pastSelect = eSelectStage::TitleBack;

			m_clearState = eGameClearState::RailFadeIn;
			m_stateFunc.emplace(eGameClearState::RailFadeIn, bind(&GameClearState::RailSpriteFadeIn, this));
			m_stateFunc.emplace(eGameClearState::SelectFadeIn, bind(&GameClearState::SelectSpriteFadeIn, this));
			m_stateFunc.emplace(eGameClearState::SelectStage, bind(&GameClearState::SelectStageState, this));
			m_stateFunc.emplace(eGameClearState::SelectFadeOut, bind(&GameClearState::SelectSpriteFadeOut, this));
		}

		/*!
		@brief �f�X�g���N�^
		*/
		~GameClearState() {}

		/*!
		@brief �X�v���C�g�̐����֐�
		*/
		void CreateState();

		/*!
		@brief �X�e�[�g�̍X�V�����֐�
		*/
		void UpdateState();

		/*!
		@brief �X�e�[�g�̏������֐�
		*/
		void ResetState();

		/*!
		@brief �X�e�[�g�擾�֐�
		@return m_clearState
		*/
		eGameClearState GetClearState() const
		{
			return m_clearState;
		}

		/*!
		@brief �I�������X�e�[�W�擾�֐�
		@return m_selectStage
		*/
		eSelectStage GetSelectStage() const
		{
			return m_selectStage;
		}

	private:

		/*!
		@brief ���[���X�v���C�g�̃t�F�[�h�C������
		*/
		void RailSpriteFadeIn();

		/*!
		@brief �I�����v���C�g�̃t�F�[�h�C������
		*/
		void SelectSpriteFadeIn();

		/*!
		@brief �X�e�[�W�I�𒆂̏���
		*/
		void SelectStageState();

		/*!
		@brief �I�����v���C�g�̃t�F�[�h�C������
		*/
		void SelectSpriteFadeOut();
	};
}