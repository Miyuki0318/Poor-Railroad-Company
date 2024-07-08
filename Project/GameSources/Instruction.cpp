/*!
@file Instruction.cpp
@brief ������@
@prod ��I�t
*/

#include "stdafx.h"
#include "Instruction.h"
#include "Input.h"
#include "GamePlayer.h"

namespace basecross
{
	void Instruction::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage();
		
		// �X�v���C�g�̍쐬
		m_keyboardInstructions.emplace(eInstructionType::Normal, stagePtr->AddGameObject<Sprite>(L"INST_NORMAL_K", m_DefaultScale, m_DefaultPos));
		m_keyboardInstructions.emplace(eInstructionType::Craft, stagePtr->AddGameObject<Sprite>(L"INST_CRAFT_K", m_DefaultScale, m_DefaultPos));
		m_xBoxInstructions.emplace(eInstructionType::Normal, stagePtr->AddGameObject<Sprite>(L"INST_NORMAL_X", m_DefaultScale, m_DefaultPos));
		m_xBoxInstructions.emplace(eInstructionType::Craft, stagePtr->AddGameObject<Sprite>(L"INST_CRAFT_X", m_DefaultScale, m_DefaultPos));

		// ���C���[�̐ݒ�
		for (int i = 0; i < eInstructionType::TypeNum; i++)
		{
			m_keyboardInstructions.at((eInstructionType)i).lock()->SetDrawLayer(m_DrawLayer);
			m_xBoxInstructions.at((eInstructionType)i).lock()->SetDrawLayer(m_DrawLayer);
		}

		SetDrawActiveInstructions(false); // ���ׂĔ�\����
	}

	void Instruction::OnUpdate()
	{
		// ���݂̏�Ԃ����肷��
		ControllerTypeDecision();
		InstructionTypeDecision();

		// ���݂̏�ԂɃ}�b�`�����摜��\������
		SetDrawActiveInstructions(true, m_currentContType, m_currentInstType);
		
		if (GetTypeStage<GameStage>()->GetIsStaging()) // ���o���Ȃ�
		{
			SetDrawActiveInstructions(false); // ��\���ɂ���
		}

		// ���t���[���̏�Ԃ�ۑ�����
		m_pastContType = m_currentContType;
		m_pastInstType = m_currentInstType;
	}

	void Instruction::SetDrawActiveInstructions(bool flag)
	{
		for (int i = 0; i < eInstructionType::TypeNum; i++)
		{
			m_xBoxInstructions.at((eInstructionType)i).lock()->SetDrawActive(flag);
			m_keyboardInstructions.at((eInstructionType)i).lock()->SetDrawActive(flag);
		}
	}

	void Instruction::SetDrawActiveInstructions(bool flag, eControllerType cType, eInstructionType iType)
	{
		if (cType == Keyboard) // �R���g���[���[�^�C�v���L�[�{�[�h/�}�E�X�Ȃ�
		{
			m_keyboardInstructions.at(iType).lock()->SetDrawActive(flag);
			return;
		}
		if (cType == XBoxPad) // �R���g���[���[�^�C�v���p�b�h�Ȃ�
		{
			m_xBoxInstructions.at(iType).lock()->SetDrawActive(flag);
		}
	}

	void Instruction::ControllerTypeDecision()
	{
		// �p�b�h���q����Ă�����
		if (Input::GetPadConected())
		{
			m_currentContType = eControllerType::XBoxPad; // �p�b�h���
		}
		else // �����łȂ����
		{
			m_currentContType = eControllerType::Keyboard; // �L�[�{�[�h/�}�E�X���
		}
		// �O�t���[���ƌ��t���[���̏�Ԃ��Ⴄ�Ȃ�\�����Ă����摜���\���ɂ���
		if (m_pastContType != m_currentContType) SetDrawActiveInstructions(false, m_pastContType, m_currentInstType);
	}

	void Instruction::InstructionTypeDecision()
	{
		// �v���C���[���擾
		auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");

		// �v���C���[�̃X�e�[�^�X���N���t�g���܂���QTE���Ȃ�
		if (player->GetStatus(ePlayerStatus::IsCrafting) || player->GetStatus(ePlayerStatus::IsCraftQTE))
		{
			m_currentInstType = eInstructionType::Craft; // �N���t�g���
		}
		else // �����łȂ����
		{
			m_currentInstType = eInstructionType::Normal; // �ʏ���
		}
		// �O�t���[���ƌ��t���[���̏�Ԃ��Ⴄ�Ȃ�\�����Ă����摜���\���ɂ���
		if (m_pastInstType != m_currentInstType) SetDrawActiveInstructions(false, m_currentContType, m_pastInstType);
	}
}