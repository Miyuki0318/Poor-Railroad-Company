/*!
@file Building.h
@brief 建物の設定
@author 新妻葵
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
		Company(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
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
		* ボタンを押した時の処理
		*/
		void ButtonPush();
		/*
		* オプション画面の処理
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
		virtual void OnCollisionEnter(shared_ptr<GameObject>& object);

		bool GetPlayerHitFlag()
		{
			return isPushButton;
		}
	};

	class Construction :public TemplateObject
	{
		const Vec3 m_position;
		Mat4x4 m_spanMat;

	public:
		Construction(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
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
		* スタートボタンを押した時の処理
		*/
		void StartButtonPush();
		/*
		* ゲームスタート
		*/
		void Start();
	};

	class ConstructionCollision : public TemplateObject
	{
		const Vec3 m_scale = Vec3(5.0f, 2.0f, 5.0f);
		const Vec3 m_position;

		bool m_playerHit;

	public:
		ConstructionCollision(shared_ptr<Stage>& stagePtr,
			const Vec3 pos
		) :
			TemplateObject(stagePtr),
			m_position(pos)
		{
			m_playerHit = false;
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& object);

		bool GetPlayerHitFlag()
		{
			return m_playerHit;
		}
	};
}