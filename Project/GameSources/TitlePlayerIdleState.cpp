/*!
@file TitlePlayerIdleState.cpp
@brief �v���C���[�̑ҋ@��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<TitlePlayerIdleState> TitlePlayerIdleState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<TitlePlayerIdleState> instance(new TitlePlayerIdleState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring TitlePlayerIdleState::GetStateName() const
	{
		return L"�ҋ@��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void TitlePlayerIdleState::Enter(const shared_ptr<TitlePlayer>& player)
	{
		// �ړ���ԂȂ�ړ��X�e�[�g�ɑJ��
		if (Input::IsInputLStick()) player->m_status.Set(ePlayerStatus::IsMove);

		// �A�j���[�V�����̕ύX
		if (!player->IsAnimation(ePAK::Waiting))
		{
			player->SetAnimationMesh(ePAK::Waiting);
		}
	}

	// �X�e�[�g�X�V���̏���
	void TitlePlayerIdleState::Execute(const shared_ptr<TitlePlayer>& player)
	{
		// L�X�e�B�b�N���͂�����Έړ��X�e�[�g�ɑJ��
		if (Input::IsInputLStick() || !player->GetStatus(ePlayerStatus::IsIdle)) player->SetState(TitlePlayerMovingState::Instance());

		// �̌@��ԂȂ�̌@�X�e�[�g�ɑJ��
		if (player->GetStatus(ePlayerStatus::IsGathering)) player->SetState(TitlePlayerGatheringState::Instance());

		player->UpdateAnimation(player->m_animationMap.at(ePAK::Waiting).animeSpeed);

		Vec3 pos = player->GetPosition();
		player->GridHitResponse(pos);
		player->SetPosition(pos);
	}

	// �X�e�[�g�I�����̏���
	void TitlePlayerIdleState::Exit(const shared_ptr<TitlePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// B�{�^�����͎�
	void TitlePlayerIdleState::OnPushB(const shared_ptr<TitlePlayer>& player)
	{
		// �C���f�B�P�[�^�[�ɉ��������������s
		player->IndicatorOrder();
	}
}