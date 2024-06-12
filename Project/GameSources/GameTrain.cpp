/*!
@file GameTrain.cpp
@brief 列車の実装
@author 矢吹悠葉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	const float START_TIME = 5.0f;

	void GameTrain::OnCreate()
	{
		Train::OnCreate();

		const auto& stagePtr = GetStage();

		m_railManager = stagePtr->GetSharedGameObject<RailManager>(L"RailManager");
		m_railDataMap = &m_railManager.lock()->GetRailDataMap();
		m_railPos = POS2LINE(m_railManager.lock()->GetStartRailPos());
		m_trainPos = m_railPos;

		m_trainState.reset(new StateMachine<GameTrain>(GetThis<GameTrain>()));
		m_trainState->ChangeState(GameTrainStraightState::Instance());

		auto smokeEffect = stagePtr->AddGameObject<SmokeEffect>();
		auto smokeTrans = smokeEffect->GetComponent<Transform>();
		smokeTrans->SetPosition(GetPosition() + Vec3(0.5f, 1.5f, 0.0f));
		smokeTrans->SetParent(GetThis<GameObject>());

		m_smokeEffect = smokeEffect;
	}

	void GameTrain::OnUpdate()
	{
		eGameProgress prog = GetTypeStage<GameStage>()->GetGameProgress();
		if (prog != eGameProgress::FadeIn)
		{
			StateProcess(m_state);
		}

		Debug::Log(L"現在のステート : ", m_trainState->GetCurrentState()->GetStateName());
	}

	void GameTrain::OnCollisionEnter(shared_ptr<GameObject>& gameObject)
	{
		if (gameObject->FindTag(L"GoalRail"))
		{
			m_state = State::Arrival;
		}
	}

	void GameTrain::ResetTrain()
	{
		m_acsel = 0.0f;
		m_state = State::None;
		m_railPos = POS2LINE(m_railManager.lock()->GetStartRailPos());
		m_trainState->ChangeState(GameTrainStraightState::Instance());
	}

	const map<string, RailData>& GameTrain::GetRailDataMap() const
	{
		return *m_railDataMap;
	}

	void GameTrain::StateProcess(State state)
	{
		const auto& stagePtr = GetTypeStage<GameStage>();

		if (state == State::None)
		{
			m_acsel += DELTA_TIME / START_TIME;
			m_acsel = min(m_acsel, 1.0f);
			m_trainState->Update();

			if (SetTimer(START_TIME))
			{
				m_state = State::OnRail;
				m_whistleSE = StartSE(L"WHISTLE_SE", 1.5f);
			}
		}

		if (state == State::Arrival)
		{
			stagePtr->SetGameProgress(eGameProgress::GameClear);
			stagePtr->GetSharedGameObject<GamePlayer>(L"Player")->SetGameResult(eGameProgress::GameClear);
			return;
		}

		if (state == State::Derail)
		{
			stagePtr->SetGameProgress(eGameProgress::GameOver);
			stagePtr->GetSharedGameObject<GamePlayer>(L"Player")->SetGameResult(eGameProgress::GameOver);
			return;
		}

		if (state == State::OnRail)
		{
			m_trainState->Update();

			if (m_railManager.lock()->IsConnectionGoalRail()) // ゴールまで線路がつながったら
			{
				m_moveSpeed = m_defSpeed * 1.2f; // 早く進む
			}
			else
			{
				DeRailWhistleSE();
			}
		}

		WhistleSmokeEffect();
	}

	void GameTrain::WhistleSmokeEffect()
	{
		auto soundItem = m_whistleSE.lock();
		if (!soundItem) return;
		if (m_whistleSEKey.find(soundItem->m_SoundKey) == m_whistleSEKey.end()) return;
		if (!soundItem->m_AudioResource.lock()) return;

		m_smokeEffect.lock()->AddSmokeEffect(m_rotation.y);
	}

	int GameTrain::GetNextedRailCount()
	{
		// レールマップの取得
		const auto& railMap = GetRailDataMap();
		if (railMap.empty()) return false;

		int nextRailCount = 0;

		m_trainPos = POS2LINE(GetPosition());

		// 一個先のレール
		string next = POS2LINE(railMap.at(m_trainPos).futurePos);
		while (railMap.find(next) != railMap.end())
		{
			// レールデータの現在と次のレールの座標が同じなら終了
			if (railMap.at(next).thisPos == railMap.at(next).futurePos) break;

			// カウンタを増やし、更に次のレールを確認する
			nextRailCount++;
			next = POS2LINE(railMap.at(next).futurePos);

			// レールデータマップに存在しなければ終了
			if (railMap.find(next) == railMap.end()) break;
		}

		return nextRailCount;
	}

	void GameTrain::DeRailWhistleSE()
	{
		int nextedRail = GetNextedRailCount();
		if (2 >= nextedRail)
		{
			// サウンドアイテムが存在しない、またはリソースが空なら
			if (auto& item = m_whistleSE.lock()) if (item->m_AudioResource.lock()) return;
			m_whistleSE = StartSE(L"SHORT_WHISTLE_SE", 1.5f);
		}
	}

	bool GameTrain::SearchNextRail()
	{
		// レールマップの取得
		const auto& railMap = GetRailDataMap();
		if (railMap.empty()) return false;

		// 次のレールのアングルを取得
		Vec3 checkPos = railMap.find(m_railPos) != railMap.end() ? railMap.at(m_railPos).futurePos : railMap.at(m_railPos).thisPos;
		string toNextLine = POS2LINE(checkPos);
		if (railMap.find(toNextLine) != railMap.end())
		{
			eRailAngle nextRailAngle = railMap.at(toNextLine).angle;

			if (NextRailSettings(railMap, nextRailAngle))
			{
				return true;
			}
		}

		return CheckGoalRail(); // レールを設定できなかったらゴールかどうか確認する
	}

	bool GameTrain::NextRailSettings(const map<string, RailData>& railMap, eRailAngle nextAngle)
	{
		// 1個先のレールデータがあるなら
		string line = POS2LINE(railMap.at(m_railPos).futurePos);	// 1個先レールのLINE
		if (railMap.find(line) != railMap.end())
		{
			// 直線なら1個先、曲るなら2個先の座標を設定する
			if (nextAngle != eRailAngle::Straight)
			{
				// レールを設定
				m_trainState->ChangeState(GameTrainCurveStandbyState::Instance());
			}
			else
			{
				m_trainState->ChangeState(GameTrainStraightState::Instance());
			}
			return true;
		}

		return false;
	}

	// ゴールにたどり着いたかのチェック
	bool GameTrain::CheckGoalRail()
	{
		// レールマップの取得
		const auto& railMap = GetRailDataMap();
		if (railMap.empty()) return false;

		// Line文字列からrowとcolを抽出
		size_t row, col;
		GetLineStringToRowCol(row, col, m_railPos);

		// ゴール用レールと一致してたらtrue、それ以外ならfalse
		const auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();
		if (Utility::WithInElemRange(row, row, stageMap))
		{
			if (STAGE_ID(stageMap.at(row).at(col)) == eStageID::GoalRail)
			{
				m_movePos.first = railMap.at(m_railPos).thisPos;
				m_movePos.second = railMap.at(m_railPos).thisPos;
				m_state = State::Arrival;
				return true;
			}
		}

		return false;
	}
}