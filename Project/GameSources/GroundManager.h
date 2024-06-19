/*!
@file GroundManager.h
@brief 地面管理オブジェクトクラス
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief 地面用のインスタンス描画オブジェクト
	*/
	class InstanceGround : public GameObject
	{
		// 描画コンポーネント
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw;
		
		// 地面の色
		const wstring m_groundTexture;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param 設定する色
		*/
		InstanceGround(const shared_ptr<Stage>& stagePtr,
			const wstring& texture
		) :
			GameObject(stagePtr),
			m_groundTexture(texture)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~InstanceGround() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief 地面描画追加関数
		@param 追加する行列
		*/
		void AddMatrix(const Mat4x4 matrix)
		{
			if (!m_ptrDraw) return;
			m_ptrDraw->AddMatrix(matrix);
		}
	};

	/*!
	@brief 地面描画オブジェクト管理クラス
	*/
	class GroundManager : public GameObject
	{
		float m_defPosY;		// インスタンス描画用のポジションY
		const Mat4x4 m_mtxScale;	// インスタンス描画用のスケール
		const Mat4x4 m_mtxRotation;	// インスタンス描画用のローテーション
		const int m_windowRange;	// 画面の範囲

		// インスタンス描画オブジェクトマップ
		map<eStageID, map<int, weak_ptr<InstanceGround>>> m_groundMap;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		GroundManager(const shared_ptr<Stage>& stagePtr, const float scale) :
			GameObject(stagePtr),
			m_defPosY(1.0f - (scale / 2.0f)),
			m_windowRange(30),
			m_mtxScale((Mat4x4)XMMatrixScalingFromVector(Vec3(1.0f, scale, 1.0f))),
			m_mtxRotation((Mat4x4)XMMatrixRotationRollPitchYawFromVector(Vec3(0.0f)))
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~GroundManager() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;
	};
}