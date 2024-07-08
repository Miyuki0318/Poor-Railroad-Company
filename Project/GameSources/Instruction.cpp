/*!
@file Instruction.cpp
@brief 操作方法
@prod 矢吹悠葉
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
		
		// スプライトの作成
		m_keyboardInstructions.emplace(eInstructionType::Normal, stagePtr->AddGameObject<Sprite>(L"INST_NORMAL_K", m_DefaultScale, m_DefaultPos));
		m_keyboardInstructions.emplace(eInstructionType::Craft, stagePtr->AddGameObject<Sprite>(L"INST_CRAFT_K", m_DefaultScale, m_DefaultPos));
		m_xBoxInstructions.emplace(eInstructionType::Normal, stagePtr->AddGameObject<Sprite>(L"INST_NORMAL_X", m_DefaultScale, m_DefaultPos));
		m_xBoxInstructions.emplace(eInstructionType::Craft, stagePtr->AddGameObject<Sprite>(L"INST_CRAFT_X", m_DefaultScale, m_DefaultPos));

		// レイヤーの設定
		for (int i = 0; i < eInstructionType::TypeNum; i++)
		{
			m_keyboardInstructions.at((eInstructionType)i).lock()->SetDrawLayer(m_DrawLayerNum);
			m_xBoxInstructions.at((eInstructionType)i).lock()->SetDrawLayer(m_DrawLayerNum);
		}

		SetDrawActiveInstructions(false); // すべて非表示に
	}

	void Instruction::OnUpdate()
	{
		// 現在の状態を決定する
		ControllerTypeDecision();
		InstructionTypeDecision();

		// 現在の状態にマッチした画像を表示する
		SetDrawActiveInstructions(true, m_currentContType, m_currentInstType);
		
		if (GetTypeStage<GameStage>()->GetIsStaging()) // 演出中なら
		{
			SetDrawActiveInstructions(false); // 非表示にする
		}

		// 現フレームの状態を保存する
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
		if (cType == Keyboard) // コントローラータイプがキーボード/マウスなら
		{
			m_keyboardInstructions.at(iType).lock()->SetDrawActive(flag);
			return;
		}
		if (cType == XBoxPad) // コントローラータイプがパッドなら
		{
			m_xBoxInstructions.at(iType).lock()->SetDrawActive(flag);
		}
	}

	void Instruction::ControllerTypeDecision()
	{
		// パッドが繋がれていたら
		if (Input::GetPadConected())
		{
			m_currentContType = eControllerType::XBoxPad; // パッド状態
		}
		else // そうでなければ
		{
			m_currentContType = eControllerType::Keyboard; // キーボード/マウス状態
		}
		// 前フレームと現フレームの状態が違うなら表示していた画像を非表示にする
		if (m_pastContType != m_currentContType) SetDrawActiveInstructions(false, m_pastContType, m_currentInstType);
	}

	void Instruction::InstructionTypeDecision()
	{
		// プレイヤーを取得
		auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");

		// プレイヤーのステータスがクラフト中またはQTE中なら
		if (player->GetStatus(ePlayerStatus::IsCrafting) || player->GetStatus(ePlayerStatus::IsCraftQTE))
		{
			m_currentInstType = eInstructionType::Craft; // クラフト状態
		}
		else // そうでなければ
		{
			m_currentInstType = eInstructionType::Normal; // 通常状態
		}
		// 前フレームと現フレームの状態が違うなら表示していた画像を非表示にする
		if (m_pastInstType != m_currentInstType) SetDrawActiveInstructions(false, m_currentContType, m_pastInstType);
	}
}