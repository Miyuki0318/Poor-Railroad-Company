/*!
@file Shop.h
@brief �V���b�v�@�\�̎���
@author �����
*/

#pragma once
#include "TemplateObject.h"
#include "ShopState.h"


namespace basecross {
	// �t�����h������N���X���̒�`
	class ShopState;
	class ShopCloseState;
	class ShopShoppingState;
	class ShopConfirmationState;

	// �V���b�v�N���X
	class Shop : public TemplateObject
	{
	public:
		// �������e�萔
		enum Enhancements {
			Backpack, // �o�b�N�p�b�N
			Status, // �X�e�[�^�X
			Gear //����
		};

		// �X�e�[�g�Ǘ��p�萔
		enum ShopState {
			Close, // �V���b�v���I��
			Shopping, // �w����
			Confirmation, // �w�����e�m�F��
		};

	private:
		// �X�e�[�g�}�V���̃t�����h��
		friend ShopState;
		friend ShopCloseState;
		friend ShopShoppingState;
		friend ShopConfirmationState;

		// �V���b�v�̌��݂̃X�e�[�g��ۑ�
		ShopState m_currentState;

		// �V���b�v�̃X�e�[�g�}�V���̃|�C���^�����L
		unique_ptr<ShopStateMachine> m_ShopState;


	public:
		Shop(const shared_ptr<Stage>& stagePtr // �X�e�[�W�̃|�C���^
		) :
			TemplateObject(stagePtr)
		{
			// ���݂̃X�e�[�g��Close�ɏ�����
			m_currentState = Close;
		}

		// �f�X�g���N�^
		virtual ~Shop() {
			m_ShopState.reset();
		}

		/*!
		@brief	 �N���X���ŏ��ɍ쐬����鎞�Ɏ��s�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief	 ���t���[�����s�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief	 �X�e�[�g��ύX����֐�
		@param[in]	newState �V�����X�e�[�g�̃|�C���^
		*/
		void SetState(const shared_ptr<ObjState<Shop>>& newState) {
			m_ShopState->ChangeState(newState);
		};

		/*!
		@brief	 ���݂̃X�e�[�g���擾����֐�
		@return ���݂̃X�e�[�g�̃|�C���^
		*/
		ShopState GetState() {
			return m_currentState;
		};


	};
}