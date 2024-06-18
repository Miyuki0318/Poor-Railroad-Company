/*!
@file TitlePlayerPauseState.cpp
@brief �v���C���[�̒�~��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<TitlePlayerPauseState> TitlePlayerPauseState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<TitlePlayerPauseState> instance(new TitlePlayerPauseState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring TitlePlayerPauseState::GetStateName() const
	{
		return L"��~��ԃX�e�[�g";
	}

	// �X�e�[�g�J�n���̏���
	void TitlePlayerPauseState::Enter(const shared_ptr<TitlePlayer>& player)
	{
		// �A�j���[�V�����̕ύX
		player->SetAnimationMesh(ePAK::Waiting);
	}

	// �X�e�[�g�X�V���̏���
	void TitlePlayerPauseState::Execute(const shared_ptr<TitlePlayer>& player)
	{
		// �A�j���[�V�����X�V
		player->UpdateAnimation();

		const auto& titleStage = player->GetTypeStage<TitleStage>();

		auto& opening = titleStage->GetSharedGameObject<TitleLogo>(L"TitleLogo");
		if (opening->GetLogoState() == eLogoState::exit && Input::GetPushB())
		{
			player->SetState(TitlePlayerIdleState::Instance());
		}
	}

	// �X�e�[�g�I�����̏���
	void TitlePlayerPauseState::Exit(const shared_ptr<TitlePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}

	// B�{�^�����͎�
	void TitlePlayerPauseState::OnPushB(const shared_ptr<TitlePlayer>& player)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}