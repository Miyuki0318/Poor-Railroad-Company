/*!
@file MainCamera.h
@brief カメラ
@prod 矢吹悠葉
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class MainCamera : public Camera {
		const Vec3 m_DefaultEye;	// カメラの初期位置
		const Vec3 m_DefaultAt;		// カメラの初期注視点
		const Vec3 m_MaxEye;		// カメラの最大移動距離

		weak_ptr<GameObject> m_targetObject; // 目標となるオブジェクト
		shared_ptr<Transform> m_targetTrans; // 目標のトランスフォーム

		Vec3 m_targetPos;  // ターゲットの位置
		Vec3 m_currentEye; // カメラの現在位置
		Vec3 m_zoomEye;		// ズーム後の位置

		float m_zoomRatio; // ズームの割合
		float m_zoomSpeed; // ズーム速度

	public:
		enum State {
			Fixed,	// 固定
			Follow,	// 追従
			Zoom	// ズーム
		};
		State m_cameraState;	// カメラの現在の状態
		const State m_DefaultState;	// カメラの初期状態

		/// <summary>
		/// カメラのコンストラクタ
		/// </summary>
		/// <param name="cameraState">カメラの初期状態</param>
		MainCamera(State cameraState) : Camera(),
			m_cameraState(cameraState),
			m_DefaultState(cameraState),
			m_targetPos(Vec3(0.0f)),
			m_DefaultEye(Vec3(3.0f, 20.0f, -22.0f)),
			m_DefaultAt(Vec3(3.0f, 1.0f, -7.0f)),
			m_MaxEye(Vec3(400.0f, 20.0f, -22.0f)),
			m_zoomEye(Vec3(0.0f)),
			m_zoomRatio(0.0f),
			m_zoomSpeed(0.8f)
		{
		}
		/// <summary>
		/// カメラのコンストラクタ
		/// </summary>
		/// <param name="cameraState">カメラの初期状態</param>
		MainCamera(State cameraState, const Vec3& eyePos, const Vec3& atPos) : Camera(),
			m_cameraState(cameraState),
			m_DefaultState(cameraState),
			m_targetPos(Vec3(0.0f)),
			m_DefaultEye(eyePos),
			m_DefaultAt(atPos),
			m_MaxEye(Vec3(400.0f, 20.0f, -22.0f)),
			m_zoomEye(Vec3(0.0f)),
			m_zoomRatio(0.0f),
			m_zoomSpeed(0.8f)
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
		/// <param name="currentEye">現在の位置</param>
		/// <param name="zoomEye">ズーム後の位置</param>
		void ZoomStart(Vec3 currentEye, Vec3 zoomEye) {
			m_currentEye = currentEye;
			m_zoomEye = zoomEye;
			m_cameraState = Zoom;
		}

		/// <summary>
		/// ズーム処理終了時に呼び出す処理
		/// </summary>
		void ZoomEnd()
		{
			SetEye(m_DefaultEye);
			SetAt(m_DefaultAt);
			m_cameraState = Fixed;
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