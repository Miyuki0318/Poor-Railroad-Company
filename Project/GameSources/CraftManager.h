/*!
@file CraftManager.h
@brief �N���t�g�Ǘ��p�}�l�[�W��
@author ���V���M
*/

#pragma once
#include "TemplateObject.h"
#include "CraftUI.h"
#include "CraftWindow.h"
#include "CraftItemIcon.h"
#include "CraftingQTE.h"
#include "CSVLoader.h"

namespace basecross
{
	// �N���t�g�A�C�e��enum
	enum class eCraftItem : size_t
	{
		Rail,		// ���[��
		WoodBridge,	// �؂̋�
		Crossing,	// ����
	};

	// �N���t�gCSV�p�����[�^
	enum class eCraftParam : size_t
	{
		ItemType,	 // ���A�C�e���^�C�v
		WoodValue,	 // �؂̗v����
		StoneValue,	 // �΂̗v����
		GearValue,	 // ���Ԃ̗v����
		SuccesValue, // QTE�������̍쐬��
		FailedValue, // QTE���s���̍쐬��
	};

	// �A�C�e���J�E���^�penum
	enum class eItemType : size_t
	{
		None,		// �����Ȃ�
		Wood,		// ��
		Stone,		// ��
		Gear,		// ����
		Rail,		// ���[��
		WoodBridge,	// �؂̋�
		Crossing,	// ����
		GoldBar,	// ����
		Money,		// ����
	};

	/*!
	@brief �N���t�g�Ǘ��N���X(�v���C���[�ƃN���t�g�֘A�N���X�̒���N���X)
	*/
	class CraftManager
	{
		const weak_ptr<TemplateObject> m_player;// �v���C���[�̃|�C���^
		const weak_ptr<CraftingQTE> m_craftQTE; // �N���t�gQTE
		const weak_ptr<CraftWindow> m_window;	// �N���t�g�E�B���h�E

		// �N���t�g�A�C�e���A�C�R��
		map<eCraftItem, weak_ptr<CraftItemIcon>> m_iconMap;
		map<eCraftItem, eInputButton> m_inputButton;

		const vector<vector<string>> m_racipe; // �N���t�g���V�s
		eCraftItem m_craftItem;	// �쐬���̃A�C�e��
		eItemType m_craftType;	// �쐬���̃A�C�e��

	public:

		map<eItemType, unsigned char> m_itemCount; // �A�C�e����

		/*!
		@brief �R���X�g���N�^
		@param �E�B���h�E�I�u�W�F�N�g�̃|�C���^
		@param �N���t�gQTE�I�u�W�F�N�g�̃|�C���^
		*/
		CraftManager(const shared_ptr<TemplateObject>& playerPtr,
			const shared_ptr<CraftWindow>& windowPtr,
			const shared_ptr<CraftingQTE>& qtePtr) :
			m_player(playerPtr),
			m_window(windowPtr),
			m_craftQTE(qtePtr),
			m_racipe(CSVLoader::LoadFile("CraftRacipe"))
		{
			m_craftItem = eCraftItem::Rail;
			m_craftType = eItemType::Rail;

			// �A�C�e���J�E���^
			m_itemCount.emplace(eItemType::Wood, 0);
			m_itemCount.emplace(eItemType::Stone, 0);
			m_itemCount.emplace(eItemType::Gear, 0);
			m_itemCount.emplace(eItemType::Rail, 0);
			m_itemCount.emplace(eItemType::WoodBridge, 0);
			m_itemCount.emplace(eItemType::Crossing, 0);
			m_itemCount.emplace(eItemType::GoldBar, 0);

			m_inputButton.emplace(eCraftItem::Rail, eInputButton::ButtonB);
			m_inputButton.emplace(eCraftItem::WoodBridge, eInputButton::ButtonA);
			m_inputButton.emplace(eCraftItem::Crossing, eInputButton::ButtonY);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		~CraftManager() {}

		/*!
		@brief �������̏����֐�
		*/
		void OnCreate();

		/*!
		@brief ���Z�b�g�����֐�
		*/
		void ResetCraftManager();

		/*!
		@brief �A�C�e���N���t�g�֐�
		@param �N���t�g����A�C�e��
		@return �N���t�g�\���̐^�U
		*/
		bool CraftOrder(eCraftItem item) const;

		/*!
		@brief �N���t�g�L�����֐�
		@param �L���ɂ��邩�̐^�U
		*/
		void CraftingEnabled(bool enable);

		/*!
		@brief QTE�J�n�֐�
		*/
		void StartQTE(eCraftItem item, eItemType type);

		/*!
		@brief QTE��~��QTE���ʎ擾�֐�
		@param �N���t�g�A�C�e��
		*/
		bool StopQTE();

		/*!
		@brief QTE������~�֐�
		*/
		void DestroyCraftQTE();

		/*!
		@brief �E�B���h�E�`�悪�����������̐^�U�擾�֐�
		@return �`�抮�����Ă���true�A����ȊO��false
		*/
		bool GetShowCraftWindow() const
		{
			return m_window.lock()->GetShowWindow();
		}

		/*!
		@brief QTE���I���������̐^�U�擾�֐�
		@return �I�����Ă���true�A����ȊO��false
		*/
		bool GetEndedQTE() const
		{
			return !m_craftQTE.lock()->GetEnableQTE();
		}

		/*!
		@brief �A�C�e�����擾�֐�
		@param �A�C�e���^�C�venum
		@return �A�C�e����
		*/
		int GetItemCount(eItemType type) const
		{
			if (m_itemCount.find(type) == m_itemCount.end()) return 0;

			return m_itemCount.at(type);
		}

		/*!
		@brief �A�C�e�����ǉ��֐�
		@param �A�C�e���^�C�venum
		@param �ǉ���(�f�t�H1)
		*/
		void AddItemCount(eItemType type, int addNum = 1)
		{
			if (m_itemCount.find(type) == m_itemCount.end()) return;

			m_itemCount.at(type) += addNum;
		}

		/*!
		@brief �A�C�e�����ǉ��֐�
		@param �A�C�e���^�C�venum
		@param �ǉ���(�f�t�H1)
		*/
		void UseItem(eItemType type, int useNum = 1)
		{
			if (m_itemCount.find(type) == m_itemCount.end()) return;

			m_itemCount.at(type) -= useNum;
		}

		/*!
		@brief �N���t�g�v���f�ސ��擾�֐�
		@param �N���t�g�A�C�e��
		@param �f�ރ^�C�v
		@return �f�ޗv����
		*/
		int GetRacipeValue(eCraftItem item, eCraftParam param) const
		{
			return stoi(m_racipe.at(static_cast<size_t>(item)).at(static_cast<size_t>(param)));
		}

		/*!
		@brief �N���t�g�\���̐^�U�擾�֐�
		@param �؂̗v����
		@param �΂̗v����
		@param ���Ԃ̗v����
		@return �\���̐^�U
		*/
		bool GetCraftPossible(int woodValue, int stoneValue, int gearValue) const
		{
			return woodValue <= GetItemCount(eItemType::Wood)	// �؂�v�����ȏ㏊�����Ă��邩
				&& stoneValue <= GetItemCount(eItemType::Stone) // �΂�v�����ȏ㏊�����Ă��邩
				&& gearValue <= GetItemCount(eItemType::Gear);	// ���Ԃ�v�����ȏ㏊�����Ă��邩
		}

		/*!
		@brief �N���t�g���Ă���A�C�e���^�C�v��Ԃ��֐�
		@return m_craftType
		*/
		eItemType GetCraftingItemType() const
		{
			return m_craftType;
		}
	};
}