/*!
@file TitlePlayer.cpp
@brief �^�C�g���p�v���C���[�I�u�W�F�N�g
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
	void TitlePlayer::OnCreate()
	{
		// �p�����̐������̏��������s
		TemplateObject::OnCreate();
		SetTransParam();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->SetMultiMeshResource(L"PLAYER");
		m_ptrDraw->SetSpecularColor(COL_BLACK);
		m_ptrDraw->SetOwnShadowActive(true);
		m_ptrDraw->SetLightingEnabled(false);

		// �A�j���[�V�����̐ݒ�
		m_ptrDraw->AddAnimation(L"WALK", 0, 60, true);

		auto shadowMap = AddComponent<Shadowmap>();
		shadowMap->SetMultiMeshResource(L"PLAYER");
		shadowMap->SetMeshToTransformMatrix(m_modelMat);

		// �R���W����OBB�̒ǉ�
		AddComponent<CollisionCapsule>();

		// �d�͂̒ǉ�
		AddComponent<Gravity>();

		// �^�O�̐ݒ�
		AddTag(L"Player");
	}

	// ���t���[���X�V����
	void TitlePlayer::OnUpdate()
	{
		// �ړ��Ɖ�]�̍X�V����
		UpdateMove();
		UpdateRotation();
		
		// �f�o�b�N�p������
		Debug::Log(L"�v���C���[�̍��W : ", GetPosition());
	}

	// �ړ��X�V
	void TitlePlayer::UpdateMove()
	{
		// LStick�̓��͂�����Ȃ�
		bool isLStick = IsInputLStick();
		if (isLStick)
		{
			// LStick���͗ʂ̎擾
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			SetRotateTarget(stickValue); // ��]�֐�
			ControllerMovement(stickValue);	// �ړ��֐�
		}
	}

	// ��]��ݒ�
	void TitlePlayer::SetRotateTarget(const Vec3& stickValue)
	{
		// �R���g���[���[�̓��͂����]�����x�N�g����ݒ�
		float rotY = atan2f(-stickValue.z, stickValue.x);
		m_rotTarget = Vec3(cos(rotY), 0.0f, sin(rotY));
	}

	// ��]�X�V����
	void TitlePlayer::UpdateRotation()
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
	void TitlePlayer::ControllerMovement(const Vec3& stickValue)
	{
		// ���݂̍��W�ɓ��͗ʁ~���x�~�f���^�^�C���ŉ��Z
		Vec3 pos = GetPosition();
		pos += stickValue * m_moveSpeed * DELTA_TIME;

		// ���W�̍X�V
		SetPosition(pos);

		// �A�j���[�V�����̍X�V
		m_ptrDraw->UpdateAnimation(DELTA_TIME * 2.0f);
	}
}