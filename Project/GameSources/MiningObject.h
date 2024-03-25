/*!
@file MiningObject.h
@brief 
@author 小宅碧
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class MiningObject : public  TemplateObject {
	protected:
		// 生成座標
		const Vec3 m_spawnPos;
		// 採掘した回数
		int m_miningCount;
		// 開始時だけ処理させたい関数用のbool
		bool m_setFlg;

	public:
		// コンストラクタ
		MiningObject::MiningObject(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const Vec3 position // 初期座標
		) :
			TemplateObject(stagePtr), // ステージのポインタ
			m_spawnPos(position) // 初期座標
		{
			// 変数の初期化
			m_miningCount = 0;
			m_setFlg = true;
		}

		/*!
		@brief	 クラスが最初に作成される時に実行される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief	 採掘時に呼び出される関数
		*/
		virtual void OnMining() = 0;

		/*
		@brief	 オブジェクトをリセットする関数
		*/
		virtual void OnReset() = 0;
	};

	class Tree : public MiningObject {
		// 採掘回数の上限
		const int m_miningCountLimit;
		// トランスフォームとモデルの差分行列
		Mat4x4 m_modelMat;
	public:
		// コンストラクタ
		Tree::Tree(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const Vec3 position, // 初期座標
			const int miningCountLimit 	// 採掘回数上限
		) :
			MiningObject(stagePtr, position), // ステージのポインタ
			m_miningCountLimit(miningCountLimit) 	// 採掘回数上限
		{
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
		void OnMining() override;

		/*
		@brief	 オブジェクトをリセットする関数
		*/
		void OnReset() override;

	};




	class Rock : public MiningObject {
		// 採掘回数の上限
		const int m_miningCountLimit;
		// トランスフォームとモデルの差分行列
		Mat4x4 m_modelMat;
	public:
		// コンストラクタ
		Rock::Rock(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const Vec3 position, // 初期座標
			const int miningCountLimit 	// 採掘回数上限
		) :
			MiningObject(stagePtr, position), // ステージのポインタ
			m_miningCountLimit(miningCountLimit) 	// 採掘回数上限
		{
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
		void OnMining() override;

		/*
		@brief	 オブジェクトをリセットする関数
		*/
		void OnReset() override;

	};
}