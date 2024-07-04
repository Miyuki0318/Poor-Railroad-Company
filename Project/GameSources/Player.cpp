/*!
@file Player.cpp
@brief �v���C���[
@author ���V���M
*/

#include "stdafx.h"
#include "Input.h"
#include "OzawaUtility.h"
#include "Player.h"
#include "RailManager.h"

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

		// �v���C���[�ɕt������@�\�̐���
		CreatePlayerFeatures();

		// �^�O�̐ݒ�
		AddTag(L"Player");
	}

	void Player::CreateComponent()
	{
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->SetSpecularColor(COL_BLACK);
		m_ptrDraw->SetOwnShadowActive(true);
		m_ptrDraw->SetLightingEnabled(false);

		// �e�̐ݒ�
		m_ptrShadow = AddComponent<Shadowmap>();
		m_ptrShadow->SetMeshToTransformMatrix(m_modelMat);

		// ���b�V���ƃA�j���[�V�����̐ݒ�
		SetAnimationMesh(ePAK::Walking);

		// �R���W����OBB�̒ǉ�
		AddComponent<CollisionCapsule>();
	}

	// �A�j���[�V�������b�V���̐؂�ւ�
	void Player::SetAnimationMesh(ePAK animation, float start)
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

	// �A�j���[�V�����̍X�V
	void Player::UpdateAnimation(float speedValue)
	{
		m_ptrDraw->UpdateAnimation(DELTA_TIME * speedValue);
	}

	// �w��A�j���[�V�����ɂȂ��Ă��邩�̊m�F
	bool Player::IsAnimation(ePAK animation)
	{
		if (!m_ptrDraw) return false;

		// ���݂̃A�j���[�V�������A�j���[�V�����L�[�ƈ�v���Ă��邩
		return m_ptrDraw->GetCurrentAnimation() == m_animationMap.at(animation).animeKey;
	}

	// �w��A�j���[�V�����ɂȂ��Ă��邩�̊m�F
	bool Player::IsAnimeEnd(ePAK animation)
	{
		if (!IsAnimation(animation)) return false;

		// ���݂̃A�j���[�V�������A�j���[�V�����L�[�ƈ�v���Ă��邩
		return m_ptrDraw->IsTargetAnimeEnd();
	}

	// �̌@����
	bool Player::GatheringOrder(const shared_ptr<SelectIndicator>& indicator)
	{
		// �̌@���߂𑗂�A�̌@�ł�����^�O�Z�b�g���󂯎��
		int id = indicator->GatheringOrder();

		// �̌@�I�u�W�F�N�g�̃^�O�Z�b�g���󂶂�Ȃ���΍̌@�����𑗂�
		if (id != 0)
		{
			GatheringProcces(id);
			return true;
		}

		return false;
	}

	// �̌@����
	void Player::GatheringProcces(int stageID)
	{
		// �̌@�Ώۃ}�b�v��p���č̌@����ǉ�
		eStageID id = STAGE_ID(stageID);

		// �A�C�e���J�E���^�̒ǉ���SE�̍Đ�
		if (m_gatherMap.find(id) != m_gatherMap.end())
		{
			StartSE(m_gatherMap.at(id).second + L"_SE", 1.0f);

			// �̎�ł��邩�̐^�U
			eItemType type = m_gatherMap.at(id).first;
			m_addItem.second = (GetItemCount(type) < GetLimitCount(type));
			if (m_addItem.second)
			{
				m_addItem.first = type;
				AddItemCount(type);
			}
		}

		// �̌@��Ԃɂ���
		m_status.Set(ePlayerStatus::IsGathering) = true;
	}

	// ��]��ݒ�
	void Player::SetRotateTarget(const Vec3& stickValue)
	{
		// �R���g���[���[�̓��͂����]�����x�N�g����ݒ�
		float rotY = atan2f(-stickValue.z, stickValue.x);
		m_rotTarget = Vec3(cos(rotY), 0.0f, sin(rotY));
	}

	// �ړ��X�V����
	void Player::UpdateMove()
	{
		// �T�E���h�A�C�e�������݂��Ȃ��A�܂��̓��\�[�X����Ȃ�
		if (auto& item = m_walkSoundItem.lock()) if (item->m_AudioResource.lock()) return;

		// ����������SE���Đ�
		StartWalkSoundEffect();
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
	
	// �A�N�V�������ɃC���f�B�P�[�^�[�Ɍ���
	void Player::SetRotateIndicatorAngle()
	{
		// �G���[�`�F�b�N
		const auto& indicator = m_indicator.lock();
		if (!indicator) return;

		// ���W���m�Ŋp�x�����߂�
		Vec3 indiPos = indicator->GetPosition();
		float rotY = atan2f(indiPos.z - m_position.z, indiPos.x - m_position.x);
		m_rotTarget = Vec3(cos(rotY), 0.0f, sin(-rotY));
	}

	// ����������SE�Đ�
	void Player::StartWalkSoundEffect()
	{
		// ��ƍs
		size_t row, col;
		Vec3 pos = GetPosition();
		row = ROW(floor(pos.z + GRID_HELF));
		col = COL(floor(pos.x + GRID_HELF));

		const auto& groundMap = GetTypeStage<BaseStage>()->GetGroundMap();
		if (!WithInElemRange(row, col, groundMap)) return;

		eStageID groundID = STAGE_ID(groundMap.at(row).at(col));
		if (m_walkSEKeyMap.find(groundID) != m_walkSEKeyMap.end())
		{
			m_walkSoundItem = StartSE(m_walkSEKeyMap.at(groundID), 1.0f);
		}
	}

	// �R���g���[���[�ɂ��ړ�
	void Player::ControllerMovement(const Vec3& stickValue)
	{
		// ���݂̍��W�ɓ��͗ʁ~���x�~�f���^�^�C���ŉ��Z
		Vec3 pos = GetPosition();
		Vec3 newPos = pos + stickValue * m_playerData.at(ePST::WalkSpeed).at(m_statusLevel) * m_acsel * DELTA_TIME;
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
		eStageID stageID = STAGE_ID(stageMap.at(row).at(col));
		eStageID groundID = STAGE_ID(groundMap.at(row).at(col));

		// ID���ʂ�Ȃ��}�XID���X�g�ɓo�^����ĂȂ����̃`�F�b�N
		return (m_impassableSet.find(stageID) != m_impassableSet.end()
			|| m_impassableSet.find(groundID) != m_impassableSet.end());
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
		pos.x = min(float(stageMap.at(row).size() - GRID_SIZE), pos.x);
		pos.z = min(-0.1f, pos.z);
		pos.z = max(-float(stageMap.size() - GRID_SIZE), pos.z);
	}
}