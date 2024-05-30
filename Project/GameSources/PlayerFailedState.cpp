/*!
@file PlayerFailedState.cpp
@brief �v���C���[�̃Q�[�����s��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<PlayerFailedState> PlayerFailedState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<PlayerFailedState> instance(new PlayerFailedState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring PlayerFailedState::GetStateName() const
	{
		return L"���s��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void PlayerFailedState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// �X�e�[�g�X�V���̏���
	void PlayerFailedState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// �X�e�[�g�I�����̏���
	void PlayerFailedState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// A�{�^�����͎�
	void PlayerFailedState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// B�{�^�����͎�
	void PlayerFailedState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// Y�{�^�����͎�
	void PlayerFailedState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// X�{�^�����͎�
	void PlayerFailedState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}