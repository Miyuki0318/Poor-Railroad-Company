/*!
@file Station.h
@brief
@author 小宅碧
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class Station : public  TemplateObject {
	protected:
		// 生成座標
		const Vec3 m_spawnPos;
		// 自分自身のトランスフォームコンポーネント
		shared_ptr<Transform> m_transform;
		// 開始時だけ処理させたい関数用のbool
		bool m_setFlg;
		// トランスフォームとモデルの差分行列
		Mat4x4 m_modelMat;

	public:
		// コンストラクタ
		Station::Station(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const Vec3 position // 初期座標
		) :
			TemplateObject(stagePtr), // ステージのポインタ
			m_spawnPos(position) // 初期座標
		{
			// 変数の初期化
						// トランスフォームとモデルの差分行列を代入
			m_modelMat.affineTransformation(
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);

		}

		/*!
		@brief	 クラスが最初に作成される時に実行される関数
		*/
		void OnCreate() override;

		/*!
		@brief	 毎フレーム実行される関数
		*/
		void OnUpdate() override;

		/*!
		@brief	 採掘時に呼び出される関数
		*/
		void OnMining();

		/*
		@brief	 オブジェクトをリセットする関数
		*/
		void OnReset();
	};
}