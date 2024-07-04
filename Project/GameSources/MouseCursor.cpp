/*!
@file MouseCursor.cpp
@brief �L�[�}�E���쎞�̃}�E�X�J�[�\��
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Input;

	// �������̏���
	void MouseCursor::OnCreate()
	{
		// �p�����̐������̏���
		Sprite::OnCreate();

		// ���C���[�ݒ�
		SetDrawLayer(11);
	}

	// �X�V����
	void MouseCursor::OnUpdate()
	{
		// �R���g���[���[�ڑ������邩
		bool isController = GetPadConected();
		SetDrawActive(!isController);
		if (isController) return;

		// �}�E�X���W���擾
		Vec3 windowPos = Vec3(GetMousePosition());

		// �E�B���h�E�T�C�Y����X�P�[�������������Ŕ͈͂�ݒ�
		Vec3 windowRange = Vec3((WINDOW_SIZE - Vec2(m_scale)) / 2.0f);
		windowPos.clamp(-windowRange, windowRange);

		// ���W�̍X�V
		SetPosition(windowPos);
	}
}