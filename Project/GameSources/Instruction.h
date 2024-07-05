/*!
@file Instruction.h
@brief ������@
@prod ��I�t
*/

#pragma once
#include "stdafx.h"
#include "Sprite.h"

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

		const Vec2 m_DefaultScale;	// �����T�C�Y
		const Vec3 m_DefaultPos;	// �����ʒu

		map<eInstructionType, weak_ptr<Sprite>> m_keyboardInstructions; // �L�[�{�[�h/�}�E�X������@�摜
		map<eInstructionType, weak_ptr<Sprite>> m_xBoxInstructions;		// XBox�p�b�h������@�摜

		void SetDrawActiveInstructions(bool flag);
	public:
		Instruction(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_DefaultScale(Vec2(1920.0f, 216.0f)),
			m_DefaultPos(Vec3(0.0f, (-WINDOW_HEIGHT + m_DefaultScale.y) / 2, 0.0f))
		{
		}
		~Instruction() {}

		void OnCreate() override;
		void OnUpdate() override;

	};
}