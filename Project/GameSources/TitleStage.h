/*!
@file TitleStage.h
@brief �^�C�g���X�e�[�W
@author �V�Ȉ�
*/

#pragma once
#include "stdafx.h"
#include "StageCSV.h"
#include "ResourcesManager.h"

namespace basecross
{
	enum Switching
	{
		off,
		on
	};

	class TitleStage : public StageCSV, public ResourcesManager
	{
	private:
		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();

		// �n�ʂ̃X�P�[��
		const Vec3 m_groundScale = Vec3(+50.0f, +2.0f, +50.0f);

		// �J�����̈ʒu�E���_
		const Vec3 m_cameraEye = Vec3(25.0f, 20.0f, -47.0f);
		const Vec3 m_cameraAt = Vec3(25.0f, 2.0f, -25.0f);

		// �t�F�[�h�̎���
		const float m_fadeOutTime = 1.0f;
		const float m_fadeInTime = 3.0f;
		
		const float m_width = static_cast<float>(App::GetApp()->GetGameWidth());
		const float m_height = static_cast<float>(App::GetApp()->GetGameHeight());

		Vec3 m_diff;
		float m_distance;

		// �{�^���������ꂽ��
		bool m_buttonPush;

		bool m_zooming;

		shared_ptr<Sprite> m_fadeSprite;

		shared_ptr<GameObject> m_selectObj;

		shared_ptr<GameObjectGroup> m_objectGroup;

		/*
		@brief �r���[�ƃ��C�g�̐���
		*/
		void CreateViewLight();

		/*
		@brief ���\�[�X�̓Ǎ�
		*/
		void CreateResourses();

		/*!
		@brief ���\�[�X�̉��
		*/
		void ReleasedResourses();

		/*
		@brief �X�v���C�g�̐���
		*/
		void CreateSprite();

		/*
		@brief �n�ʂ̐���
		*/
		void CreateGround();

		/*
		@brief �^�C�g���p�̃v���C���[����
		*/
		void CreatePlayer();

		/*
		@brief �����̐���
		*/
		void CreateBuilding();

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
		void PushButtonX();

		/*
		@brief �I�u�W�F�N�g�ƃv���C���[�̋���
		*/
		void DistanceToPlayer();

		/*!
		@brief �X�e�[�W��csv�Ő���
		@param csv�̃t�@�C����
		*/
		void CreateStageCSV(string csvPath = "TestStage") override
		{
		}

	public:
		/*
		@brief �R���X�g���N�^
		*/
		TitleStage() : StageCSV(), ResourcesManager()
		{
			m_buttonPush = false;
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

		/*!
		@brief �`��X�V�֐�
		*/
		virtual void OnDraw() override;

		wstring GetTagName()
		{
			return tagName;
		}
	};
}