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
		m_instructions[Keyboard][Normal] = stagePtr->AddGameObject<Sprite>(L"INST_NORMAL_K", m_DefaultScale, m_DefaultPos);
		m_instructions[Keyboard][Craft] = stagePtr->AddGameObject<Sprite>(L"INST_CRAFT_K", m_DefaultScale, m_DefaultPos);
		m_instructions[XBoxPad][Normal] = stagePtr->AddGameObject<Sprite>(L"INST_NORMAL_X", m_DefaultScale, m_DefaultPos);
		m_instructions[XBoxPad][Craft] = stagePtr->AddGameObject<Sprite>(L"INST_CRAFT_X", m_DefaultScale, m_DefaultPos);

		// レイヤーの設定
		for (int i = 0; i < ContTypeNum; i++)
		{
			for (int j = 0; j < InstTypeNum; j++)
			{
				m_instructions.at((eControllerType)i).at((eInstructionType)j).lock()->SetDrawLayer(m_DrawLayerNum);
			}
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
		// すべてのスプライト描画を切り替える
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
		// 指定されたタイプのスプライトの描画を切り替える
		m_instructions.at(cType).at(iType).lock()->SetDrawActive(flag);
	}

	void Instruction::ControllerTypeDecision()
	{
		// パッドが繋がれていたら
		if (Input::GetPadConected())
		{
			m_currentContType = XBoxPad; // パッド状態
		}
		else // そうでなければ
		{
			m_currentContType = Keyboard; // キーボード/マウス状態
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
			m_currentInstType = Craft; // クラフト状態
		}
		else // そうでなければ
		{
			m_currentInstType = Normal; // 通常状態
		}
		// 前フレームと現フレームの状態が違うなら表示していた画像を非表示にする
		if (m_pastInstType != m_currentInstType) SetDrawActiveInstructions(false, m_currentContType, m_pastInstType);
	}
}