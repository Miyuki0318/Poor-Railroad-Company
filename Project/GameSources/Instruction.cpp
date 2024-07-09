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
		m_instructions[Keyboard][Normal] = stagePtr->AddGameObject<Sprite>(L"INST_NORMAL_K", m_DefaultScale, m_DefaultPos);
		m_instructions[Keyboard][Craft] = stagePtr->AddGameObject<Sprite>(L"INST_CRAFT_K", m_DefaultScale, m_DefaultPos);
		m_instructions[XBoxPad][Normal] = stagePtr->AddGameObject<Sprite>(L"INST_NORMAL_X", m_DefaultScale, m_DefaultPos);
		m_instructions[XBoxPad][Craft] = stagePtr->AddGameObject<Sprite>(L"INST_CRAFT_X", m_DefaultScale, m_DefaultPos);

		// ���C���[�̐ݒ�
		for (int i = 0; i < ContTypeNum; i++)
		{
			for (int j = 0; j < InstTypeNum; j++)
			{
				m_instructions.at((eControllerType)i).at((eInstructionType)j).lock()->SetDrawLayer(m_DrawLayerNum);
			}
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
		// ���ׂẴX�v���C�g�`���؂�ւ���
		for (int i = 0; i < ContTypeNum; i++)
		{
			for (int j = 0; j < InstTypeNum; j++)
			{
				m_instructions.at((eControllerType)i).at((eInstructionType)j).lock()->SetDrawActive(flag);
			}
		}
	}

	void Instruction::SetDrawActiveInstructions(bool flag, eControllerType cType, eInstructionType iType)
	{
		// �w�肳�ꂽ�^�C�v�̃X�v���C�g�̕`���؂�ւ���
		m_instructions.at(cType).at(iType).lock()->SetDrawActive(flag);
	}

	void Instruction::ControllerTypeDecision()
	{
		// �p�b�h���q����Ă�����
		if (Input::GetPadConected())
		{
			m_currentContType = XBoxPad; // �p�b�h���
		}
		else // �����łȂ����
		{
			m_currentContType = Keyboard; // �L�[�{�[�h/�}�E�X���
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
			m_currentInstType = Craft; // �N���t�g���
		}
		else // �����łȂ����
		{
			m_currentInstType = Normal; // �ʏ���
		}
		// �O�t���[���ƌ��t���[���̏�Ԃ��Ⴄ�Ȃ�\�����Ă����摜���\���ɂ���
		if (m_pastInstType != m_currentInstType) SetDrawActiveInstructions(false, m_currentContType, m_pastInstType);
	}
}