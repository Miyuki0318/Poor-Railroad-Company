/*!
@file TitlePlayer.h
@brief タイトル用プレイヤーオブジェクト
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief タイトル用プレイヤー
	*/
	class TitlePlayer : public TemplateObject
	{
		shared_ptr<BcPNTBoneModelDraw> m_ptrDraw; // 描画コンポーネント

		Vec3 m_rotTarget;	// 回転先
		Vec3 m_currentRot;  // 前回の回転軸
		Mat4x4 m_modelMat;	// モデルとトランスフォーム差分行列

		const float m_moveSpeed; // 移動速度
		const float m_rotSpeed;  // 回転速度

	public:
		
		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		TitlePlayer(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 3.0f, 0.0f), Vec3(0.0f), Vec3(1.0f)),
			m_moveSpeed(5.0f), // 今後CSVから速度等のステータスを取得予定
			m_rotSpeed(0.5f)  // 今後CSVから速度等のステータスを取得予定
		{
			m_rotTarget.zero(); // 回転先は0.0fで初期化
			m_currentRot.zero(); // 回転先は0.0fで初期化

			// スケールだけ、Y軸方向に2倍にする
			m_modelMat.affineTransformation(
				Vec3(0.6f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(0.0f, -1.0f, 0.0f)
			);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~TitlePlayer() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

	private:

		/*!
		@brief 移動更新関数
		*/
		void UpdateMove();

		/*!
		@brief 回転更新関数
		*/
		void UpdateRotation();

		/*!
		@brief 回転先設定関数
		@param Lスティック入力量
		*/
		void SetRotateTarget(const Vec3& stickValue);

		/*!
		@brief コントローラー移動関数
		@param Lスティック入力量
		*/
		void ControllerMovement(const Vec3& stickValue);
	};
}