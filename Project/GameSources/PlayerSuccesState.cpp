/*!
@file PlayerSuccesState.cpp
@brief �v���C���[�̃Q�[���N���A��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<PlayerSuccesState> PlayerSuccesState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<PlayerSuccesState> instance(new PlayerSuccesState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring PlayerSuccesState::GetStateName() const
	{
		return L"�N���A��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void PlayerSuccesState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// �A�j���[�V�����̕ύX
		if (!player->IsAnimation(ePAKey::GameSucces))
		{
			player->SetAnimationMesh(ePAKey::GameSucces);
		}
	}

	// �X�e�[�g�X�V���̏���
	void PlayerSuccesState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// �A�j���[�V�������X�V
		player->UpdateAnimation();
	}

	// �X�e�[�g�I�����̏���
	void PlayerSuccesState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void PlayerSuccesState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// B�{�^�����͎�
	void PlayerSuccesState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// Y�{�^�����͎�
	void PlayerSuccesState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// X�{�^�����͎�
	void PlayerSuccesState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}