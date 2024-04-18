/*!
@file Player.cpp
@brief �v���C���[
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

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

		// ��]�ʂ����Z���ꂽ���݂̉�]�x�N�g���̃��W�A����Y���Ƃ��Đݒ�
		float rotY = atan2f(m_currentRot.z, m_currentRot.x);
		SetRotation(Vec3(0.0f, rotY, 0.0f));
	}
	
	// �R���g���[���[�ɂ��ړ�
	void Player::ControllerMovement(const Vec3& stickValue)
	{
		// ���݂̍��W�ɓ��͗ʁ~���x�~�f���^�^�C���ŉ��Z
		Vec3 pos = GetPosition();
		pos += stickValue * m_moveSpeed * DELTA_TIME;
		
		// ���W�̍X�V
		SetPosition(pos);
	}

	// �O���b�h�ɏՓ˂��ĂȂ����̉�������
	void Player::GridHitResponse(Vec3& pos)
	{
		// �X�e�[�Wcsv�z��̎擾
		const auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();

		// �e�����ւ̉�������
		GridHitFlontResponse(pos, stageMap);
		GridHitBackResponse(pos, stageMap);
		GridHitLeftResponse(pos, stageMap);
		GridHitRightResponse(pos, stageMap);

		// ���W�̍X�V
		SetPosition(pos);
	}

	// �O�����ւ̏Փ�
	void Player::GridHitFlontResponse(Vec3& pos, const vector<vector<int>>& stageMap)
	{
		// ��ƍs
		size_t row, col;
		row = ROW(round(pos.z + 0.5f));
		col = COL(round(pos.x));

		// �z��͈̔͊O����Ȃ����̃`�F�b�N
		if (!WithInElemRange(row + 1, stageMap.size())) return;
		
		// �ʂ�Ȃ��}�XID�ƈ�v���邩
		eStageID id = static_cast<eStageID>(stageMap.at(row + 1).at(col));
		if (m_impassableSet.find(id) == m_impassableSet.end()) return;

		// ��������
		float gridZ = -float(row + 1) - m_margin;
		if (pos.z - m_radius < gridZ)
		{
			pos.z = gridZ + m_radius;
		}
	}

	// ������ւ̏Փ�
	void Player::GridHitBackResponse(Vec3& pos, const vector<vector<int>>& stageMap)
	{
		// ��ƍs
		size_t row, col;
		row = ROW(round(pos.z));
		col = COL(round(pos.x));

		// �z��͈̔͊O����Ȃ����̃`�F�b�N
		if (!WithInElemRange(row - 1, stageMap.size())) return;

		// �ʂ�Ȃ��}�XID�ƈ�v���邩
		eStageID id = static_cast<eStageID>(stageMap.at(row - 1).at(col));
		if (m_impassableSet.find(id) == m_impassableSet.end()) return;

		// ��������
		float gridZ = -float(row - 1) - m_margin;
		if (pos.z + m_radius > gridZ)
		{
			pos.z = gridZ - m_radius;
		}
	}

	// �������ւ̏Փ�
	void Player::GridHitLeftResponse(Vec3& pos, const vector<vector<int>>& stageMap)
	{
		// ��ƍs
		size_t row, col;
		row = ROW(round(pos.z));
		col = COL(round(pos.x + 0.5f));

		// �z��͈̔͊O����Ȃ����̃`�F�b�N
		if (!WithInElemRange(row, col - 1, stageMap)) return;

		// �ʂ�Ȃ��}�XID�ƈ�v���邩
		eStageID id = static_cast<eStageID>(stageMap.at(row).at(col - 1));
		if (m_impassableSet.find(id) == m_impassableSet.end()) return;

		// �O���b�h����̉�������
		float gridX = float(col - 1) + m_margin;
		if (pos.x - m_radius < gridX)
		{
			pos.x = gridX + m_radius;
		}
	}

	// �E�����ւ̏Փ�
	void Player::GridHitRightResponse(Vec3& pos, const vector<vector<int>>& stageMap)
	{
		// ��ƍs
		size_t row, col;
		row = ROW(round(pos.z));
		col = COL(round(pos.x));

		// �z��͈̔͊O����Ȃ����̃`�F�b�N
		if (!WithInElemRange(row, col + 1, stageMap)) return;
		
		// �ʂ�Ȃ��}�XID�ƈ�v���邩
		eStageID id = static_cast<eStageID>(stageMap.at(row).at(col + 1));
		if (m_impassableSet.find(id) == m_impassableSet.end()) return;
		
		// �O���b�h����̉�������
		float gridX = float(col + 1) - m_margin;
		if (pos.x + m_radius > gridX)
		{
			pos.x = gridX - m_radius;
		}
	}
}