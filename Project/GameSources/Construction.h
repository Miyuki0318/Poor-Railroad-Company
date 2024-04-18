/*!
@file Construction.h
@brief 工事現場の設定(ゲームスタート)
@author 新妻葵
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class Construction :public TemplateObject
	{
		const Vec3 m_position;
		Mat4x4 m_spanMat;

		shared_ptr<Transform> transComp;
		shared_ptr<PNTStaticDraw> drawComp;

		shared_ptr<Sprite> m_fadeSprite;
		bool isPushButton;

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
			isPushButton = false;
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
		void StartCountDown();
	};

	class ConstructionCollision : public TemplateObject
	{
		const Vec3 m_scale = Vec3(5.0f, 2.0f, 5.0f);
		const Vec3 m_position;

	public:
		bool m_playerHit;

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
		virtual void OnCollisionExit(shared_ptr<GameObject>& object);

		bool GetPlayerHitFlag()
		{
			return m_playerHit;
		}
	};
}