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
		ItemType,	// ���A�C�e���^�C�v
		StoneValue,	// �΂̗v����
		WoodValue,	// �؂̗v����
	};

	/*!
	@brief �N���t�g�Ǘ��N���X(�v���C���[�ƃN���t�g�֘A�N���X�̒���N���X)
	*/
	class CraftManager
	{
		weak_ptr<CraftWindow> m_window;	// �N���t�g�E�B���h�E

		const vector<vector<string>> m_racipe; // �N���t�g���V�s

	public:

		/*!
		@brief �R���X�g���N�^
		@param �E�B���h�E�I�u�W�F�N�g�̃|�C���^
		*/
		CraftManager(const shared_ptr<CraftWindow>& windowPtr) :
			m_window(windowPtr),
			m_racipe(CSVLoader::LoadFile("CraftRacipe"))
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		~CraftManager() {}

		/*!
		@brief �A�C�e���N���t�g�֐�
		@param �v���C���[�̃|�C���^
		*/
		void Crafting(const shared_ptr<TemplateObject>& tempPtr);

		/*!
		@brief �N���t�g�L�����֐�
		@param �L���ɂ��邩�̐^�U
		*/
		void CraftingEnabled(bool enable);

		/*!
		@brief �E�B���h�E�`�悪�����������̐^�U�擾�֐�
		@return �`�抮�����Ă���true�A����ȊO��false
		*/
		bool GetShowCraftWindow() const
		{
			return m_window.lock()->GetShowWindow();
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
	};
}