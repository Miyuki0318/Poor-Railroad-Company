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
		// �X�e�[�g�Ǘ��p�萔
		enum eShopState {
			Close, // �V���b�v���I��
			Shopping, // �w�����e�I��
			Confirmation, // �w�����e�m�F��
		};

	private:
		// �������e�萔
		enum eEnhancements {
			LimitChoices, // ���ޏ������
			Status, // �̌@/�ړ����x����
			Gear, //����
			EnhancementsSize,
		};

		// �m�F�p�萔
		enum eConfirmState {
			Yes, // �͂�
			No, // ������
			ConfirmSize,
		};

		// �����ޗp�萔
		enum eOperationType {
			Controller, // �R���g���[���[�p
			Keyboard, // �L�[�{�[�h�p
			OperationSize,
		};

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
		const float m_transparencyLimit;
		// �I�����X�v���C�g�̃X�P�[�����g�傷��ő�{��
		const float m_spriteSizeRange;

		// �����X�v���C�g�̃X�P�[��
		const Vec2 m_numberScale;
		// �����X�v���C�g���m�̊Ԋu
		const Vec3 m_numberMargin;

		// �I���ꏊ�\���X�v���C�g�̕\�����W
		const vector<Vec3> m_selectPointSpritePos;
		// ����Lv�X�v���C�g�̕\�����W
		const vector<Vec3> m_playerLvSpritePos;
		// �����m�F�X�v���C�g�̕\�����W
		const vector<Vec3> m_confirmSpritePos;
		// �����m�F�X�v���C�g�̏����X�P�[��
		const Vec2 m_confirmSpriteScale;
		// �������e�X�v���C�g�̃X�P�[��
		const Vec2 m_enhancementsContentsScale;
		// �������e�X�v���C�g�̕\�����W
		const Vec3 m_enhancementsContentsPos;
		// �I�����X�v���C�g�̃X�P�[��
		const Vec2 m_selectPointScale;
		// �X�N���[���X�v���C�g�̕\�����W
		const Vec3 m_screenSpritePos;
		// ���̃��x���̕\�����W
		const Vec3 m_nextLevelSpritePos;
		// �A�C�R���̕\�����W
		const Vec3 m_iconPos;
		// �A�C�R���̃X�P�[��
		const Vec2 m_iconScale;
		// ������p�X�v���C�g�̕\�����W
		vector<Vec3> m_enhancementsCostSpritePos;
		// ���̋�����p�̕\�����W
		Vec3 m_nextCostSpritePos;

		// ������p�̐��l�X�v���C�g
		vector<vector<weak_ptr<Number>>> m_enhancementsCostNumSprite;
		// �e����Lv�p�̐��l�X�v���C�g
		vector<weak_ptr<Number>> m_playerLvNumSprite;
		// �w����ʗp�X�v���C�g
		weak_ptr<Sprite> m_purchaseScreenSprite;
		// �I���ꏊ�\���p�X�v���C�g
		weak_ptr<Sprite> m_selectPointSprite;
		// �I���ꏊ�����s�p�X�v���C�g(�I���ӏ����O���[�̃X�v���C�g�ŕ���)
		vector<weak_ptr<Sprite>> m_notSelectableSprite;
		// �}�E�X�E�R���g���[���[�ؑ֎��̕\���A�C�R���p�X�v���C�g
		vector<weak_ptr<Sprite>> m_iconSprite;
		// �w�����e�m�F��ʗp�X�v���C�g
		weak_ptr<Sprite> m_confirmationScreenSprite;
		// �I�����ꂽ�������e�̎���Lv�p�̐��l�X�v���C�g
		weak_ptr<Number> m_enhancementsNextLvSprite;
		// �I�����ꂽ�������e�̋�����p�̐��l�X�v���C�g
		vector<weak_ptr<Number>> m_enhancementsNextCostSprite;
		// �������e�u���ޏ�������㏸�v�p�X�v���C�g
		weak_ptr<Sprite> m_LimitChoicesSprite;
		// �������e�u�X�e�[�^�X�v�p�X�v���C�g
		weak_ptr<Sprite> m_statusSprite;
		// �������e�u���ؑf�ޏ����������v�p�X�v���C�g
		weak_ptr<Sprite> m_gearSprite;
		// �I�����p�X�v���C�g
		map<eConfirmState, weak_ptr<Sprite>> m_confirmSprite;

		// �V���b�v�p�̃X�e�[�g�}�V���̃|�C���^�����L
		unique_ptr<ShopStateMachine> m_ShopState;

	public:
		Shop(const shared_ptr<Stage>& stagePtr // �X�e�[�W�̃|�C���^
		) :
			// �p������TemplateObject�ɃX�e�[�W�̃|�C���^��n��
			TemplateObject(stagePtr),
			// �X�v���C�g�̓����x�̏���l��ݒ�
			m_transparencyLimit(0.5f),
			// �X�v���C�g���g�傷��ő�{����ݒ�
			m_spriteSizeRange(1.25f),
			// �����X�v���C�g�̃X�P�[����ݒ�
			m_numberScale(Vec2(80.0f)),
			// �����X�v���C�g���m�̊Ԋu��ݒ�
			m_numberMargin(Vec3(m_numberScale.x * 0.8f, 0.0f, 0.0f)),
			// �I�����̕W���̃X�P�[��
			m_confirmSpriteScale(Vec2(360.0f, 90.0f)),
			// �������e�̃X�P�[��
			m_enhancementsContentsScale(Vec2(875.0f, 125.0f)),
			// �I�����X�v���C�g�̃X�P�[��
			m_selectPointScale(Vec2(1170.0f, 238.0)),
			// �X�N���[���X�v���C�g�̕\�����W��ݒ�
			m_screenSpritePos(Vec3(-400.0f, 0.0f, 0.0f)),
			// ������p�̕\�����W��ݒ�
			m_nextCostSpritePos(Vec3(120.0f, -90.0f, 0.0f)),
			// ���̃��x���̕\�����W��ݒ�
			m_nextLevelSpritePos(Vec3(-350.0f, -90.0f, 0.0f)),
			// �A�C�R���̕\�����W�̐ݒ�
			m_iconPos(Vec3(100.0f, 380.0f, 0.0f)),
			// �A�C�R���̃X�P�[���̐ݒ�
			m_iconScale(Vec2(100.0f, 100.0f)),
			// �������e�̕W�����W��ݒ�
			m_enhancementsContentsPos(Vec3(-170.0f, 100.0f, 0.0f)),
			// �I�����̕\���ʒu��ݒ�
			m_selectPointSpritePos{
				Vec3(-179.0f, 181.0f, 0.0f),
				Vec3(-179.0f, -66.0f, 0.0f),
				Vec3(-179.0f, -320.0f, 0.0f)
			},
			// ����Lv�̕\���ʒu��ݒ�
			m_playerLvSpritePos{
				Vec3(-370.0f, 117.0f, 0.0f),
				Vec3(-370.0f, -128.0f, 0.0f),
				Vec3(-370.0f, -383.0f, 0.0f)
			},
			// �I�����X�v���C�g�̕\���ʒu��ݒ�
			m_confirmSpritePos{
				Vec3(-400.0f, -300.0f, 0.0f), // �I�����u�͂��v
				Vec3(100.0f, -300.0f, 0.0f) // �I�����u�������v
			},
			// ������p�̕\���ʒu��ݒ�
			m_enhancementsCostSpritePos{
				Vec3(100.0f, 117.0f, 0.0f),
				Vec3(100.0f, -128.0f, 0.0f),
				Vec3(100.0f, -383.0f, 0.0f)
			}


		{
			// csv���狭����p���擾
			m_enhancementsCost = CSVLoader::ReadDataToInt(CSVLoader::LoadFile("ManagingMoney"));

			// �ϐ���������
			m_currentState = eShopState::Close;
			m_currentEnhancements = eEnhancements::Status;
			m_maxDigit = 0;

			// �z��̃T�C�Y��ݒ�
			m_playerLv.resize(eEnhancements::EnhancementsSize);
			m_canEnhancements.resize(eEnhancements::EnhancementsSize);
			m_notSelectableSprite.resize(eEnhancements::EnhancementsSize);
			m_enhancementsCostNumSprite.resize(eEnhancements::EnhancementsSize);
			m_playerLvNumSprite.resize(eEnhancements::EnhancementsSize);

			m_iconSprite.resize(eOperationType::OperationSize);
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