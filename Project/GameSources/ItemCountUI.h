/*!
@file ItemCountUI.h
@brief �A�C�e���̃J�E���^UI
@author ���V���M
*/

#pragma once
#include "Number.h"
#include "CraftManager.h"

namespace basecross
{
	/*!
	@brief �A�C�e����UI
	*/
	class ItemCountUI : public GameObject
	{	
		weak_ptr<Sprite> m_backSprite;	// �w�i�X�v���C�g
		weak_ptr<Sprite> m_itemSprite;	// �A�C�e���A�C�R���X�v���C�g
		weak_ptr<Sprite> m_slashSprite;	// �^�X�v���C�g

		// �A�C�e�����Ə�����̃X�v���C�g
		pair<weak_ptr<Number>, weak_ptr<Number>> m_itemNums;
		pair<weak_ptr<Number>, weak_ptr<Number>> m_limitNums;

		const float m_spriteScale;	// �X�v���C�g�̃X�P�[��
		const float m_numberScale;	// �����̃X�P�[��
		const float m_limitScale;	// ������̃X�P�[��
		const Vec2 m_backScale;		// �w�i�X�v���C�g�̃X�P�[��
		const Vec3 m_startPos;		// �J�n���W
		const Vec3 m_spriteMargin;	// �X�v���C�g���̗]��
		const Vec3 m_numberMargin;	// �������̗]��
		const Vec3 m_limitMargin;	// ��������̗]��
		const Vec3 m_limitPos;		// ������p�̍���
		const wstring m_texKey;		// �e�N�X�`��
		const eItemType m_itemType;	// �A�C�e���^�C�v

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �X�P�[��
		@param �|�W�V����
		@param �e�N�X�`��
		@param �A�C�e���^�C�v
		*/
		ItemCountUI(const shared_ptr<Stage>& stagePtr,
			const float scale,
			const Vec3& position,
			const wstring& texture,
			eItemType itemType
		) :
			GameObject(stagePtr),
			m_startPos(position),
			m_texKey(texture),
			m_itemType(itemType),
			m_spriteScale(scale * 1.3f),
			m_numberScale(scale * 0.7f),
			m_limitScale(scale * 0.3f),
			m_spriteMargin(scale * 1.15f, 0.0f, 0.0f),
			m_numberMargin(scale * 0.5f, 0.0f, 0.0f),
			m_limitMargin(scale * 0.35f, 0.0f, 0.0f),
			m_limitPos(0.0f, -scale * 0.35f / 2.0f, 0.0f),
			m_backScale(Vec2(scale * 4.25f, scale * 2.0f))
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~ItemCountUI() {}

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