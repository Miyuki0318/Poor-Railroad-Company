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
		usually, // �ʏ�
		push,   // �{�^������
		zoom,   // �Y�[��
		select, // �I�u�W�F�N�g�I��
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
		const Vec3 m_cameraEye = Vec3(25.0f, 20.0f, -47.0f);
		const Vec3 m_cameraAt = Vec3(25.0f, 2.0f, -25.0f);

		// ��Ԃ̊J�n�ʒu
		const Vec3 m_trainPos = Vec3(35.0f,1.0f,-29.0f);

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

		shared_ptr<GameObject> m_selectObj;

		shared_ptr<GameObjectGroup> m_objectGroup;

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

		/*
		@brief �I�u�W�F�N�g�ƃv���C���[�̋���
		*/
		void DistanceToPlayer();

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
			titleProgress = eTitleProgress::usually;

			m_zooming = false;

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

		void StartBGM();

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
	};
}