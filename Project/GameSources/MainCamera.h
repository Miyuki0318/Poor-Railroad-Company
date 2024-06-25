/*!
@file MainCamera.h
@brief カメラ
@prod 矢吹悠葉
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class MainCamera : public Camera {
		const Vec3 m_MaxEye;		// カメラの最大移動距離
		const float m_ZoomRatioC;	// 固定状態時のズーム割合

		weak_ptr<GameObject> m_targetObject; // 目標となるオブジェクト
		shared_ptr<Transform> m_targetTrans; // 目標のトランスフォーム

		Vec3 m_targetPos;	// ターゲットの位置
		Vec3 m_zoomEye;		// ズーム後の位置
		Vec3 m_currentEye;	// ズームのスタート位置
		Vec3 m_initialEye;	// カメラの初期位置
		Vec3 m_initialAt;	// カメラの初期注視点

		float m_zoomRatio; // ズームの割合
		float m_zoomSpeed; // ズーム速度

	public:
		enum State {
			Fixed,	// 固定
			Follow,	// 追従
			ZoomIn,	// ズームイン
			ZoomOut	// ズームアウト
		};
		State m_cameraState;	// カメラの現在の状態
		const State m_DefaultState;	// カメラの初期状態

		/// <summary>
		/// カメラのコンストラクタ
		/// </summary>
		/// <param name="cameraState">カメラの初期状態</param>
		MainCamera(State cameraState, const Vec3& eyePos, const Vec3& atPos) : Camera(),
			m_cameraState(cameraState),
			m_DefaultState(cameraState),
			m_targetPos(Vec3(0.0f)),
			m_initialEye(eyePos),
			m_initialAt(atPos),
			m_MaxEye(Vec3(400.0f, 20.0f, -22.0f)),
			m_currentEye(0.0f),
			m_ZoomRatioC(0.6f),
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
		void ZoomProcess();

		void ResetCamera(Vec3 eyePos, Vec3 atPos);

		/// <summary>
		/// ズーム処理スタート時に呼び出す処理
		/// </summary>
		/// <param name="currentEye">現在の位置</param>
		/// <param name="zoomEye">ズーム後の位置</param>
		void ZoomStart(Vec3 zoomEye) {
			m_zoomEye = zoomEye;
			m_currentEye = GetEye();
			m_cameraState = ZoomIn;
			m_zoomRatio = 0.0f;
		}
		void ZoomStart(Vec3 zoomEye, Vec3 defAt) {
			m_zoomEye = zoomEye;
			m_initialAt = defAt;
			m_currentEye = GetEye();
			m_cameraState = ZoomIn;
			m_zoomRatio = 0.0f;
		}

		/// <summary>
		/// ズーム処理終了時に呼び出す処理
		/// </summary>
		void ZoomEnd()
		{
			m_cameraState = ZoomOut;
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