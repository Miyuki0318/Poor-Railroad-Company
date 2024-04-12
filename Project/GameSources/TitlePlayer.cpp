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

	// ���t���[���X�V����
	void TitlePlayer::OnUpdate()
	{
		// �ړ��Ɖ�]�̍X�V����
		UpdateMove();
		UpdateRotation();
		
		// �f�o�b�N�p������
		Debug::Log(L"�v���C���[�̍��W : ", GetPosition());
	}

	// �R���|�[�l���g�ݒ�
	void TitlePlayer::CreateComponent()
	{
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->SetMultiMeshResource(L"PLAYER");
		m_ptrDraw->SetSpecularColor(COL_BLACK);
		m_ptrDraw->SetOwnShadowActive(true);
		m_ptrDraw->SetLightingEnabled(false);

		// �A�j���[�V�����̐ݒ�
		m_ptrDraw->AddAnimation(L"WALK", 0, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(L"WALK");

		auto shadowMap = AddComponent<Shadowmap>();
		shadowMap->SetMultiMeshResource(L"PLAYER");
		shadowMap->SetMeshToTransformMatrix(m_modelMat);

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

			SetRotateTarget(stickValue); // ��]�֐�
			ControllerMovement(stickValue);	// �ړ��֐�

			// �A�j���[�V�����̍X�V
			m_ptrDraw->UpdateAnimation(DELTA_TIME * 2.0f);
		}
	}
}