/*!
@file UnBreakRock.h
@brief 破壊できない岩描画オブジェクト
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	// 壊せない岩のメッシュタイプenum
	enum class eRockMeshType : int
	{
		Mesh1,	// メッシュタイプ1
		Mesh2,	// メッシュタイプ2
		Mesh3,	// メッシュタイプ3
	};

	// 壊せない岩に使うデータ用
	namespace UnBreakRockData
	{
		// 壊せない岩のメッシュキーマップ
		static map<eRockMeshType, wstring> UnBreakRockMeshMap = {
			{eRockMeshType::Mesh1, L"UN_BREAK_ROCK1"},	// タイプ1
			{eRockMeshType::Mesh2, L"UN_BREAK_ROCK2"},	// タイプ2
			{eRockMeshType::Mesh3, L"UN_BREAK_ROCK3"},	// タイプ3
		};
	}

	/*!
	@brief インスタンス描画する壊せない岩
	*/
	class InstanceRock : public GameObject
	{
		// 描画コンポーネント
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw;

		// メッシュのキー
		const eRockMeshType m_meshType;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param 設定するメッシュのキー
		*/
		InstanceRock(const shared_ptr<Stage>& stagePtr,
			const eRockMeshType& meshType
		) :
			GameObject(stagePtr),
			m_meshType(meshType)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~InstanceRock() {}

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

		/*!
		@brief 地面描画全削除関数
		*/
		void ClearMatrix() const
		{
			if (!m_ptrDraw) return;
			m_ptrDraw->ClearMatrixVec();
		}
	};

	/*!
	@brief 壊せない岩描画生成オブジェクト
	*/
	class UnBreakRock : public TemplateObject
	{
		Mat4x4 m_mtxScale;		// インスタンス描画用のスケール
		Mat4x4 m_mtxRotation;	// インスタンス描画用のローテーション
		Mat4x4 m_mtxPosition;	// インスタンス描画用のローテーション

		// インスタンス描画マップ
		map<eRockMeshType, weak_ptr<InstanceRock>> m_instanceMap;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		UnBreakRock(const shared_ptr<Stage>& stagePtr
		) :
			TemplateObject(stagePtr)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~UnBreakRock() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 壊れない岩描画生成関数
		*/
		void CreateUnBreakRock();
	};
}