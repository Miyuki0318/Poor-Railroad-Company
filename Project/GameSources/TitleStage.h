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
		opening,// �I�[�v�j���O
		normal, // �ʏ�
		push,   // �{�^������
		zoom, // �Y�[���C��
		select, // �I�u�W�F�N�g�I��
		shop, // �V���b�v��ʒ�
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
		const Vec3 m_cameraEye = Vec3(18.0f, 12.5f, -30.0f);
		const Vec3 m_cameraAt = Vec3(18.0f, 2.0f, -17.0f);

		// �Y�[���ʒu�ɉ��Z���鍷��
		const Vec3 m_shopDiffEye;
		const Vec3 m_routeMapDiffEye;
		const Vec3 m_trainDiffEye;

		// ��Ԃ̊J�n�ʒu
		const Vec3 m_trainPos = Vec3(23.0f,1.0f,-20.0f);

		// �v���C���[�̊J�n���W
		Vec3 m_startPosition;
		
		const float m_searchArea = 3.0f;

		wstring m_textureKeys[3] = {
			L"SIGNBOARD_OFFICIAL_TX",
			L"SIGNBOARD_SELECT_TX",
			L"SIGNBOARD_START_TX"
		};

		Vec3 m_boardPositions[3] = {
			Vec3( 9.5f,1.0f,-10.0f),
			Vec3(18.0f,1.0f,-13.0f),
			Vec3(23.0f,1.0f,-14.5f),
		};

		const int m_boardQuantity = sizeof(m_textureKeys) / sizeof(m_textureKeys[0]);

		eTitleProgress m_titleProgress;

		Vec3 m_diff;
		float m_distance;

		bool m_zooming;

		bool m_zoomEnd;

		shared_ptr<GameObject> m_selectObj;

		shared_ptr<GameObjectGroup> m_objectGroup;

		weak_ptr<SoundItem> m_bgmItem;

		weak_ptr<Sprite> m_pushAButton;
		weak_ptr<Sprite> m_pushBButton;

		eTitleProgress m_oldProgress;

		/*
		@brief �r���[�ƃ��C�g�̐���
		*/
		void CreateViewLight();

		/*
		@brief ���\�[�X�̓Ǎ�
		*/
		void CreateResourses() override;
		
		/*
		@brief �^�C�g�����S�̐���
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
		@brief �������X�v���C�g�̐���
		*/
		void CreateUISprite();

		/*
		@brief ���UI�̐���
		*/
		void CreateArrowSprite();

		/*
		@brief �{�^��UI�̐���
		*/
		void CreateButtonUI();

		/*
		@brief �J�����̃Y�[������
		*/
		void TitleCameraZoom();

		/*
		@brief �X�v���C�g�̃t�F�[�h����
		*/
		void FadeSprite();

		/*
		@brief �{�^��UI�̕\������
		*/
		void ButtonUIActive();

		/*
		@brief A�{�^�������������̏���
		*/
		void PushButtonA();

		/*
		@brief B�{�^�������������̏���
		*/
		void PushButtonB();

		/*!
		@brief �X�e�[�W��csv�Ő���
		@param csv�̃t�@�C����
		*/
		void CreateStageCSV(string csvPath = "Title") override;

	public:
		/*
		@brief �R���X�g���N�^
		*/
		TitleStage(const string& stagePath, eTitleProgress prog) :
			BaseStage(stagePath),
			m_titleProgress(prog),
			m_shopDiffEye(0.0f, 3.0f, -0.5f),
			m_routeMapDiffEye(0.0f, 3.0f, +11.0f),
			m_trainDiffEye(0.0f, 8.0f, 12.0f)
		{
			m_oldProgress = eTitleProgress::start;

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
			return m_titleProgress == eTitleProgress::select;
		}

		/*
		@brief �Q�[���X�^�[�g����true��Ԃ��֐�
		*/
		bool GetStartFlag() const
		{
			return m_titleProgress == eTitleProgress::start;
		}

		/*
		@brief �^�C�g���X�e�[�W�̏�Ԃ�Ԃ��֐�
		*/
		eTitleProgress& GetTitleProgress()
		{
			return m_titleProgress;
		}

		shared_ptr<GameObject> GetSelectObject() const
		{
			return m_selectObj;
		}

		/*
		@brief ��Ԃ���v���Ă��邩�𔻒肷��֐�
		*/
		bool MatchProgress()
		{
			return m_oldProgress == m_titleProgress;
		}

		/*
		@brief �w�肵���I�u�W�F�N�g���I������Ă��邩���肷��֐�
		*/
		bool MatchSelectObject(const shared_ptr<GameObject>& obj)
		{
			return m_selectObj == obj;
		}
	};
}