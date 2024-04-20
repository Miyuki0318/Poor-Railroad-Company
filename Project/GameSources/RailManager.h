/*!
@file RailManager.h
@brief レール管理クラス
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

#define ROW(posZ) size_t(-posZ)	// 行
#define COL(posX) size_t(posX) // 列
#define LINE(row, col) to_string(row) + "-" + to_string(col)
#define LINE2POS(row, col) Vec3(float(col), 1.0f, -float(row))

namespace basecross
{
	enum class eNextRailDir
	{
		DirFlont,	// 前
		DirBack,	// 後
		DirLeft,	// 左
		DirRight,	// 右
	};

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

		/*!
		@brief ポイントの前後左右が配列の範囲内かを取得する関数
		@param row
		@param col
		@param csv配列
		*/
		static vector<CSVElementCheck> GetElemsCheck(size_t row, size_t col, const vector<vector<int>>& csvMap)
		{
			vector<CSVElementCheck> elems; // 前後左右の結果保存用配列
			if (csvMap.empty()) return elems; // 参照する配列が空なら空を返す

			elems = {
				{row - 1, col, Utility::WithInElemRange(row - 1, csvMap.size())},
				{row + 1, col, Utility::WithInElemRange(row + 1, csvMap.size())},
				{row, col - 1, Utility::WithInElemRange(col - 1, csvMap.at(row).size())},
				{row, col + 1, Utility::WithInElemRange(col - 1, csvMap.at(row).size())},
			};

			return elems;
		}
	};

	/*!
	@brief LINEマクロで保持されたRowCol文字列をsize_tに代入する関数
	@param row格納変数
	@param col格納変数
	@param line文字列
	*/
	void GetLineStringToRowCol(size_t& row, size_t& col, string line);

	/*!
	@brief レール管理用クラス
	*/
	class RailManager : public GameObject
	{
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw; // 描画コンポーネント
		vector<Point2D<size_t>> m_guidePoints; // ガイドがあるポイント
		vector<vector<int>> m_guideMap; // ガイド付きのステージcsv
		Mat4x4 m_mtxScale;		// インスタンス描画用のスケール
		Mat4x4 m_mtxRotation;	// インスタンス描画用のローテーション
		size_t m_railNum;
		map<string, Vec3> m_railMap;
		Vec3 m_pastDeRailPos;
		
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		RailManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
			m_railNum = 0;

			Quat quatRot;
			quatRot.rotationRollPitchYawFromVector(Vec3(0.0f, XM_PIDIV2, 0.0f));
			m_mtxRotation.rotation(quatRot);
			m_mtxScale.scale(Vec3(0.675f));
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
		@brief ガイドを再計算する関数
		*/
		void GuideRecalculation();

		/*!
		@brief ガイドのポイント取得関数
		@return m_guidePoints
		*/
		const vector<Point2D<size_t>>& GetGuidePoints() const
		{
			return m_guidePoints;
		}

		/*!
		@brief レールの座標配列取得関数
		@return m_railPositions
		*/
		const map<string, Vec3>& GetRailMap() const
		{
			return m_railMap;
		}

		/*!
		@brief ガイドつきcsv取得関数
		@return m_guideMap
		*/
		const vector<vector<int>>& GetGuideMap() const
		{
			return m_guideMap;
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
			Vec3 addPos = LINE2POS(row, col);

			// トランスフォーム行列の設定
			Mat4x4 matrix, mtxPosition;
			mtxPosition.translation(addPos);

			// 行列の設定と追加
			matrix = m_mtxScale * m_mtxRotation * mtxPosition;
			m_ptrDraw->AddMatrix(matrix);
			m_railMap.insert(make_pair(LINE(row, col), addPos));
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
	};
}