/*!
@file CraftManager.h
@brief �N���t�g�Ǘ��p�}�l�[�W��
@author ���V���M
*/

#pragma once
#include "TemplateObject.h"
#include "CraftWindow.h"
#include "CraftItemIcon.h"
#include "CraftingIcon.h"
#include "CraftingQTE.h"

namespace basecross
{
	// �N���t�g�A�C�e��enum
	enum class eCraftItem : size_t
	{
		Rail, // ���[��
	};

	// �N���t�gCSV�p�����[�^
	enum class eCraftParam : size_t
	{
		ItemType,	 // ���A�C�e���^�C�v
		StoneValue,	 // �΂̗v����
		WoodValue,	 // �؂̗v����
		SuccesValue, // QTE�������̍쐬��
		FailedValue, // QTE���s���̍쐬��
	};

	// �A�C�e���J�E���^�penum
	enum class eItemType : size_t
	{
		Wood,	// ��
		Stone,	// ��
		Rail,	// ���[��
	};

	/*!
	@brief �N���t�g�Ǘ��N���X(�v���C���[�ƃN���t�g�֘A�N���X�̒���N���X)
	*/
	class CraftManager
	{
		const weak_ptr<CraftingQTE> m_craftQTE; // �N���t�gQTE
		const weak_ptr<CraftWindow> m_window;	// �N���t�g�E�B���h�E

		const vector<vector<string>> m_racipe; // �N���t�g���V�s

		vector<int> m_itemCount; // �A�C�e����

	public:

		/*!
		@brief �R���X�g���N�^
		@param �E�B���h�E�I�u�W�F�N�g�̃|�C���^
		@param �N���t�gQTE�I�u�W�F�N�g�̃|�C���^
		*/
		CraftManager(const shared_ptr<CraftWindow>& windowPtr,
			const shared_ptr<CraftingQTE>& qtePtr) :
			m_window(windowPtr),
			m_craftQTE(qtePtr),
			m_racipe(CSVLoader::LoadFile("CraftRacipe"))
		{
			m_itemCount = {10, 10, 0};
		}

		/*!
		@brief �f�X�g���N�^
		*/
		~CraftManager() {}

		/*!
		@brief �A�C�e���N���t�g�֐�
		@param �N���t�g����A�C�e��
		@return �N���t�g�\���̐^�U
		*/
		bool CraftOrder(eCraftItem item);

		/*!
		@brief �N���t�g�L�����֐�
		@param �L���ɂ��邩�̐^�U
		*/
		void CraftingEnabled(bool enable);

		/*!
		@brief QTE�J�n�֐�
		*/
		void StartQTE();

		/*!
		@brief QTE��~��QTE���ʎ擾�֐�
		@param �N���t�g�A�C�e��
		*/
		void StopQTE(eCraftItem item);

		/*!
		@brief �E�B���h�E�`�悪�����������̐^�U�擾�֐�
		@return �`�抮�����Ă���true�A����ȊO��false
		*/
		bool GetShowCraftWindow() const
		{
			return m_window.lock()->GetShowWindow();
		}

		/*!
		@brief �A�C�e�����擾�֐�
		@param �A�C�e���^�C�venum
		@return �A�C�e����
		*/
		int GetItemCount(eItemType type) const
		{
			return m_itemCount.at(static_cast<size_t>(type));
		}

		/*!
		@brief �A�C�e�����ǉ��֐�
		@param �A�C�e���^�C�venum
		@param �ǉ���(�f�t�H1)
		*/
		void AddItemCount(eItemType type, int addNum = 1)
		{
			m_itemCount.at(static_cast<size_t>(type)) += addNum;
		}

	private:

		/*!
		@brief �N���t�g�v���f�ސ��擾�֐�
		@param �N���t�g�A�C�e��
		@param �f�ރ^�C�v
		@return �f�ޗv����
		*/
		int GetRacipeValue(eCraftItem item, eCraftParam param)
		{
			return stoi(m_racipe.at(static_cast<size_t>(item)).at(static_cast<size_t>(param)));
		}

		/*!
		@brief �N���t�g�\���̐^�U�擾�֐�
		@param �؂̗v����
		@param �΂̗v����
		@return �\���̐^�U
		*/
		bool GetCraftPossible(int woodValue, int stoneValue) const
		{
			return woodValue <= GetItemCount(eItemType::Wood) && stoneValue <= GetItemCount(eItemType::Stone);
		}
	};
}