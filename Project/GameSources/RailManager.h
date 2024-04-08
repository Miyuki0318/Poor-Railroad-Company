/*!
@file RailManager.h
@brief レール管理クラス
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

#define ROW(posZ) size_t(-posZ)	// 行
#define COL(posX) size_t(posX) // 列

namespace basecross
{
	// CSVのチェック用構造体
	struct CSVElementCheck
	{
		size_t row = 0;	// 行
		size_t col = 0; // 列
		bool isRange = false; // 配列の範囲内かの真偽

		/*!
		@brief コンストラクタ
		@param row
		@param col
		@param isRange
		*/
		CSVElementCheck(size_t elemRow, size_t elemCol, bool range) :
			row(elemRow),
			col(elemCol),
			isRange(range)
		{
		}
	};

	/*!
	@brief レール管理用クラス
	*/
	class RailManager : public GameObject
	{
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw; // 描画コンポーネント
		vector<Point2D<size_t>> m_guidePoints; // ガイドがあるポイント
		vector<vector<int>> m_guideMap; // ガイド付きのステージcsv
		vector<Vec3> m_railPositions; // レールの座標配列
		Mat4x4 m_mtxScale;		// インスタンス描画用のスケール
		Mat4x4 m_mtxRotation;	// インスタンス描画用のローテーション
		
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		RailManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
			m_mtxScale.scale(Vec3(1.0f, 0.2f, 1.0f));
			m_mtxRotation.rotation(Quat(0.0f, 0.0f, 0.0f, 1.0f));
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~RailManager() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief レール追加関数
		@param 選択ポイント
		*/
		void AddRail(const Point2D<size_t>& point);

		/*!
		@brief 現在のポイントの位置がガイドになっているか取得関数
		@param 選択ポイント
		*/
		bool GetIsGuidePoint(const Point2D<size_t>& point) const;

		/*!
		@brief ガイドのポイント取得関数
		@return m_guidePoints
		*/
		vector<Point2D<size_t>> GetGuidePoints() const
		{
			return m_guidePoints;
		}

		/*!
		@brief レールの座標配列取得関数
		@return m_railPositions
		*/
		vector<Vec3> GetRailPositions() const
		{
			return m_railPositions;
		}

	private:

		/*!
		@brief インスタンス描画のレール追加関数
		@param row
		@param col
		*/
		void AddInstanceRail(size_t row, size_t col)
		{
			// 座標の設定
			Vec3 addPos = Vec3(float(col), 1.0f, -float(row));

			// トランスフォーム行列の設定
			Mat4x4 matrix, mtxPosition;
			mtxPosition.translation(addPos);

			// 行列の設定と追加
			matrix = m_mtxScale * m_mtxRotation * mtxPosition;
			m_ptrDraw->AddMatrix(matrix);
			m_railPositions.push_back(addPos);
		}

		/*!
		@brief CSVをレールIDに書き換える関数
		@param row
		@param col
		*/
		void SetRailID(size_t row, size_t col) const;

		/*!
		@brief CSVをガイドIDに書き換える関数
		@param row
		@param col
		*/
		void SetGuideID(size_t row, size_t col);

		/*!
		@brief ポイントの前後左右が配列の範囲内かを取得する関数
		@param row
		@param col
		@param csv配列
		*/
		vector<CSVElementCheck> GetElemsCheck(size_t row, size_t col, const vector<vector<int>>& csvMap) const;
	};
}