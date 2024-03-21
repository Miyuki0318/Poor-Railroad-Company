/*!
@file TemplateObject.cpp
@brief �I�u�W�F�N�g�̌p����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void TemplateObject::OnCreate()
	{
		// �g�����X�t�H�[���R���|�[�l���g�̎擾
		m_ptrTrans = GetComponent<Transform>();
	}

	// SE�̍Đ�
	void TemplateObject::StartSE(const wstring& seKey, float volume)
	{
		// �����X�e�[�W���SEManager��SE�Đ��֐��𑗂�
		GetTypeStage<BaseStage>()->CreateSE(seKey, volume, ThisPtr);
	}

	// SE�̒�~
	void TemplateObject::StopSE(const wstring& seKey)
	{
		// �����X�e�[�W���SEManager��SE��~�֐��𑗂�
		GetTypeStage<BaseStage>()->StopSE(seKey, ThisPtr);
	}

	// �^�C�}�[�ݒ�
	bool TemplateObject::SetTimer(float time, bool reset)
	{
		// �����X�e�[�W����^�C�}�[���擾
		const auto& timer = GetTypeStage<BaseStage>()->GetTimer();

		// �^�C�}�[��ݒ�
		return timer->SetTimer(ThisPtr, time, reset);
	}

	// �^�C�}�[�̌o�ߎ��Ԏ擾
	float TemplateObject::GetTime(float time)
	{
		// �����X�e�[�W����^�C�}�[���擾
		const auto& timer = GetTypeStage<BaseStage>()->GetTimer();

		// �^�C�}�[�̌o�ߎ��Ԃ��擾
		return timer->GetTime(ThisPtr, time);
	}
}