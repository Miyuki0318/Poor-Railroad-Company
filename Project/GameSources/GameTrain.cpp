/*!
@file GameTrain.cpp
@brief 列車の実装
@author 矢吹悠葉
*/

#include "stdafx.h"
#include "GameStage.h"
#include "GamePlayer.h"
#include "GameTrain.h"
#include "GameTrainState.h"
#include "MainCamera.h"

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
		smokeTrans->SetPosition(GetPosition() + Vec3(0.25f, 1.5f, 0.0f));
		smokeTrans->SetParent(GetThis<GameObject>());

		m_smokeEffect = smokeEffect;
	}

	void GameTrain::OnUpdate()
	{
		eGameProgress prog = GetTypeStage<GameStage>()->GetGameProgress();
		bool isPause = Utility::ORS(prog, eGameProgress::FadeIn, eGameProgress::Pause, eGameProgress::CraftPause);
		if (!isPause)
		{
			StateProcess(m_state);
		}

		PauseStopWhistleSE(isPause);
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
		m_moveSpeed = m_defSpeed;
		m_railPos = POS2LINE(m_railManager.lock()->GetStartRailPos());
		m_DefaultPosition = m_railManager.lock()->GetStartRailPos();
		m_trainState->ChangeState(GameTrainStraightState::Instance());
		m_trainState->Update();
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
				m_isWhistle = true;
				m_whistleSE = StartSE(L"WHISTLE_SE", 1.5f);
			}
		}

		if (state == State::Arrival)
		{
			stagePtr->GetSharedGameObject<GamePlayer>(L"Player")->SetGameResult(eGameProgress::GameClear);
			stagePtr->SetGameProgress(eGameProgress::GameClear);
			m_state = State::StandBy;
		}

		if (state == State::Derail)
		{
			stagePtr->GetSharedGameObject<GamePlayer>(L"Player")->SetGameResult(eGameProgress::GameOver);
			stagePtr->SetGameProgress(eGameProgress::DeRailed);
			m_railManager.lock()->GetGuidePoints().clear();
			m_trainState->ChangeState(GameTrainDeRailedState::Instance());
			m_state = State::Staging;
			StartSE(L"TRAIN_DERAIL_SE", 1.0f);
		}

		if (state == State::Staging)
		{
			m_trainState->Update();
		}

		if (state == State::OnRail)
		{
			m_trainState->Update();

			if (m_railManager.lock()->IsConnectionGoalRail()) // ゴールまで線路がつながったら
			{
				if (m_moveSpeed == m_defSpeed)
				{
					auto& camera = dynamic_pointer_cast<MainCamera>(stagePtr->GetView()->GetTargetCamera());
					camera->SetTargetObject(GetThis<GameTrain>());

					Vec3 eye = Utility::Lerp(camera->GetEye(), m_position, 0.75f);
					camera->ZoomStart(eye, m_position);

					stagePtr->SetGameProgress(eGameProgress::GoalConect);
				}
				m_moveSpeed = m_defSpeed * 5.0f; // 早く進む
			}
			else
			{
				DeRailWhistleSE();
			}
		}

		WhistleSmokeEffect();
	}

	void GameTrain::PauseStopWhistleSE(bool isPause)
	{
		if (!m_isWhistle && !isPause)
		{
			auto soundItem = m_whistleSE.lock();
			if (!soundItem) return;
			if (!soundItem->m_SourceVoice) return;

			XAUDIO2_VOICE_STATE state;
			soundItem->m_SourceVoice->GetState(&state);
			if (!(state.BuffersQueued > 0) != 0) return;
			if (m_whistleSEKey.find(soundItem->m_SoundKey) == m_whistleSEKey.end()) return;

			soundItem->m_SourceVoice->Start();
			m_isWhistle = true;
		}

		if (m_isWhistle && isPause)
		{
			auto soundItem = m_whistleSE.lock();
			if (!soundItem) return;
			if (!soundItem->m_SourceVoice) return;

			XAUDIO2_VOICE_STATE state;
			soundItem->m_SourceVoice->GetState(&state);
			if (!(state.BuffersQueued > 0) != 0) return;
			if (m_whistleSEKey.find(soundItem->m_SoundKey) == m_whistleSEKey.end()) return;

			soundItem->m_SourceVoice->Stop();
			m_isWhistle = false;
		}
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

		m_trainPos = POS2LINE(m_movePos.first);

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
		// 設置されたレールが残り1個以下なら汽笛を鳴らす
		int nextedRail = GetNextedRailCount();
		if (1 >= nextedRail)
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