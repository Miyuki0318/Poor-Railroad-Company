#pragma once
#include "CraftWindow.h"
#include "CraftItemIcon.h"
#include "CraftingIcon.h"
#include "CraftingQTE.h"

namespace basecross
{
	class CraftManager
	{
		weak_ptr<CraftWindow> m_window;	// �N���t�g�E�B���h�E

	public:

		CraftManager(const shared_ptr<CraftWindow>& windowPtr) :
			m_window(windowPtr)
		{

		}

		~CraftManager() {}

		void CraftingEnabled(bool b) {}
	};
}