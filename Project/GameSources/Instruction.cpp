/*!
@file Instruction.cpp
@brief ‘€ì•û–@
@prod –î—I—t
*/

#include "stdafx.h"
#include "Instruction.h"

namespace basecross
{
	void Instruction::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage();
		
		m_keyboardInstructions.emplace(eInstructionType::Normal, stagePtr->AddGameObject<Sprite>(L"INST_NORMAL_K", m_DefaultScale, m_DefaultPos));
		//m_keyboardInstructions.emplace(eInstructionType::Craft, stagePtr->AddGameObject<Sprite>(L"INST_CRAFT_K", m_DefaultScale, m_DefaultPos));
		//m_keyboardInstructions.at(eInstructionType::Normal).lock()->SetDrawLayer(10);
		//m_keyboardInstructions.at(eInstructionType::Craft).lock()->SetDrawLayer(10);

		//m_xBoxInstructions.emplace(eInstructionType::Normal, stagePtr->AddGameObject<Sprite>(L"INST_NORMAL_X", m_DefaultScale, m_DefaultPos));
		//m_xBoxInstructions.emplace(eInstructionType::Craft, stagePtr->AddGameObject<Sprite>(L"INST_CRAFT_X", m_DefaultScale, m_DefaultPos));
		//m_xBoxInstructions.at(eInstructionType::Normal).lock()->SetDrawLayer(10);
		//m_xBoxInstructions.at(eInstructionType::Craft).lock()->SetDrawLayer(10);

		//SetDrawActiveInstructions(false);
	}

	void Instruction::OnUpdate()
	{

	}

	void Instruction::SetDrawActiveInstructions(bool flag)
	{
		for (int i = 0; i < eInstructionType::TypeNum; i++)
		{
			m_keyboardInstructions.at((eInstructionType)i).lock()->SetDrawActive(flag);
			m_xBoxInstructions.at((eInstructionType)i).lock()->SetDrawActive(flag);
		}
	}
}