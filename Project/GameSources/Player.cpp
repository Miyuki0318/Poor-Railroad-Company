/*!
@file Player.cpp
@brief �v���C���[
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

#define MESH L"SM_PLAYER_"

const int GRID_SIZE = 1;
const float GRID_HELF = 0.5f;

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Input;
	using namespace Utility;

	// �������̏���
	void Player::OnCreate()
	{
		// �p�����̐������̏��������s
		TemplateObject::OnCreate();
		SetTransParam();

		// �R���|�[�l���g�̐ݒ�
		CreateComponent();

		// �^�O�̐ݒ�
		AddTag(L"Player");
	}

	// �A�j���[�V�������b�V���̐؂�ւ�
	void Player::SetAnimationMesh(ePAKey animation, float start)
	{
		if (!m_ptrDraw) return;
		if (!m_ptrShadow) return;

		// �A�j���[�V�����t�����b�V���̕ύX
		AnimationMap mesh = m_animationMap.at(animation);
		m_ptrDraw->SetMeshResource(MESH + mesh.animeKey);
		m_ptrDraw->AddAnimation(mesh.animeKey, 0, mesh.flameNum, mesh.loopActive);
		m_ptrDraw->ChangeCurrentAnimation(mesh.animeKey, start);

		// �e�̍X�V
		m_ptrShadow->SetMeshResource(MESH + mesh.animeKey);
	}

	// �w��A�j���[�V�����ɂȂ��Ă��邩�̊m�F
	bool Player::IsAnimation(ePAKey animation)
	{
		if (!m_ptrDraw) return false;

		// ���݂̃A�j���[�V�������A�j���[�V�����L�[�ƈ�v���Ă��邩
		return m_ptrDraw->GetCurrentAnimation() == m_animationMap.at(animation).animeKey;
	}

	// �w��A�j���[�V�����ɂȂ��Ă��邩�̊m�F
	bool Player::IsAnimeEnd(ePAKey animation)
	{
		if (!IsAnimation(animation)) return false;

		// ���݂̃A�j���[�V�������A�j���[�V�����L�[�ƈ�v���Ă��邩
		return m_ptrDraw->IsTargetAnimeEnd();
	}

	// ��]��ݒ�
	void Player::SetRotateTarget(const Vec3& stickValue)
	{
		// �R���g���[���[�̓��͂����]�����x�N�g����ݒ�
		float rotY = atan2f(-stickValue.z, stickValue.x);
		m_rotTarget = Vec3(cos(rotY), 0.0f, sin(rotY));
	}

	// ��]�X�V����
	void Player::UpdateRotation()
	{
		// ��]�����x�N�g���ƌ��݂̉�]�x�N�g���̍��������]�ʂ�ݒ�
		Vec3 rot = m_rotTarget - m_currentRot;
		float rotate = rot.length() * (DELTA_TIME / m_rotSpeed);
		m_currentRot += Vec3(rot.normalize() * rotate);
		m_moveValue += rotate / m_maxMove;

		// ��]�ʂ����Z���ꂽ���݂̉�]�x�N�g���̃��W�A����Y���Ƃ��Đݒ�
		float rotY = atan2f(m_currentRot.z, m_currentRot.x);
		SetRotation(Vec3(0.0f, rotY, 0.0f));
	}
	
	// �R���g���[���[�ɂ��ړ�
	void Player::ControllerMovement(const Vec3& stickValue)
	{
		// ���݂̍��W�ɓ��͗ʁ~���x�~�f���^�^�C���ŉ��Z
		Vec3 pos = GetPosition();
		Vec3 newPos = pos + stickValue * m_moveSpeed * m_acsel * DELTA_TIME;
		m_moveValue += (newPos - pos).lengthSqr();

		// csv�O���b�h�Ƃ̏Փ˔�������s
		GridHitResponse(newPos);
	}

	// �O���b�h�ɏՓ˂��ĂȂ����̉�������
	void Player::GridHitResponse(Vec3& pos)
	{
		// �X�e�[�Wcsv�z��̎擾
		const auto& stagePtr = GetTypeStage<BaseStage>();
		const auto& stageMap = stagePtr->GetStageMap();
		const auto& positionMap = stagePtr->GetStagePosMap();

		// ��ƍs
		size_t row, col;
		row = ROW(floor(pos.z + GRID_HELF));
		col = COL(floor(pos.x + GRID_HELF));

		// �z��͈̔͊O����Ȃ����̃`�F�b�N
		if (!WithInElemRange(row, col, stageMap)) return;

		// �Փ˔���
		GridHitFlontResponse(pos, positionMap);
		GridHitBackResponse(pos, positionMap);
		GridHitLeftResponse(pos, positionMap);
		GridHitRightResponse(pos, positionMap);
		GridHitOliqueResponse(pos, positionMap);

		// �X�e�[�W�z��͈̔͊O�ɍs���Ȃ��l�ɏC��
		GridRangeResponse(pos, stageMap);

		// ���W�̍X�V
		SetPosition(pos);
	}

	// �w�肵���O���b�h���Փ˔�������O���b�h�����擾
	bool Player::GetIsImpassable(size_t row, size_t col)
	{
		// �X�e�[�Wcsv�z��̎擾
		const auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		const auto&	groundMap = GetTypeStage<BaseStage>()->GetGroundMap();

		// �z��͈̔͊O����Ȃ����̃`�F�b�N
		if (!WithInElemRange(row, col, stageMap)) return false;
		if (!WithInElemRange(row, col, groundMap)) return false;

		// �ʂ�Ȃ��}�XID�ƈ�v���邩
		eStageID id = STAGE_ID(stageMap.at(row).at(col));
		if (m_impassableSet.find(id) != m_impassableSet.end())
		{
			return true;
		}
		else
		{
			id = STAGE_ID(groundMap.at(row).at(col));
			return m_impassableSet.find(id) != m_impassableSet.end();
		}
	}

	// �O�����̒藝�ŉ����o������
	void Player::GridSquareTheorem(Vec3& pos, float gridX, float gridZ)
	{
		// �O�����̒藝�ŉ����o�����������߂�
		float a = pos.x - gridX;
		float b = pos.z - gridZ;
		float c = a * a + b * b;
		if (c < m_radius * m_radius)
		{
			c = sqrtf(c);
			float s = m_radius - c;
			float dx = a / c;
			float dz = b / c;
			pos.x += dx * s;
			pos.z += dz * s;
		}
	}

	// �O�����ւ̏Փ�
	void Player::GridHitFlontResponse(Vec3& pos, const vector<vector<Vec3>>& posMap)
	{
		// ��ƍs
		size_t row, col;
		row = ROW(floor(pos.z + GRID_HELF)) + GRID_SIZE;
		col = COL(floor(pos.x + GRID_HELF));

		// �Փ˔�������ID���̃`�F�b�N
		if (!GetIsImpassable(row, col)) return;

		// ��������
		float gridZ = posMap.at(row).at(col).z;
		if (pos.z - m_radius < gridZ)
		{
			pos.z = gridZ + m_radius;
		}
	}

	// ������ւ̏Փ�
	void Player::GridHitBackResponse(Vec3& pos, const vector<vector<Vec3>>& posMap)
	{
		// ��ƍs
		size_t row, col;
		row = ROW(floor(pos.z + GRID_HELF)) - GRID_SIZE;
		col = COL(floor(pos.x + GRID_HELF));

		// �Փ˔�������ID���̃`�F�b�N
		if (!GetIsImpassable(row, col)) return;

		// ��������
		float gridZ = posMap.at(row).at(col).z;
		if (pos.z + m_radius > gridZ)
		{
			pos.z = gridZ - m_radius;
		}
	}

	// �������ւ̏Փ�
	void Player::GridHitLeftResponse(Vec3& pos, const vector<vector<Vec3>>& posMap)
	{
		// ��ƍs
		size_t row, col;
		row = ROW(floor(pos.z + GRID_HELF));
		col = COL(floor(pos.x + GRID_HELF)) - GRID_SIZE;

		// �Փ˔�������ID���̃`�F�b�N
		if (!GetIsImpassable(row, col)) return;

		// ��������
		float gridX = posMap.at(row).at(col).x;
		if (pos.x - m_radius < gridX)
		{
			pos.x = gridX + m_radius;
		}
	}

	// �E�����ւ̏Փ�
	void Player::GridHitRightResponse(Vec3& pos, const vector<vector<Vec3>>& posMap)
	{
		// ��ƍs
		size_t row, col;
		row = ROW(floor(pos.z + GRID_HELF));
		col = COL(floor(pos.x + GRID_HELF)) + GRID_SIZE;

		// �Փ˔�������ID���̃`�F�b�N
		if (!GetIsImpassable(row, col)) return;

		// ��������
		float gridX = posMap.at(row).at(col).x;
		if (pos.x + m_radius > gridX)
		{
			pos.x = gridX - m_radius;
		}
	}

	// �΂ߕ����ւ̏Փ�
	void Player::GridHitOliqueResponse(Vec3& pos, const vector<vector<Vec3>>& posMap)
	{
		// �΂߂̃O���b�h�����[�v
		for (const auto& oblique : m_obliqueGridArray)
		{
			// ��ƍs
			size_t row, col;
			row = ROW(floor(pos.z + GRID_HELF)) + oblique.first;
			col = COL(floor(pos.x + GRID_HELF)) + oblique.second;

			// �Փ˔�������ID���̃`�F�b�N
			if (!GetIsImpassable(row, col)) continue;

			// �O�����̒藝�ŉ����o���������s��
			float gridX = posMap.at(row).at(col).x;
			float gridZ = posMap.at(row).at(col).z;
			GridSquareTheorem(pos, gridX, gridZ);
		}
	}

	// �X�e�[�W�O�ւ̏Փ�
	void Player::GridRangeResponse(Vec3& pos, const vector<vector<int>>& stageMap)
	{
		// ��ƍs
		size_t row, col;
		row = ROW(floor(pos.z + GRID_HELF));
		col = COL(floor(pos.x + GRID_HELF));

		pos.x = max(0.0f, pos.x);
		pos.x = min(float(stageMap.at(row).size()), pos.x);
		pos.z = min(-0.1f, pos.z);
		pos.z = max(-float(stageMap.size() - GRID_SIZE), pos.z);
	}
}