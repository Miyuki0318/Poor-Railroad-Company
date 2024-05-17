/*!
@file TitlePlayer.cpp
@brief �^�C�g���p�v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

const float ANIME_SPEED = 0.75f; // �A�j���[�V�����̑��x

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Input;
	using namespace Utility;

	// ���t���[���X�V����
	void TitlePlayer::OnUpdate()
	{
		// �ړ��Ɖ�]�̍X�V����
		m_moveValue = 0.0f;
		UpdateMove();
		UpdateRotation();
		
		// �A�j���[�V�����̍X�V
		m_moveValue = min(floor(RadToDeg(m_moveValue), 1), m_maxMove);
		m_ptrDraw->UpdateAnimation(DELTA_TIME * m_moveValue * ANIME_SPEED);

		// �f�o�b�N�p������
		Debug::Log(L"�v���C���[�̍��W : ", GetPosition());
		Debug::Log(L"�������� : ", m_moveValue);
	}

	// �R���|�[�l���g�ݒ�
	void TitlePlayer::CreateComponent()
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
		SetAnimationMesh(ePAKey::Walking);

		// �R���W����OBB�̒ǉ�
		AddComponent<CollisionCapsule>();

		// �d�͂̒ǉ�
		AddComponent<Gravity>();
	}

	// �ړ��X�V
	void TitlePlayer::UpdateMove()
	{
		// LStick�̓��͂�����Ȃ�
		if (IsInputLStick())
		{
			// LStick���͗ʂ̎擾
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			m_acsel += DELTA_TIME;
			m_acsel = min(m_acsel, m_maxAcsel);
			SetRotateTarget(stickValue); // ��]�֐�
			ControllerMovement(stickValue);	// �ړ��֐�
		}
	}
}