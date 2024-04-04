/*!
@file MainCamera.h
@brief カメラ
@prod 矢吹悠葉
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class MainCamera : public Camera {
		weak_ptr<GameObject> m_targetObject;	// 目標となるオブジェクト
		shared_ptr<Transform> m_targetTrans;	// 目標のトランスフォーム

	public:
		// コンストラクタとデストラクタ
		MainCamera() : Camera()
		{
		}
		~MainCamera() {}

		void OnCreate() override;
		void OnUpdate() override;

		// カメラが追尾するオブジェクトを取得する関数
		shared_ptr<GameObject> GetTargetObject() const {
			if (!m_targetObject.expired()) {
				return m_targetObject.lock();
			}
			return nullptr;
		}
		// カメラが追尾するオブジェクトを設定する関数
		void SetTargetObject(const shared_ptr<GameObject>& Obj) {
			m_targetObject = Obj;
		}
	};
}