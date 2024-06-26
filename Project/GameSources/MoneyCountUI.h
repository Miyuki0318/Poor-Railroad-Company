/*!
@file MoneyCountUI.h
@brief �������̃J�E���^UI
@author ���V���M
*/

#pragma once
#include "Number.h"

namespace basecross
{	
	/*!
	@brief ������UI
	*/
	class MoneyCountUI : public GameObject
	{
		weak_ptr<Sprite> m_backSprite;	// �w�i�X�v���C�g
		weak_ptr<Sprite> m_itemSprite;	// �A�C�e���A�C�R���X�v���C�g
		weak_ptr<Sprite> m_slashSprite;	// �^�X�v���C�g

		// �������̐��l�X�v���C�g
		vector<weak_ptr<Number>> m_numbers;

		const float m_spriteScale;	// �X�v���C�g�̃X�P�[��
		const float m_numberScale;	// �����̃X�P�[��
		const Vec2 m_backScale;		// �w�i�X�v���C�g�̃X�P�[��
		const Vec3 m_startPos;		// �J�n���W
		const Vec3 m_spriteMargin;	// �X�v���C�g���̗]��
		const Vec3 m_numberMargin;	// �������̗]��

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
			GameObject(stagePtr),
			m_startPos(position),
			m_spriteScale(scale * 1.3f),
			m_numberScale(scale * 0.7f),
			m_spriteMargin(scale * 1.1f, 0.0f, 0.0f),
			m_numberMargin(scale * 0.55f, 0.0f, 0.0f),
			m_backScale(Vec2(scale * 5.0f, scale * 2.0f))
		{
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
		@brief �����̍X�V
		*/
		void UpdateNumbers();
	};
}