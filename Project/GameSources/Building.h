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
		/*
		* オブジェクトと接触している時の処理
		*/
		void OnCollisionEnter(shared_ptr<GameObject>& object);
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
}