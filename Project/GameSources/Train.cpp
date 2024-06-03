/*!
@file Train.cpp
@brief ��Ԃ̐e����
@author ��I�t
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void Train::OnCreate()
	{
		TemplateObject::OnCreate();
		SetPosition(m_DefaultPosition);
		SetScale(m_DefaultScale);

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticModelDraw>();
		m_ptrDraw->SetMeshResource(L"TRAIN");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);

		auto shadowMap = AddComponent<Shadowmap>();
		shadowMap->SetMeshResource(L"TRAIN");
		shadowMap->SetMeshToTransformMatrix(m_modelMat);

		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();
		m_railPos = POS2LINE(m_DefaultPosition);
		m_movePos.first = m_DefaultPosition;
		m_movePos.second = m_DefaultPosition;

		// �^�O�̐ݒ�
		AddTag(L"Train");
	}

	void Train::MoveProcess(State nextState)
	{
		// ���`��Ԃňړ�
		Vec3 pos = Utility::Lerp(m_movePos.first, m_movePos.second, m_moveRatio);
		m_moveRatio = MathF::Repeat01(m_moveRatio, m_moveSpeed);

		// �͈͊O�ɂȂ�����
		if (MathF::GetOutRange())
		{
			// ���̃��[�����������Ȃ������玟�̃X�e�[�g��
			if (!SearchNextRail()) m_state = nextState;

			SetNextRailDirection(); // ���̃��[���̕�����ݒ�
		}

		// ���W�̍X�V
		SetPosition(pos);
	}

	bool Train::SearchNextRail()
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();
		if (railMap.empty()) return false;
		
		// �n�_�ƏI�_�̐ݒ�A�I�_�������ꍇ��false��Ԃ�
		const auto& drs = m_drMap.at(m_direction);
		for (const auto& dr : drs)
		{
			// �s�Ɨ�
			size_t row, col;
			row = ROW(m_movePos.second.z + dr.y);
			col = COL(m_movePos.second.x + dr.x);

			// �s��ŃL�[��ݒ�
			string line = ROWCOL2LINE(row, col);
			if (railMap.find(line) != railMap.end())
			{
				// ���[����ݒ�
				m_movePos.first = railMap.at(m_railPos).thisPos;
				m_movePos.second = railMap.at(line).thisPos;
				m_railPos = line;

				return true;
			}
		}

		return false;
	}

	void Train::SetNextRailDirection()
	{
		// �n�_�ƏI�_�̃��[�����烉�W�A���p�����߂�
		float rad = atan2f(m_movePos.first.z - m_movePos.second.z, m_movePos.first.x - m_movePos.second.x);
		if (m_radMap.find(rad) != m_radMap.end())
		{
			// ���W�A���p�������enum��ݒ�
			m_direction = m_radMap.at(rad);
		}
	}
}