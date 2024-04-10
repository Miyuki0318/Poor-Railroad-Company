/*!
@file Building.h
@brief �����̐ݒ�
@author �V�Ȉ�
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class Company : public TemplateObject
	{
		const Vec3 m_position;
		Mat4x4 m_spanMat;

		shared_ptr<Sprite> m_menuSprite;

		bool m_pushButton;
		bool m_spriteActive;

	public:
		Company(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3 position
		) :
			TemplateObject(stagePtr),
			m_position(position)
		{
			m_spanMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);
			m_pushButton = false;
			m_spriteActive = false;
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		/*
		* �{�^�������������̏���
		*/
		void ButtonPush();
		/*
		* �I�v�V������ʂ̏���
		*/
		void OptionMenu();
	};

	class CompanyCollision : public TemplateObject
	{
		const Vec3 m_scale = Vec3(15.0f, 2.0f, 8.0f);
		const Vec3 m_position;

		bool isPushButton;

	public:
		CompanyCollision(const shared_ptr<Stage>& stagePtr,
			const Vec3 pos) :
			TemplateObject(stagePtr),
			m_position(pos)
		{
			isPushButton = false;
		}

		virtual void OnCreate() override;
		void OnCollisionEnter(shared_ptr<GameObject>& object);
	};

	class Construction :public TemplateObject
	{
		const Vec3 m_position;
		Mat4x4 m_spanMat;

	public:
		Construction(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3 position
		) :
			TemplateObject(stagePtr),
			m_position(position)
		{
			m_spanMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		/*
		* �X�^�[�g�{�^�������������̏���
		*/
		void StartButtonPush();
		/*
		* �Q�[���X�^�[�g
		*/
		void Start();
	};
}