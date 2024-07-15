/*!
@file GameTrainDeRailedState.h
@brief �Q�[�����̗�Ԃ̒E�����o�̃X�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "GameTrainDeRailedState.h"
#include "MathFunc.h"
#include "GameStage.h"

namespace basecross
{
	const float HELF_TIME = 0.5f;	// �����̎���

	// �C���X�^���X����
	shared_ptr<GameTrainDeRailedState> GameTrainDeRailedState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<GameTrainDeRailedState> instance(new GameTrainDeRailedState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring GameTrainDeRailedState::GetStateName() const
	{
		return L"GameTrainDeRailedState";
	}

	// �X�e�[�g�J�n���̏���
	void GameTrainDeRailedState::Enter(const shared_ptr<GameTrain>& train)
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = train->GetRailDataMap();
		if (railMap.empty()) return;

		// ���[���}�l�[�W���[�̎擾
		const auto& railManager = train->m_railManager.lock();
		if (!railManager) return;

		// ���[����ݒ�
		string pastLine = POS2LINE(railManager->GetPastRailPos());
		Vec3 pointA = railMap.at(pastLine).thisPos;
		Vec3 pastPos = railMap.at(pastLine).pastPos;
		float rot = -atan2f(pointA.z - pastPos.z, pointA.x - pastPos.x);
		Vec3 pointB = pointA + Vec3(cos(rot), 0.0f, sin(-rot));
		Vec3 pointC = pointB + (Vec3(cos(rot + XM_PIDIV2), 0.0f, sin(rot + XM_PIDIV2)) * Utility::RangeRand(0.5f, -0.5f));
		Vec3 pointA2B = Utility::Lerp(pointA, pointB, HELF_TIME);
		
		// �J�[�u���W�̐ݒ�Ɗ����̏�����
		m_curvePoints = CurvePoints(pointA, pointA2B, pointC);
		train->m_moveRatio = 0.0f;

		// ���[�e�[�V����Y����ێ�
		m_pastRotY = train->GetRotation().y;
	}

	// �X�e�[�g�X�V���̏���
	void GameTrainDeRailedState::Execute(const shared_ptr<GameTrain>& train)
	{
		// �J�[�u����`��Ԃŏ���
		Vec3 pos = Utility::BezierLerp(m_curvePoints.pointA, m_curvePoints.pointB, m_curvePoints.pointC, train->m_moveRatio);

		// ��]�̐ݒ�
		float endRad = -atan2f(m_curvePoints.pointC.z - m_curvePoints.pointA.z, m_curvePoints.pointC.x - m_curvePoints.pointA.x);
		float rad = Utility::Lerp(m_pastRotY, endRad, train->m_moveRatio);

		// �����̉��Z
		train->m_moveRatio = MathF::Repeat01(train->m_moveRatio, train->m_moveSpeed * train->m_acsel);

		// �͈͊O�ɂȂ�����
		if (MathF::GetOutRange())
		{
			// �ҋ@��Ԃ̃X�e�[�g�ɐ؂�ւ�
			train->m_state = Train::State::StandBy;
			train->GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::GameOver);
		}

		// ���W�ƃ��[�e�[�V�����̍X�V
		train->SetPosition(pos);
		train->SetRotation(Vec3(0.0f, rad, 0.0f));
	}

	// �X�e�[�g�I�����̏���
	void GameTrainDeRailedState::Exit(const shared_ptr<GameTrain>& train)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}