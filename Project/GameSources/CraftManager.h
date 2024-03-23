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
	enum class eCraftItem
	{
		Rail, // ���[��
	};

	// �N���t�gCSV�p�����[�^
	enum class eCraftParam
	{
		ItemType,	// ���A�C�e���^�C�v
		StoneValue,	// �΂̗v����
		WoodValue,	// �؂̗v����
	};

	/*!
	@brief �N���t�g�Ǘ��N���X
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

		~CraftManager() {}

		/*!
		@brief �A�C�e���N���t�g�֐�
		*/
		void Crafting(const shared_ptr<TemplateObject>& tempPtr);

		/*!
		@brief �N���t�g�L�����֐�
		*/
		void CraftingEnabled(bool enable);



	private:

		/*!
		@brief �N���t�g�v���f�ސ��擾�֐�
		@param �N���t�g�A�C�e��
		@param �f�ރ^�C�v
		*/
		int GetRacipeValue(eCraftItem item, eCraftParam param)
		{
			return stoi(m_racipe.at(static_cast<int>(item)).at(static_cast<int>(param)));
		}

	};
}