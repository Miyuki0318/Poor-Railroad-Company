/*!
@file TitleStage.h
@brief �^�C�g���X�e�[�W
@author �V�Ȉ�
*/

#pragma once
#include "stdafx.h"
#include "BaseStage.h"

namespace basecross
{
	enum eTitleProgress
	{
		normal, // �ʏ�
		push,   // �{�^������
		zoom,   // �Y�[��
		select, // �I�u�W�F�N�g�I��
		move,	// ��ԂɈړ���
		start   // �Q�[���X�^�[�g
	};

	class TitleStage : public BaseStage
	{
	private:
		// �^�O���擾
		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();

		// �n�ʂ̃X�P�[��
		const Vec3 m_groundScale = Vec3(+50.0f, +2.0f, +50.0f);

		// �J�����̈ʒu�E���_
		const Vec3 m_cameraEye = Vec3(18.0f, 20.0f, -42.5f);
		const Vec3 m_cameraAt = Vec3(18.0f, 2.0f, -18.0f);

		// ��Ԃ̊J�n�ʒu
		const Vec3 m_trainPos = Vec3(23.0f,1.0f,-20.0f);

		// �t�F�[�h�̎���
		const float m_fadeOutTime = 1.0f;
		const float m_fadeInTime = 3.0f;
		
		const float m_width = static_cast<float>(App::GetApp()->GetGameWidth());
		const float m_height = static_cast<float>(App::GetApp()->GetGameHeight());

		const float m_searchArea = 3.0f;

		eTitleProgress titleProgress;

		Vec3 m_diff;
		float m_distance;

		bool m_zooming;

		bool m_zoomEnd;

		shared_ptr<GameObject> m_selectObj;

		shared_ptr<GameObjectGroup> m_objectGroup;

		weak_ptr<SoundItem> m_bgmItem;

		/*
		@brief �r���[�ƃ��C�g�̐���
		*/
		void CreateViewLight();

		/*
		@brief ���\�[�X�̓Ǎ�
		*/
		void CreateResourses() override;
		
		/*
		@brief �I�[�v�j���O��ʂ̐���
		*/
		void CreateOpningScreen();

		/*
		@brief �n�ʂ̐���
		*/
		void CreateGround();

		/*
		@brief �^�C�g���p�̃v���C���[����
		*/
		void CreatePlayer();

		/*
		@brief ���[���Ǘ��N���X�̐���
		*/
		void CreateRailManager();

		/*
		@brief �����̐���
		*/
		void CreateBuilding();

		/*!
		@brief �̏W�I�u�W�F�N�g�̐���
		*/
		void CreateGatheringManager();

		/*
		@brief ��Ԃ̐���
		*/
		void CreateTrain();

		/*
		@brief �J�����̃Y�[������
		*/
		void TitleCameraZoom();

		/*
		@brief �X�v���C�g�̃t�F�[�h����
		*/
		void FadeSprite();

		/*
		@brief �{�^�������������̏���
		*/
		void PushButtonB();

		/*
		@brief ��Ԃ�ω������鏈��
		*/
		void Progress(shared_ptr<GameObject>& obj);

		/*!
		@brief �X�e�[�W��csv�Ő���
		@param csv�̃t�@�C����
		*/
		void CreateStageCSV(string csvPath = "Title") override;

	public:
		/*
		@brief �R���X�g���N�^
		*/
		TitleStage(const string& stagePath) : BaseStage(stagePath)
		{
			titleProgress = eTitleProgress::normal;

			m_zooming = false;

			m_zoomEnd = false;

			m_objectGroup = CreateSharedObjectGroup(L"Settings");
		}

		/*
		@brief �f�X�g���N�^
		*/
		virtual ~TitleStage() {};

		/*
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief �j������鎞�Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnDestroy() override;

		/*
		@brief ���t���[���Ăяo�����֐�
		*/
		virtual void OnUpdate() override;

		/*
		@brief BGM�J�n�֐�
		*/
		void StartBGM();

		/*
		@brief �I�u�W�F�N�g�ƃv���C���[�̋���
		*/
		void DistanceToPlayer();

		/*
		@brief �J�����̏�����
		*/
		void CameraReset();

		/*
		@brief bgm�̃T�E���h�A�C�e�����擾����֐�
		@return m_bgmItem
		*/
		weak_ptr<SoundItem>& GetBGMItem()
		{
			return m_bgmItem;
		}

		/*
		@brief �I�u�W�F�N�g�I������true��Ԃ��֐�
		*/
		bool GetPlayerStop() const
		{
			return titleProgress == eTitleProgress::select;
		}

		/*
		@brief �Q�[���X�^�[�g����true��Ԃ��֐�
		*/
		bool GetStartFlag() const
		{
			return titleProgress == eTitleProgress::start;
		}

		/*
		@brief �^�C�g���X�e�[�W�̏�Ԃ�Ԃ��֐�
		*/
		eTitleProgress& GetTitleProgress()
		{
			return titleProgress;
		}

		shared_ptr<GameObject> GetSelectObject() const
		{
			return m_selectObj;
		}
	};
}