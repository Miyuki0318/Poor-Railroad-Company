/*!
@file GameTrain.cpp
@brief ��Ԃ̎���
@author ��I�t
*/

#include "stdafx.h"
#include "GameStage.h"
#include "GamePlayer.h"
#include "GameTrain.h"
#include "GameTrainState.h"
#include "MainCamera.h"

namespace basecross {

	const float START_TIME = 13.0f;

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
		bool isPause = Utility::ORS(prog, eGameProgress::FadeIn, eGameProgress::Pause, eGameProgress::CraftPause, eGameProgress::ToTitleClear);
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
		// ����������
		m_acsel = 0.0f;
		m_state = State::None;
		m_moveSpeed = m_defSpeed;
		m_railPos = POS2LINE(m_railManager.lock()->GetStartRailPos());
		m_DefaultPosition = m_railManager.lock()->GetStartRailPos();
		m_trainState->ChangeState(GameTrainStraightState::Instance());
		m_trainState->Update();

		// SE���X�g�����Z�b�g
		for (auto& sound : m_whistleSE)
		{
			auto soundItem = sound.lock();
			if (!soundItem) continue;
			if (!soundItem->m_SourceVoice) continue;

			soundItem->m_SourceVoice->Stop();
		}

		m_whistleSE.clear();
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
			if (m_acsel <= 0.0f) m_whistleSE.push_back(StartSE(L"START_WHISTLE_SE", 0.75f));

			m_acsel += DELTA_TIME / START_TIME;
			m_acsel = min(m_acsel, 1.0f);
			m_trainState->Update();

			if (m_acsel >= 1.0f)
			{
				m_state = State::OnRail;
				m_isWhistle = true;
				m_whistleSE.push_back(StartSE(L"WHISTLE_SE", 1.5f));
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

			if (m_railManager.lock()->IsConnectionGoalRail()) // �S�[���܂Ő��H���Ȃ�������
			{
				if (m_moveSpeed == m_defSpeed)
				{
					auto& camera = dynamic_pointer_cast<MainCamera>(stagePtr->GetView()->GetTargetCamera());
					camera->SetTargetObject(GetThis<GameTrain>());

					Vec3 eye = Utility::Lerp(camera->GetEye(), m_position, 0.75f);
					camera->ZoomStart(eye, m_position);

					stagePtr->SetGameProgress(eGameProgress::GoalConect);
					StopSE(L"WHISTLE_SE");
					m_whistleSE.push_back(StartSE(L"WHISTLE_SE", 1.0f));
				}
				m_moveSpeed = m_defSpeed * 5.0f; // �����i��

				if (SetTimer(START_TIME / 2.0f))
				{
					m_whistleSE.push_back(StartSE(L"WHISTLE_SE", 1.0f));
				}
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
			for (auto& sound : m_whistleSE)
			{
				auto soundItem = sound.lock();
				if (!soundItem) continue;
				if (!soundItem->m_SourceVoice) continue;

				XAUDIO2_VOICE_STATE state;
				soundItem->m_SourceVoice->GetState(&state);
				if (!(state.BuffersQueued > 0) != 0) continue;
				if (m_stopedSEKey.find(soundItem->m_SoundKey) == m_stopedSEKey.end()) continue;

				soundItem->m_SourceVoice->Start();
			}
			m_isWhistle = true;
		}

		if (m_isWhistle && isPause)
		{
			for (auto& sound : m_whistleSE)
			{
				auto soundItem = sound.lock();
				if (!soundItem) continue;
				if (!soundItem->m_SourceVoice) continue;

				XAUDIO2_VOICE_STATE state;
				soundItem->m_SourceVoice->GetState(&state);
				if (!(state.BuffersQueued > 0) != 0) continue;
				if (m_stopedSEKey.find(soundItem->m_SoundKey) == m_stopedSEKey.end()) continue;

				soundItem->m_SourceVoice->Stop();
			}
			m_isWhistle = false;
		}
	}

	void GameTrain::WhistleSmokeEffect()
	{
		bool isWhistle = false;
		for (auto& sound : m_whistleSE)
		{
			auto soundItem = sound.lock();
			if (!soundItem) continue;
			if (!soundItem->m_SourceVoice) continue;

			XAUDIO2_VOICE_STATE state;
			soundItem->m_SourceVoice->GetState(&state);
			if (!(state.BuffersQueued > 0) != 0) continue;
			if (m_whistleSEKey.find(soundItem->m_SoundKey) == m_whistleSEKey.end()) continue;

			isWhistle = true;
		}

		if (!isWhistle) return;
		m_smokeEffect.lock()->AddSmokeEffect(m_rotation.y);
	}

	int GameTrain::GetNextedRailCount()
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = GetRailDataMap();
		if (railMap.empty()) return false;

		int nextRailCount = 0;

		m_trainPos = POS2LINE(m_movePos.first);

		// ���̃��[��
		string next = POS2LINE(railMap.at(m_trainPos).futurePos);
		while (railMap.find(next) != railMap.end())
		{
			// ���[���f�[�^�̌��݂Ǝ��̃��[���̍��W�������Ȃ�I��
			if (railMap.at(next).thisPos == railMap.at(next).futurePos) break;

			// �J�E���^�𑝂₵�A�X�Ɏ��̃��[�����m�F����
			nextRailCount++;
			next = POS2LINE(railMap.at(next).futurePos);

			// ���[���f�[�^�}�b�v�ɑ��݂��Ȃ���ΏI��
			if (railMap.find(next) == railMap.end()) break;
		}

		return nextRailCount;
	}

	void GameTrain::DeRailWhistleSE()
	{
		// �ݒu���ꂽ���[�����c��1�ȉ��Ȃ�D�J��炷
		int nextedRail = GetNextedRailCount();
		if (1 >= nextedRail)
		{
			for (auto& sound : m_whistleSE)
			{
				auto soundItem = sound.lock();
				if (!soundItem) continue;
				if (!soundItem->m_SourceVoice) continue;

				XAUDIO2_VOICE_STATE state;
				soundItem->m_SourceVoice->GetState(&state);
				if (!(state.BuffersQueued > 0) != 0) continue;
				if (soundItem->m_SoundKey == L"SHORT_WHISTLE_SE") return;
			}
			m_whistleSE.push_back(StartSE(L"SHORT_WHISTLE_SE", 1.5f));
		}
	}

	bool GameTrain::SearchNextRail()
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = GetRailDataMap();
		if (railMap.empty()) return false;

		// ���̃��[���̃A���O�����擾
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

		return CheckGoalRail(); // ���[����ݒ�ł��Ȃ�������S�[�����ǂ����m�F����
	}

	bool GameTrain::NextRailSettings(const map<string, RailData>& railMap, eRailAngle nextAngle)
	{
		// 1��̃��[���f�[�^������Ȃ�
		string line = POS2LINE(railMap.at(m_railPos).futurePos);	// 1�惌�[����LINE
		if (railMap.find(line) != railMap.end())
		{
			// �����Ȃ�1��A�Ȃ�Ȃ�2��̍��W��ݒ肷��
			if (nextAngle != eRailAngle::Straight)
			{
				// ���[����ݒ�
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

	// �S�[���ɂ��ǂ蒅�������̃`�F�b�N
	bool GameTrain::CheckGoalRail()
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = GetRailDataMap();
		if (railMap.empty()) return false;

		// Line�����񂩂�row��col�𒊏o
		size_t row, col;
		GetLineStringToRowCol(row, col, m_railPos);

		// �S�[���p���[���ƈ�v���Ă���true�A����ȊO�Ȃ�false
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