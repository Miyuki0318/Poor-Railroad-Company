/*!
@file MoneyCountUI.h
@brief �������̃J�E���^UI
@author ���V���M
*/

#pragma once
#include "Scene.h"
#include "Number.h"

namespace basecross
{	
	/*!
	@brief ������UI
	*/
	class MoneyCountUI : public TemplateObject
	{
		weak_ptr<Sprite> m_backSprite;	// �w�i�X�v���C�g
		weak_ptr<Sprite> m_itemSprite;	// �A�C�e���A�C�R���X�v���C�g
		weak_ptr<Sprite> m_slashSprite;	// �^�X�v���C�g

		// �������̐��l�X�v���C�g
		NumberCount m_numbers;
		vector<float> m_totalTime; // �o�ߎ���
		bool m_isGoal; // �\�����I��������̐^�U
		bool m_isNow; // �v�Z�����̐^�U

		const float m_spriteScale;	// �X�v���C�g�̃X�P�[��
		const float m_numberScale;	// �����̃X�P�[��
		const Vec2 m_backScale;		// �w�i�X�v���C�g�̃X�P�[��
		const Vec3 m_startPos;		// �J�n���W
		const Vec3 m_spriteMargin;	// �X�v���C�g���̗]��
		const Vec3 m_numberMargin;	// �������̗]��
		const Vec3 m_backMargin;	// �w�i�̗]��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �X�P�[��
		@param �|�W�V����
		@param �e�N�X�`��
		@param �A�C�e���^�C�v
		*/
		MoneyCountUI(const shared_ptr<Stage>& stagePtr,
			const float scale,
			const Vec3& position
		) :
			TemplateObject(stagePtr),
			m_startPos(position),
			m_spriteScale(scale * 1.3f),
			m_numberScale(scale * 0.7f),
			m_spriteMargin(scale * 1.1f, 0.0f, 0.0f),
			m_numberMargin(scale * 0.55f, 0.0f, 0.0f),
			m_backScale(Vec2(scale * 5.0f, scale * 2.0f)),
			m_backMargin(Vec3(scale * 1.4f, 0.0f, 0.0f))
		{
			m_isGoal = true;
			m_isNow = false;
			m_totalTime = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~MoneyCountUI() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �J�n���̐��l�ݒ�֐�
		*/
		void StartSetNumbers();

		/*!
		@brief �����̍X�V
		*/
		void UpdateNumbers();

		/*!
		@brief �F�̍X�V
		*/
		void UpdateColor();

		/*!
		@brief ���W���ړ��ʂŉ��Z���čX�V
		@param �ړ���
		*/
		void SetMovePosition(const Vec3& moveVal);

		/*!
		@brief �����̌����ƃC���f�b�N�X�̐ݒ�֐�
		@param �ݒ肷����z
		*/
		void SetNumberGoal(int money)
		{
			// �������ݒ�
			m_isGoal = false;
			money = min(money, MONEY_LIMIT);
			m_numbers.goal = money;
			m_numbers.degit = max(Utility::GetUpperDigit(money) - 1, 0);
			m_numbers.index = m_numbers.degit;
			m_totalTime = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		}
	};
}