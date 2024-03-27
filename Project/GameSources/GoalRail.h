/*!
@file GoalRail.h
@brief ゴール地点オブジェクト
@author 小宅碧
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {
	class GoalRail : public TemplateObject
	{
		// 自身のドローコンポーネントのポインタ
		shared_ptr<PNTStaticDraw> m_ptrDraw;
		// 自身のコリジョンのポインタ
		shared_ptr<CollisionObb> m_ptrColl;
		// 開始時だけ処理させたい関数用のbool
		bool m_setFlg;

	public:

		GoalRail(const shared_ptr<Stage>& stagePtr, const Vec3& position) :
			TemplateObject(stagePtr, position, Vec3(0.0f), Vec3(1.0f, 0.2f, 1.0f))
		{
			// 変数の初期化
			m_setFlg = true;
		}

		~GoalRail() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief	 他のコリジョンと衝突した時に実行される関数
		@param[in]	Other 衝突したオブジェクトのポインタ
		*/
		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

	};
}
