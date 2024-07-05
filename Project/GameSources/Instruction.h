/*!
@file Instruction.h
@brief 操作方法
@prod 矢吹悠葉
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
			Keyboard,	// キーボード/マウス操作
			XBoxPad		// XBoxパッド操作
		};
		enum eInstructionType
		{
			Normal,		// 通常
			Craft,		// クラフト
			TypeNum
		};
		eControllerType m_currentContType;
		eControllerType m_pastContType;
		eInstructionType m_currentInstType;
		eInstructionType m_pastInstType;

		const Vec2 m_DefaultScale;	// 初期サイズ
		const Vec3 m_DefaultPos;	// 初期位置

		map<eInstructionType, weak_ptr<Sprite>> m_keyboardInstructions; // キーボード/マウス操作方法画像
		map<eInstructionType, weak_ptr<Sprite>> m_xBoxInstructions;		// XBoxパッド操作方法画像

		void SetDrawActiveInstructions(bool flag);

		void SetDrawActiveInstructions(bool flag, eControllerType cType, eInstructionType iType);

	public:
		Instruction(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_DefaultScale(Vec2(1920.0f, 216.0f)),
			m_DefaultPos(Vec3(0.0f, (-WINDOW_HEIGHT + m_DefaultScale.y) / 2, 0.0f)),
			m_currentContType(eControllerType::Keyboard),
			m_currentInstType(eInstructionType::Normal)
		{
		}
		~Instruction() {}

		void OnCreate() override;
		void OnUpdate() override;

	};
}