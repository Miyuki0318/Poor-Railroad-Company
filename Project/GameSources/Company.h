/*!
@file Company.h
@brief 会社の設定(メニュー画面)
@author 新妻葵
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class Company : public TemplateObject
	{
		const Vec3 m_position = Vec3(35.0f, 1.0f, 10.0f);
		Mat4x4 m_spanMat;

		shared_ptr<Transform> transComp;
		shared_ptr<PNTStaticDraw> drawComp;

		shared_ptr<Sprite> m_menuSprite;

		bool m_pushButton;
		bool m_spriteActive;

	public:
		Company(const shared_ptr<Stage>& stagePtr // ステージのポインタ
		) :
			TemplateObject(stagePtr)
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
		const Vec3 m_scale = Vec3(15.0f, 3.0f, 8.0f);
		const Vec3 m_position;


	public:
		bool m_playerHit;

		CompanyCollision(const shared_ptr<Stage>& stagePtr,
			const Vec3 pos) :
			TemplateObject(stagePtr),
			m_position(pos)
		{
			m_playerHit = false;
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& object);
		virtual void OnCollisionExit(shared_ptr<GameObject>& object);

		bool GetPlayerHitFlag()
		{
			return m_playerHit;
		}
	};
}