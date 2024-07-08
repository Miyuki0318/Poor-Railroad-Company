/*!
@file Instruction.h
@brief ������@
@prod ��I�t
*/

#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "OzawaUtility.h"

namespace basecross
{
	class Instruction : public GameObject
	{
		enum eControllerType
		{
			Keyboard,	// �L�[�{�[�h/�}�E�X����
			XBoxPad		// XBox�p�b�h����
		};
		enum eInstructionType
		{
			Normal,		// �ʏ�
			Craft,		// �N���t�g
			TypeNum
		};
		eControllerType m_currentContType;	// ���݂̃R���g���[���[�^�C�v
		eControllerType m_pastContType;		// �O�t���[���̃R���g���[���[�^�C�v
		eInstructionType m_currentInstType;	// ���݂̑���^�C�v
		eInstructionType m_pastInstType;	// �O�t���[���̑���^�C�v

		const Vec2 m_DefaultScale;	// �����T�C�Y
		const Vec3 m_DefaultPos;	// �����ʒu

		map<eInstructionType, weak_ptr<Sprite>> m_keyboardInstructions; // �L�[�{�[�h/�}�E�X������@�摜
		map<eInstructionType, weak_ptr<Sprite>> m_xBoxInstructions;		// XBox�p�b�h������@�摜

		/// <summary>
		/// ������@�̕\����\��(���ׂ�)
		/// </summary>
		/// <param name="flag">�\��/��\��</param>
		void SetDrawActiveInstructions(bool flag);

		/// <summary>
		/// ������@�̕\����\��
		/// </summary>
		/// <param name="flag">�\��/��\��</param>
		/// <param name="cType">���݂̃R���g���[���[�^�C�v</param>
		/// <param name="iType">���݂̑���^�C�v</param>
		void SetDrawActiveInstructions(bool flag, eControllerType cType, eInstructionType iType);

		/// <summary>
		/// �R���g���[���[�^�C�v�̌��菈��
		/// </summary>
		void ControllerTypeDecision();

		/// <summary>
		/// ����^�C�v�̌��菈��
		/// </summary>
		void InstructionTypeDecision();

	public:
		Instruction(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_DefaultScale(Vec2(WINDOW_WIDTH, WINDOW_HEIGHT / 10.0f)),
			m_DefaultPos(Vec3(0.0f, (-WINDOW_HEIGHT + m_DefaultScale.y) / 2, 0.0f)),
			m_currentContType(eControllerType::Keyboard),
			m_pastContType(eControllerType::Keyboard),
			m_currentInstType(eInstructionType::Normal),
			m_pastInstType(eInstructionType::Normal)
		{
		}
		~Instruction() {}

		void OnCreate() override;
		void OnUpdate() override;
	};
}