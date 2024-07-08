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
		eControllerType m_currentContType;	// 現在のコントローラータイプ
		eControllerType m_pastContType;		// 前フレームのコントローラータイプ
		eInstructionType m_currentInstType;	// 現在の操作タイプ
		eInstructionType m_pastInstType;	// 前フレームの操作タイプ

		const Vec2 m_DefaultScale;	// 初期サイズ
		const Vec3 m_DefaultPos;	// 初期位置
		const int m_DrawLayer;
		

		map<eInstructionType, weak_ptr<Sprite>> m_keyboardInstructions; // キーボード/マウス操作方法画像
		map<eInstructionType, weak_ptr<Sprite>> m_xBoxInstructions;		// XBoxパッド操作方法画像

		/// <summary>
		/// 操作方法の表示非表示(すべて)
		/// </summary>
		/// <param name="flag">表示/非表示</param>
		void SetDrawActiveInstructions(bool flag);

		/// <summary>
		/// 操作方法の表示非表示
		/// </summary>
		/// <param name="flag">表示/非表示</param>
		/// <param name="cType">現在のコントローラータイプ</param>
		/// <param name="iType">現在の操作タイプ</param>
		void SetDrawActiveInstructions(bool flag, eControllerType cType, eInstructionType iType);

		/// <summary>
		/// コントローラータイプの決定処理
		/// </summary>
		void ControllerTypeDecision();

		/// <summary>
		/// 操作タイプの決定処理
		/// </summary>
		void InstructionTypeDecision();

	public:
		Instruction(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_DefaultScale(Vec2(WINDOW_WIDTH, WINDOW_HEIGHT / 10.0f)),
			m_DefaultPos(Vec3(0.0f, (-WINDOW_HEIGHT + m_DefaultScale.y) / 2, 0.0f)),
			m_DrawLayer(7),
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