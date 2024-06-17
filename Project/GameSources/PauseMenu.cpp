#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void PauseMenu::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage();

		m_menuSprites = stagePtr->AddGameObject<Sprite>(L"PAUSEMENU_TX", m_DefaultScale, m_DefaultPosition);
	}

	void PauseMenu::OnUpdate()
	{

	}
}