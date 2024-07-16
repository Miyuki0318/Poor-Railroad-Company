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
		const float m_ScrollSpeed;	// スクロールスピード
		const float m_ZoomRatioC;	// 固定状態時のズーム割合

		weak_ptr<GameObject> m_targetObject; // 目標となるオブジェクト
		shared_ptr<Transform> m_targetTrans; // 目標のトランスフォーム

		Vec3 m_targetPos;	// ターゲットの位置
		Vec3 m_zoomEye;		// ズーム後の位置
		Vec3 m_currentEye;	// ズームのスタート位置
		Vec3 m_defScrollEye;// カメラの初期位置(スクロール前)
		Vec3 m_initialEye;	// カメラの初期位置
		Vec3 m_initialAt;	// カメラの初期注視点

		float m_zoomAtY;	// ズーム後の注視点
		float m_zoomRatio;	// ズームの割合
		float m_zoomSpeed;	// ズーム速度
		float m_scrollRatio; // スクロール割合

	public:
		enum State {
			Fixed,	// 固定
			Follow,	// 追従
			ZoomIn,	// ズームイン
			ZoomOut, // ズームアウト
			ZoomedIn, // ズームイン後
			Scroll,	// スクロール
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
			m_defScrollEye(eyePos),
			m_targetPos(Vec3(0.0f)),
			m_initialEye(m_defScrollEye),
			m_initialAt(atPos),
			m_MaxEye(Vec3(400.0f, 20.0f, -22.0f)),
			m_currentEye(0.0f),
			m_ZoomRatioC(0.6f),
			m_ScrollSpeed(0.3f),
			m_zoomEye(Vec3(0.0f)),
			m_zoomAtY(0.0f),
			m_zoomRatio(0.0f),
			m_zoomSpeed(0.8f),
			m_scrollRatio(0.0f)
		{
		}
		/// <summary>
		/// カメラのコンストラクタ
		/// </summary>
		/// <param name="cameraState">カメラの初期状態</param>
		MainCamera(State cameraState, const Vec3& defEyePos, const Vec3& startEyePos, const Vec3& atPos) : Camera(),
			m_cameraState(cameraState),
			m_DefaultState(cameraState),
			m_defScrollEye(defEyePos),
			m_targetPos(Vec3(0.0f)),
			m_initialEye(startEyePos),
			m_initialAt(atPos),
			m_MaxEye(Vec3(400.0f, 20.0f, -22.0f)),
			m_currentEye(0.0f),
			m_ZoomRatioC(0.6f),
			m_ScrollSpeed(0.3f),
			m_zoomEye(Vec3(0.0f)),
			m_zoomAtY(0.0f),
			m_zoomRatio(0.0f),
			m_zoomSpeed(0.8f),
			m_scrollRatio(0.0f)
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
		/// ズームイン後の処理
		/// </summary>
		void ZoomedInProcess();

		/// <summary>
		/// ズーム処理
		/// </summary>
		void ZoomProcess();

		/// <summary>
		/// スクロール処理
		/// </summary>
		void ScrollProcess();

		/// <summary>
		/// カメラのリセット処理
		/// </summary>
		/// <param name="defEyePos">スクロール前の位置</param>
		/// <param name="startEyePos">プレイスタート時の位置</param>
		/// <param name="atPos">ゲーム中の注視点</param>
		void ResetCamera(Vec3 defEyePos, Vec3 startEyePos, Vec3 atPos);

		/// <summary>
		/// ズーム処理スタート時に呼び出す処理
		/// </summary>
		/// <param name="zoomEye">ズーム後の位置</param>
		void ZoomStart(Vec3 zoomEye) {
			m_zoomEye = zoomEye;
			m_currentEye = GetEye();
			m_cameraState = ZoomIn;
			m_zoomRatio = 0.0f;
		}

		/// <summary>
		/// ズーム処理スタート時に呼び出す処理
		/// </summary>
		/// <param name="zoomEye">ズーム後の位置</param>
		/// <param name="zoomAtY">ズーム後のY座標に加算する値</param>
		void ZoomStart(Vec3 zoomEye, float zoomAtY) {
			m_zoomEye = zoomEye;
			m_zoomAtY = zoomAtY;
			m_currentEye = GetEye();
			m_cameraState = ZoomIn;
			m_zoomRatio = 0.0f;
		}

		/// <summary>
		/// ズーム処理スタート時に呼び出す処理
		/// </summary>
		/// <param name="zoomEye">ズーム後の位置</param>
		/// <param name="defAt">初期注視点</param>
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
			if (m_cameraState != ZoomedIn) return;
			m_cameraState = ZoomOut;
		}

		/// <summary>
		/// スクロール開始処理
		/// </summary>
		void ScrollStart()
		{
			m_cameraState = Scroll;
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

		/// <summary>
		/// スクロールが終了しているかどうか
		/// </summary>
		/// <returns>終了していたらtrue</returns>
		bool GetScrollEnd() const
		{
			return m_scrollRatio >= 1.0f;
		}
	};
}