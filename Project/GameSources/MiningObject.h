/*!
@file MiningObject.h
@brief 
@author 小宅碧
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class MiningObject : public  TemplateObject {
	public:
		// 状態管理用定数
		enum class eState
		{
			Normal, //通常の状態
			Damage, // 損傷した状態
			Broken, // 破壊された状態
			None // 消滅した状態
		};

	protected:
		// 生成座標
		const Vec3 m_spawnPos;
		// 採掘した回数
		int m_miningCount;
		// 採掘回数の上限
		const int m_miningLimit;
		// 状態を保持する変数
		eState m_state;
		// CSV上の位置
		Point2D<size_t> m_csvPos;

	public:
		// コンストラクタ
		MiningObject::MiningObject(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const Vec3 position,// 初期座標
			const int miningLimit
		) :
			TemplateObject(stagePtr), // ステージのポインタ
			m_spawnPos(Vec3(position.x, 1.0f, position.z)),// 初期座標
			m_miningLimit(miningLimit)
		{
			// 変数の初期化
			m_miningCount = 0;
			m_state = eState::Normal;
		}

		/*!
		@brief	 クラスが最初に作成される時に実行される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief	 採掘時に呼び出される関数
		*/
		virtual void OnMining();

		/*
		@brief	 オブジェクトをリセットする関数
		*/
		virtual void OnReset() = 0;

		/*
		@brief	 オブジェクトの情報をステージマップから消去する関数
		*/
		virtual void OnDelete();
		
		/*
		@brief	 CSV上のRowとColを返す関数
		@return　 m_csvPos
		*/
		virtual Point2D<size_t> GetCSVPos() const
		{
			return m_csvPos;
		}
	};


	class Tree : public MiningObject {
		// トランスフォームとモデルの差分行列
		Mat4x4 m_modelMat;
	public:
		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param 初期座標
		@param 採掘回数上限
		*/
		Tree::Tree(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const Vec3 position, // 初期座標
			const int miningLimit 	// 採掘回数上限
		) :
			MiningObject(stagePtr, position, miningLimit) // ステージのポインタ
		{
			// トランスフォームとモデルの差分行列を代入
			m_modelMat.affineTransformation(
				Vec3(0.08f, 0.09f, 0.08f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param 初期座標
		*/
		Tree::Tree(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const Vec3 position // 初期座標
		) :
			MiningObject(stagePtr, position, 2) // ステージのポインタ
		{
			// トランスフォームとモデルの差分行列を代入
			m_modelMat.affineTransformation(
				Vec3(0.08f, 0.09f, 0.08f),
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

		/*
		@brief	 ステートに応じて処理を行う関数
		*/
		void AccordingState();

	};



	class Rock : public MiningObject {
		// トランスフォームとモデルの差分行列
		Mat4x4 m_modelMat;
	public:
		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param 初期座標
		@param 採掘回数上限
		*/
		Rock::Rock(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const Vec3 position, // 初期座標
			const int miningLimit 	// 採掘回数上限
		) :
			MiningObject(stagePtr, position, miningLimit) // ステージのポインタ
		{
			// トランスフォームとモデルの差分行列を代入
			m_modelMat.affineTransformation(
				Vec3(0.28f, 0.28f, 0.28f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);
		}

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param 初期座標
		*/
		Rock::Rock(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const Vec3 position // 初期座標
		) :
			MiningObject(stagePtr, position, 2) // ステージのポインタ
		{
			// トランスフォームとモデルの差分行列を代入
			m_modelMat.affineTransformation(
				Vec3(0.28f, 0.28f, 0.28f),
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

		/*
		@brief	 ステートに応じて処理を行う関数
		*/
		void AccordingState();

	};
}