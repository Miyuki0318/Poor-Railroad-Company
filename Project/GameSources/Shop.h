/*!
@file Shop.h
@brief �V���b�v�@�\�̎���
@author �����
*/

#pragma once
#include "TemplateObject.h"
#include "ShopState.h"
#include "Number.h"
#include "CSVLoader.h"


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
		enum eEnhancements {
			Backpack, // �o�b�N�p�b�N
			Status, // �X�e�[�^�X
			Gear, //����
			size
		};

		// �X�e�[�g�Ǘ��p�萔
		enum eShopState {
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

		// ���݂̃X�e�[�g��ێ�
		eShopState m_currentState;
		// �I������Ă��鋭�����e��ێ�����
		eEnhancements m_currentEnhancements;
		// �e�������e�̃��x����ێ�
		vector<int> m_playerLv;
		// �e�������e�̔�p��ێ�
		vector<vector<int>> m_enhancementsCost;
		// �e�������e�������\����ێ�
		vector<bool>  m_canEnhancements;

		// ������p�̍ő包����ێ�
		int m_maxDigit;
		// �I���ꏊ�\���X�v���C�g�̓����x�̏���l
		float m_transparencyLimit;
		// �I�����X�v���C�g�̃X�P�[�����g�傷��ő�{��
		float m_spriteSizeRange;

		// �����X�v���C�g�̃X�P�[��
		Vec2 m_numberScale;
		// �����X�v���C�g���m�̊Ԋu
		Vec3 m_numberMargin;

		// �I���ꏊ�\���X�v���C�g�̕\�����W
		vector<Vec3> m_selectPointSpritePos;
		// ����Lv�X�v���C�g�̕\�����W
		vector<Vec3> m_playerLvSpritePos;
		// ������p�X�v���C�g�̕\�����W
		vector<Vec3> m_enhancementsCostSpritePos;
		// �����m�F�X�v���C�g�̕\�����W
		vector<Vec3> m_confirmSpritePos;
		// �����m�F�X�v���C�g�̏����X�P�[��
		Vec2 m_confirmSpritedefScale;
		// ������p�̕\�����W
		Vec3 m_nextCostSpritePos;
		// ���̃��x���̕\�����W
		Vec3 m_nextLevelSpritePos;


		// ������p�̐��l�X�v���C�g
		vector<vector<weak_ptr<Number>>> m_enhancementsCostNumSprite;
		// �e����Lv�p�̐��l�X�v���C�g
		vector<weak_ptr<Number>> m_playerLvNumSprite;
		// �w����ʗp�X�v���C�g
		weak_ptr<Sprite> m_purchaseScreenSprite;
		// �I���ꏊ�\���p�X�v���C�g
		weak_ptr<Sprite> m_selectPointSprite;
		// �I���ꏊ�����s�p�X�v���C�g
		vector<weak_ptr<Sprite>> m_notSelectableSprite;
		// �w�����e�m�F��ʗp�X�v���C�g
		weak_ptr<Sprite> m_confirmationScreenSprite;
		// �I�����ꂽ�������e�̎���Lv�p�̐��l�X�v���C�g
		weak_ptr<Number> m_enhancementsNextLvSprite;
		// �I�����ꂽ�������e�̋�����p�̐��l�X�v���C�g
		vector<weak_ptr<Number>> m_enhancementsNextCostSprite;
		// �������e�u�o�b�N�p�b�N�v�p�X�v���C�g
		weak_ptr<Sprite> m_backpackSprite;
		// �������e�u�X�e�[�^�X�v�p�X�v���C�g
		weak_ptr<Sprite> m_statusSprite;
		// �������e�u�M�A�v�p�X�v���C�g
		weak_ptr<Sprite> m_gearSprite;
		// �I�����u�͂��v�p�X�v���C�g
		weak_ptr<Sprite>m_yesSprite;
		// �I�����u�������v�p�X�v���C�g
		weak_ptr<Sprite>m_noSprite;

		// �V���b�v�p�̃X�e�[�g�}�V���̃|�C���^�����L
		unique_ptr<ShopStateMachine> m_ShopState;


	public:
		Shop(const shared_ptr<Stage>& stagePtr // �X�e�[�W�̃|�C���^
		) :
			TemplateObject(stagePtr)
		{
			// csv���狭����p���擾
			m_enhancementsCost = CSVLoader::ReadDataToInt(CSVLoader::LoadFile("ManagingMoney"));

			// �ϐ���������
			m_currentState = eShopState::Close;
			m_currentEnhancements = eEnhancements::Status;
			m_maxDigit = 0;
			m_transparencyLimit = 0.5f;
			m_spriteSizeRange = 1.25f;

			// �����X�v���C�g�̃X�P�[����ݒ�
			m_numberScale = Vec2(80.0f);

			// �����X�v���C�g���m�̊Ԋu��ݒ�
			m_numberMargin = Vec3(m_numberScale.x * 0.8f, 0.0f, 0.0f);

			// �T�C�Y���������e�̎�ޕ��ɐݒ�
			m_playerLv.resize(eEnhancements::size);
			m_canEnhancements.resize(eEnhancements::size);
			m_selectPointSpritePos.resize(eEnhancements::size);
			m_notSelectableSprite.resize(eEnhancements::size);
			m_enhancementsCostSpritePos.resize(eEnhancements::size);
			m_enhancementsCostNumSprite.resize(eEnhancements::size);
			m_playerLvNumSprite.resize(eEnhancements::size);

			// �T�C�Y���u�͂��v�u�������v��2�𕪗p��
			m_confirmSpritePos.resize(2);
		}

		// �f�X�g���N�^
		virtual ~Shop() {
			// �X�e�[�g�����Z�b�g
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
		@brief	 ������p�̍ő包�������߂�֐�
		*/
		int DemandDigit();

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
		eShopState GetState() const {
			return m_currentState;
		};
	};
}