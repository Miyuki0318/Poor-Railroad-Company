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

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_RED);

		// �R���W����OBB�̒ǉ�
		m_ptrColl = AddComponent<CollisionObb>();

		// �Z���N�g�C���f�B�P�[�^�[�̐���
		m_indicator = GetStage()->AddGameObject<SelectIndicator>(GetThis<Player>());

		// �^�O�̐ݒ�
		AddTag(L"Player");
	}

	// ���t���[���X�V����
	void Player::OnUpdate()
	{
		// A�{�^�������͂��ꂽ��
		if (GetPushA()) OnPushA();

		// X�{�^�������͂��ꂽ��N���t�g
		if (GetPushX()) OnCraft();

		// �ړ��X�V
		UpdateMove();

		// �f�o�b�N�p������
		Debug::Log(L"�v���C���[�̍��W : ", GetPosition());
	}

	// A�{�^�����͎�
	void Player::OnPushA()
	{

	}

	// �̌@�Ăяo��
	void Player::OnMining()
	{

	}

	// �N���t�g�Ăяo��
	void Player::OnCraft()
	{

	}

	// �ړ��X�V
	void Player::UpdateMove()
	{
		// LStick�̓��͂������
		if (IsInputLStick())
		{
			// LStick���͗ʂ̎擾
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			ControllerRotation(stickValue); // ��]�֐�
			ControllerMovement(stickValue);	// �ړ��֐�
		}
	}

	// �R���g���[���[�ɂ���]
	void Player::ControllerRotation(const Vec3& stickValue)
	{
		// �X�e�B�b�N�̌X���ɍ��킹�ăI�u�W�F�N�g����]������
		float rotY = atan2f(-stickValue.z, stickValue.x);
		SetRotation(Vec3(0.0f, rotY, 0.0f));
	}
	
	// �R���g���[���[�ɂ��ړ�
	void Player::ControllerMovement(const Vec3& stickValue)
	{
		// ���݂̍��W�ɓ��͗ʁ~���x�~�f���^�^�C���ŉ��Z
		Vec3 pos = GetPosition();
		pos += stickValue * m_speed * DELTA_TIME;
		
		// ���W�̍X�V
		SetPosition(pos);
	}
}