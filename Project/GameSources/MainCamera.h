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

		Vec3 m_targetPos;
		Vec3 m_cameraArm;
		Vec3 m_currentEye;

		float m_zoomRatio;
		float m_zoomSpeed;

	public:
		enum State {
			Fixed,	// 固定
			Follow,	// 追従
			Zoom	// ズーム
		};
		State m_cameraState;	// カメラの現在の状態
		State m_defaultState;	// カメラの初期状態

		/// <summary>
		/// カメラのコンストラクタ
		/// </summary>
		MainCamera() : Camera(),
			m_cameraState(Fixed),
			m_defaultState(Fixed),
			m_targetPos(Vec3(0.0f)),
			m_cameraArm(Vec3(0.0f, 20.0f, -22.0f)),
			m_zoomRatio(0.0f),
			m_zoomSpeed(1.0f)
		{
		}
		/// <summary>
		/// カメラのコンストラクタ
		/// </summary>
		/// <param name="cameraState">カメラの初期状態</param>
		MainCamera(State cameraState) : Camera(),
			m_cameraState(cameraState),
			m_defaultState(cameraState),
			m_targetPos(Vec3(0.0f)),
			m_cameraArm(Vec3(0.0f, 20.0f, -22.0f)),
			m_zoomRatio(0.0f),
			m_zoomSpeed(1.0f)
		{
		}
		~MainCamera() {}

		void OnCreate() override;
		void OnUpdate() override;

		/// <summary>
		/// 対象に追従する処理
		/// </summary>
		void FollowTarget();

		/// <summary>
		/// ズーム処理
		/// </summary>
		void ZoomInProcess();

		/// <summary>
		/// ズーム処理スタート時に呼び出す処理
		/// </summary>
		/// <param name="currentEye"></param>
		void ZoomStart(Vec3 currentEye) {
			m_currentEye = currentEye;
			m_cameraState = Zoom;
		}

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