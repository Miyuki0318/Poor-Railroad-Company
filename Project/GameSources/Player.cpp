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
		
		// csv�O���b�h�Ƃ̏Փ˔�������s
		GridHitResponse(pos);
	}

	// �O���b�h�ɏՓ˂��ĂȂ����̉�������
	void Player::GridHitResponse(Vec3& pos)
	{
		// �X�e�[�Wcsv�z��̎擾
		const auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();

		// ��ƍs
		size_t row, col;
		row = ROW(round(pos.z + 0.5f));
		col = COL(round(pos.x));

		// �z��͈̔͊O����Ȃ����̃`�F�b�N
		if (!WithInElemRange(row, col, stageMap)) return;

		// �e�����ւ̉�������
		GridHitFlontResponse(pos);
		GridHitBackResponse(pos);
		GridHitLeftResponse(pos);
		GridHitRightResponse(pos);

		// ���W�̍X�V
		SetPosition(pos);
	}

	// �w�肵���O���b�h���Փ˔�������O���b�h�����擾
	bool Player::GetIsImpassable(size_t row, size_t col)
	{
		// �X�e�[�Wcsv�z��̎擾
		const auto& stageMap = GetTypeStage<GameStage>()->GetStageMap();

		// �z��͈̔͊O����Ȃ����̃`�F�b�N
		if (!WithInElemRange(row, col, stageMap)) return false;

		// �ʂ�Ȃ��}�XID�ƈ�v���邩
		eStageID id = STAGE_ID(stageMap.at(row).at(col));
		return m_impassableSet.find(id) != m_impassableSet.end();
	}

	// �O�����ւ̏Փ�
	void Player::GridHitFlontResponse(Vec3& pos)
	{
		// ��ƍs
		size_t row, col;
		row = ROW(round(pos.z + 0.5f)) + 1;
		col = COL(round(pos.x));

		// �Փ˔�������ID���̃`�F�b�N
		if (!GetIsImpassable(row, col)) return;

		// ��������
		float gridZ = -float(row) - m_margin;
		if (pos.z - m_radius < gridZ)
		{
			pos.z = gridZ + m_radius;
		}
	}

	// ������ւ̏Փ�
	void Player::GridHitBackResponse(Vec3& pos)
	{
		// ��ƍs
		size_t row, col;
		row = ROW(round(pos.z)) - 1;
		col = COL(round(pos.x));

		// �Փ˔�������ID���̃`�F�b�N
		if (!GetIsImpassable(row, col)) return;

		// ��������
		float gridZ = -float(row) - m_margin;
		if (pos.z + m_radius > gridZ)
		{
			pos.z = gridZ - m_radius;
		}
	}

	// �������ւ̏Փ�
	void Player::GridHitLeftResponse(Vec3& pos)
	{
		// ��ƍs
		size_t row, col;
		row = ROW(round(pos.z));
		col = COL(round(pos.x + 0.5f)) - 1;

		// �Փ˔�������ID���̃`�F�b�N
		if (!GetIsImpassable(row, col)) return;

		// ��������
		float gridX = float(col) + m_margin;
		if (pos.x - m_radius < gridX)
		{
			pos.x = gridX + m_radius;
		}
	}

	// �E�����ւ̏Փ�
	void Player::GridHitRightResponse(Vec3& pos)
	{
		// ��ƍs
		size_t row, col;
		row = ROW(round(pos.z));
		col = COL(round(pos.x)) + 1;

		// �Փ˔�������ID���̃`�F�b�N
		if (!GetIsImpassable(row, col)) return;

		// ��������
		float gridX = float(col) - m_margin;
		if (pos.x + m_radius > gridX)
		{
			pos.x = gridX - m_radius;
		}
	}
}