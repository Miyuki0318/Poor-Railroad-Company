/*!
@file PaymentsState.h
@brief �J���������̎������Z�\���X�e�[�g
@author ���V���M
*/

#pragma once
#include "Number.h"
#include "GameStage.h"
#include "MoneyCountUI.h"

namespace basecross
{
	// ���Z���̃X�e�[�genum
	enum class eGamePaymentsState : char
	{
		MenuFadeIn,			// ���j���[�t�F�[�h�C��
		RewardCount,		// �J����V
		RailsSales,			// ���[�����p����
		GoldBarSales,		// ���򔄋p����
		ResourceSales,		// �������p����
		RailsInstallations,	// ���[���ݒu��p
		TotalIncome,		// ���v����
		DrawStandBy,		// �\���ҋ@
		MenuFadeOut,		// ���j���[�t�F�[�h�A�E�g
		StandBy,			// �ҋ@���
	};

	// �Q�[���X�e�[�W�̖��O�錾
	class GameStage;

	/*!
	@brief ��V���Z���̃X�e�[�g
	*/
	class PaymentsState
	{
		// �����X�e�[�W�|�C���^
		weak_ptr<GameStage> m_stage;

		// ���j���[�X�v���C�g
		weak_ptr<Sprite> m_menuSprite;	

		// ������UI
		weak_ptr<MoneyCountUI> m_moneyCount;

		// ���l�X�v���C�g�}�b�v
		map<eGamePaymentsState, NumberCount> m_numbersMap;
		
		// �X�e�[�g�֐��}�b�v
		map<eGamePaymentsState, function<void()>> m_stateFunc;

		const float m_numberScale;	// �����̃X�P�[��
		const float m_moneyScale;	// ������UI�̃X�P�[��
		const float m_menuFadeTime;	// �t�F�[�h����
		const float m_standByTime;	// �ς邽�߂̎���
		const Vec3 m_startPosition;	// �J�n���W
		const Vec3 m_numberMarginX;	// �������m�̗]��
		const Vec3 m_numberMarginY;	// �������m�̗]��
		const Vec3 m_totalMargin;	// ���v�����̗]��
		const Vec3 m_menuPosition;	// ���j���[�̍��W
		const Vec3 m_moneyStartPos;	// ������UI�̊J�n���̍��W

		eGamePaymentsState m_state;	// �����X�e�[�g

		float m_fadeTotalTime;		// �t�F�[�h�p�o�ߎ���
		vector<float> m_totalTime;	// �o�ߎ��Ԕz��

	public:
		
		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		PaymentsState(const shared_ptr<GameStage>& stagePtr) :
			m_stage(stagePtr),
			m_menuFadeTime(1.5f),
			m_standByTime(4.0f),
			m_numberScale(55.0f),
			m_moneyScale(75.0f),
			m_startPosition(135.0f, 75.0f, 0.0f),
			m_numberMarginX(45.0f, 0.0f, 0.0f),
			m_numberMarginY(0.0f, -70.0f, 0.0f),
			m_totalMargin(0.0f, -25.0f, 0.0f),
			m_menuPosition(0.0f, -200.0f, 0.1f),
			m_moneyStartPos(650.0f, 1000.0f, 0.2f)
		{
			m_state = eGamePaymentsState::MenuFadeIn;
			m_fadeTotalTime = 0.0f;
			m_totalTime = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

			m_stateFunc.emplace(eGamePaymentsState::MenuFadeIn, bind(&PaymentsState::MenuFadeInState, this));
			m_stateFunc.emplace(eGamePaymentsState::RewardCount, bind(&PaymentsState::RewardCountState, this));
			m_stateFunc.emplace(eGamePaymentsState::RailsSales, bind(&PaymentsState::RailsSalesState, this));
			m_stateFunc.emplace(eGamePaymentsState::GoldBarSales, bind(&PaymentsState::GoldBarSalesState, this));
			m_stateFunc.emplace(eGamePaymentsState::ResourceSales, bind(&PaymentsState::ResourceSalesState, this));
			m_stateFunc.emplace(eGamePaymentsState::RailsInstallations, bind(&PaymentsState::RailsInstallationsState, this));
			m_stateFunc.emplace(eGamePaymentsState::TotalIncome, bind(&PaymentsState::TotalIncomeState, this));
			m_stateFunc.emplace(eGamePaymentsState::DrawStandBy, bind(&PaymentsState::DrawStandByState, this));
			m_stateFunc.emplace(eGamePaymentsState::MenuFadeOut, bind(&PaymentsState::MenuFadeOutState, this));
		}

		/*!
		@brief �f�X�g���N�^
		*/
		~PaymentsState() {}

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
		@brief �����̌����ƃC���f�b�N�X�̐ݒ�֐�
		@param �^�C�v
		@param �ݒ肷����z
		*/
		void SetNumberGoal(eGamePaymentsState type, int money)
		{
			if (m_numbersMap.find(type) == m_numbersMap.end()) return;

			auto& numbers = m_numbersMap.at(type);
			numbers.goal = money;
			numbers.degit = max(Utility::GetUpperDigit(money) - 1, 0);
			numbers.index = numbers.degit;
		}

		/*!
		@brief �X�e�[�g�擾�֐�
		@return eGamePaymentsState
		*/
		eGamePaymentsState GetPaymentsState() const
		{
			return m_state;
		}

	private:

		/*!
		@brief ���j���[�X�v���C�g�t�F�[�h�C���X�e�[�g
		*/
		void MenuFadeInState();

		/*!
		@brief �J����V�\���X�e�[�g
		*/
		void RewardCountState();

		/*!
		@brief ���H���p�����\���e�[�g
		*/
		void RailsSalesState();

		/*!
		@brief ���򔄋p�����\���X�e�[�g
		*/
		void GoldBarSalesState();

		/*!
		@brief �������p�����\���X�e�[�g
		*/
		void ResourceSalesState();

		/*!
		@brief ���[���ݒu��p�\���X�e�[�g
		*/
		void RailsInstallationsState();

		/*!
		@brief ���v�����\���X�e�[�g
		*/
		void TotalIncomeState();

		/*!
		@brief �\���ҋ@��ԃX�e�[�g
		*/
		void DrawStandByState();

		/*!
		@brief ���j���[�X�v���C�g�t�F�[�h�A�E�g�X�e�[�g
		*/
		void MenuFadeOutState();

		/*!
		@brief �����ړ��֐�
		@param ���l�\����
		@param �J�n���̍��W����̈ړ���
		*/
		void MoveNumbersPosition(const NumberCount& count, const Vec3& move);
	};
}