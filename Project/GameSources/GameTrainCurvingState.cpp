/*!
@file GameTrainCurvingState.cpp
@brief �Q�[�����̗�Ԃ̃J�[�u���̃X�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	const float HELF_TIME = 0.5f;	// �����̎���

	// �C���X�^���X����
	shared_ptr<GameTrainCurvingState> GameTrainCurvingState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<GameTrainCurvingState> instance(new GameTrainCurvingState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring GameTrainCurvingState::GetStateName() const
	{
		return L"GameTrainCurvingState";
	}

	// �X�e�[�g�J�n���̏���
	void GameTrainCurvingState::Enter(const shared_ptr<GameTrain>& train)
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = train->GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();
		if (railMap.empty()) return;
	
		// ���݂̃��[���A�J�[�u���郌�[���A�I�����̃��[���̍��W
		Vec3 pointA = railMap.at(train->m_railPos).thisPos;
		Vec3 pointB = railMap.at(POS2LINE(railMap.at(train->m_railPos).futurePos)).thisPos;
		Vec3 pointC = railMap.at(POS2LINE(railMap.at(train->m_railPos).futurePos)).futurePos;

		// ���Ԃ̍��W
		Vec3 pointA2B = Utility::Lerp(pointA, pointB, HELF_TIME);
		Vec3 pointB2C = Utility::Lerp(pointB, pointC, HELF_TIME);

		// �J�[�u���W�̐ݒ�Ɗ����̏�����
		train->m_curvePoints = CurvePoints(pointA2B, pointB, pointB2C);
		train->m_moveRatio = 0.0f;
	}

	// �X�e�[�g�X�V���̏���
	void GameTrainCurvingState::Execute(const shared_ptr<GameTrain>& train)
	{
		// �J�[�u����`��Ԃŏ���
		Vec3 pos = Utility::CurveLerp(train->m_curvePoints.pointA, train->m_curvePoints.pointB, train->m_curvePoints.pointC, train->m_moveRatio);

		// ���[�e�[�V��������`��Ԃŏ���
		float startRad = -Utility::rotYatan2f(train->m_curvePoints.pointA, train->m_curvePoints.pointB);
		float endRad = -Utility::rotYatan2f(train->m_curvePoints.pointB, train->m_curvePoints.pointC);
		float rad = Utility::Lerp(startRad, endRad, train->m_moveRatio);

		// �����̉��Z
		train->m_moveRatio = MathF::Repeat01(train->m_moveRatio, train->m_MoveSpeed);

		// �͈͊O�ɂȂ�����
		if (MathF::GetOutRange())
		{
			// �J�[�u�I�����̃X�e�[�g�ɐ؂�ւ�
			train->m_trainState->ChangeState(GameTrainCurveExitState::Instance());
		}

		// ���W�ƃ��[�e�[�V�����̍X�V
		train->SetPosition(pos);
		train->SetRotation(Vec3(0.0f, rad, 0.0f));
	}

	// �X�e�[�g�I�����̏���
	void GameTrainCurvingState::Exit(const shared_ptr<GameTrain>& train)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}