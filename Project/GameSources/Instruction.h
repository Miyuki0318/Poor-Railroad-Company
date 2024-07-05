/*!
@file Instruction.h
@brief 操作方法
@prod 矢吹悠葉
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
			Keyboard,	// キーボード/マウス操作
			XBoxPad		// XBoxパッド操作
		};
		enum eInstructionType
		{
			Normal,		// 通常
			Craft,		// クラフト
			TypeNum
		};

		const Vec2 m_DefaultScale;	// 初期サイズ
		const Vec3 m_DefaultPos;	// 初期位置

		map<eInstructionType, weak_ptr<Sprite>> m_keyboardInstructions; // キーボード/マウス操作方法画像
		map<eInstructionType, weak_ptr<Sprite>> m_xBoxInstructions;		// XBoxパッド操作方法画像

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