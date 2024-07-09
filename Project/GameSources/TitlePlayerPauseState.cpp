/*!
@file TitlePlayerPauseState.cpp
@brief �v���C���[�̒�~��ԃX�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "TitlePlayerPauseState.h"
#include "TitlePlayerIdleState.h"
#include "TitlePlayerStartState.h"
#include "TitleTrain.h"
#include "TitleStage.h"
#include "Opening.h"
#include "MainCamera.h"
#include "Company.h"

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
		
		// ��Ԃ̌��̈ʒu��ێ�
		if (auto& train = player->GetStage()->GetSharedGameObject<TitleTrain>(L"TitleTrain", false))
		{
			m_trainBackPosition = train->GetPosition() + BACK_VEC;
		}
	}

	// �X�e�[�g�X�V���̏���
	void TitlePlayerPauseState::Execute(const shared_ptr<TitlePlayer>& player)
	{
		// �A�j���[�V�����X�V
		player->UpdateAnimation();

		// �^�C�g���X�e�[�W�ƃX�e�[�W�v���O���X�ƃJ�������擾
		const auto& titleStage = player->GetTypeStage<TitleStage>();
		auto& prog = titleStage->GetTitleProgress();
		auto& camera = dynamic_pointer_cast<MainCamera>(titleStage->GetView()->GetTargetCamera());
		
		// �ړ���ŃJ�������Œ�Ȃ�v���C���[����̃`�F�b�N�ƃJ�������Z�b�g�𑗂�
		if (prog == eTitleProgress::move && camera->m_cameraState == MainCamera::ZoomedIn)
		{
			prog = eTitleProgress::start;
			const auto& train = titleStage->GetSharedGameObject<TitleTrain>(L"TitleTrain");
			train->AddTag(App::GetApp()->GetScene<Scene>()->GetTagName());
			return;
		}

		// �I���܂��̓Y�[������B�{�^�����͂�����
		if (Utility::OR(prog, eTitleProgress::select, eTitleProgress::zoom) && Input::GetPushB())
		{
			// ��ЃI�u�W�F�N�g��I�����Ă�Ȃ�Y�[�����I���Č��ɖ߂�
			if (dynamic_pointer_cast<Company>(titleStage->GetSelectObject()))
			{
				player->SetState(TitlePlayerIdleState::Instance());
				auto& zoomCamera = dynamic_pointer_cast<MainCamera>(titleStage->GetView()->GetTargetCamera());
				zoomCamera->ZoomEnd();
				return;
			}

			// ��ԂɈړ��X�e�[�g�ɐ؂�ւ��āA�ړ��v���O���X�ɂ���
			player->SetState(TitlePlayerStartState::Instance());
			prog = eTitleProgress::move;
			return;
		}

		// �^�C�g���v���O���X���m�[�}���Ȃ�ҋ@��ԃX�e�[�g�ɐ؂�ւ�
		if (prog == normal)
		{
			player->SetState(TitlePlayerIdleState::Instance());

			// BGM������Ă��Ȃ���΍Đ�
			if (!titleStage->GetBGMItem().lock()) titleStage->StartBGM();

			return;
		}

		// �I�[�v�j���O�X�v���C�g���Ƀ{�^�����͂�����ҋ@��ԃX�e�[�g�ɐ؂�ւ�
		if (prog != opening) return;
		auto& opening = titleStage->GetSharedGameObject<TitleLogo>(L"TitleLogo");
		if (opening->GetLogoState() == eLogoState::exit && Input::GetPush())
		{
			prog = eTitleProgress::normal;
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