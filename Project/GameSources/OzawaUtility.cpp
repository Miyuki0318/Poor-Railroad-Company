/*!
@file OzawaUtility.cpp
@brief ユーティリティ
@author 小澤博貴
*/

#include "stdafx.h"
#include "OzawaUtility.h"

namespace Utility
{
	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	bool GetBetween(int value, int a, int b)
	{
		return a > b ? value <= a && value >= b : value <= b && value >= a;
	}

	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	bool GetBetween(float value, float a, float b)
	{
		return a > b ? value <= a && value >= b : value <= b && value >= a;
	}

	/*!
	@brief アルファベット大文字化
	@param 変更前文字列
	@return 変更後文字列
	*/
	string ToUpperString(string str)
	{
		transform(str.begin(), str.end(), str.begin(), ::toupper);
		return str;
	}

	/*!
	@brief アルファベット大文字化
	@param 変更前文字列
	@return 変更後文字列
	*/
	wstring ToUpperString(wstring str)
	{
		transform(str.begin(), str.end(), str.begin(), ::towupper);
		return str;
	}

	/*!
	@brief アルファベット小文字化
	@param 変更前文字列
	@return 変更後文字列
	*/
	string ToLowerString(string str)
	{
		transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}

	/*!
	@brief アルファベット小文字化
	@param 変更前文字列
	@return 変更後文字列
	*/
	wstring ToLowerString(wstring str)
	{
		transform(str.begin(), str.end(), str.begin(), ::towlower);
		return str;
	}

#if BASECROSS

	/*!
	@brief シンプルな頂点データを作成
	@param (vertices) 頂点データ
	*/
	void SimpleVertices(vector<basecross::VertexPositionColorTexture>& vertices, const Col4& color)
	{
		// 半分のサイズ
		const float HALF_SIZE = 0.5f;

		// 初期化
		vertices.clear();

		// 頂点データ
		vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), color, Vec2(0.0f, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), color , Vec2(1.0f, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), color, Vec2(0.0f, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), color, Vec2(1.0f, 1.0f)},
		};
	}

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	*/
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices)
	{
		// シンプルな頂点データの作成
		SimpleVertices(vertices);

		// 頂点の並び順
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex)
	{
		// シンプルな頂点データの作成
		SimpleVertices(vertex.vertices);

		// 頂点の並び順
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (color) 固定色
	*/
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Col4& color)
	{
		// シンプルな頂点データの作成
		SimpleVertices(vertices, color);

		// 頂点の並び順
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex, const Col4& color)
	{
		// シンプルな頂点データの作成
		SimpleVertices(vertex.vertices, color);

		// 頂点の並び順
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief 数字用頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (number) 数字
	*/
	void NumberVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int number)
	{
		// 半分のサイズとデフォルトの色
		const float HALF_SIZE = 0.5f;
		const float UV_LEFT = 1.0f / 10.0f * number;
		const float UV_RIGHT = 1.0f / 10.0f * (number + 1);
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// 頂点データ
		vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_LEFT, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(UV_LEFT, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
		};

		// 頂点の並び順
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief 数字用頂点データと頂点インデックスを作成
	@param (vertex) 頂点データと頂点インデックス構造体
	@param (number) 数字
	*/
	void NumberVerticesIndices(basecross::VertexData& vertex, const int number)
	{
		// 半分のサイズとデフォルトの色
		const float HALF_SIZE = 0.5f;
		const float UV_LEFT = 1.0f / 10.0f * number;
		const float UV_RIGHT = 1.0f / 10.0f * (number + 1);
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// 頂点データ
		vertex.vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_LEFT, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(UV_LEFT, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
		};

		// 頂点の並び順
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief リボン型頂点データと頂点インデックスを作成
	@param (point) 節目の座標配列
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (axisVec) 直行軸ベクトル
	@param (weight) 幅
	@param (texLoop) テクスチャループ
	@return 生成出来たかの真偽
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Vec3& axisVec, float weight, int texLoop)
	{
		// 節目の数が0なら終了
		const int& size = static_cast<int>(point.size()); 
		if (size == 0) return false;

		vertices.clear(); // 頂点座標の初期化

		// 節目の数ループ
		for (int i = 0; i < size; i++)
		{
			// テクスチャをループするか
			const int loop = texLoop > 0 ? texLoop : 1;

			int front = (i - 1) < 0 ? size - 1 : i - 1; // 手前
			int rear = (i + 1) % size; // 後方

			// 方向から外積を求める
			Vec3 dir = point.at(front) - point.at(rear);
			Vec3 cross = dir.cross(axisVec).normalize();
		
			// 幅分を差し引いて座標を頂点として追加
			Vec3 pos = point.at(i) - cross * weight;
			basecross::VertexPositionColorTexture vertexLeft(pos, COL_WHITE, Vec2(0.0f, static_cast<float>(i) / size * loop));
			vertices.push_back(vertexLeft);

			// 幅分を加算した座標を頂点として追加
			pos = point.at(i) + cross * weight;
			basecross::VertexPositionColorTexture vertexRight(pos, COL_WHITE, Vec2(1.0f, static_cast<float>(i) / size * loop));
			vertices.push_back(vertexRight);
		}

		// 頂点インデックスの元
		const vector<uint16_t> baseIndices = {
			0, 1, 2,
			2, 1, 3
		};

		indices.clear(); // インデックスの初期化

		// サイズ分より一回少ない数ループ
		for (int i = 0; i < size - 1; i++)
		{
			// 元サイズ分ループ
			for (int j = 0; j < baseIndices.size(); j++)
			{
				// 元のインデックスに2の倍数を加算させて追加
				indices.push_back(baseIndices.at(j) + (2 * i));
			}
		}

		return true; // 生成出来たのでtrueを返す
	}

	/*!
	@brief リボン型頂点データと頂点インデックスを作成
	@param (point) 節目の座標配列
	@param (vertex) 頂点データと頂点インデックス
	@param (axisVec) 直行軸ベクトル
	@param (weight) 幅
	@param (texLoop) テクスチャループ
	@return 生成出来たかの真偽
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, basecross::VertexData& vertex, const Vec3& axisVec, float weight, int texLoop)
	{
		// 頂点データ構造体を分解して生成関数を実行
		return RibonVerticesIndices(point, vertex.vertices, vertex.indices, axisVec, weight, texLoop);
	}

	/*!
	@brief 円型用頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (number) 数字
	*/
	void CircleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int squareNum)
	{
		const float HELF_SIZE = 0.5f;
		const Col4 DEFAULT_COLOR = Col4(1.0f);
		
		vertices.clear();
		indices.clear();

		basecross::VertexPositionColorTexture first(Vec3(0.0f), DEFAULT_COLOR, Vec2(HELF_SIZE));
		vertices.push_back(first);

		for (int i = 0; i < squareNum + 1; i++)
		{
			float rad = (XM_2PI / squareNum * i);
			float x = cosf(rad) * HELF_SIZE;
			float y = sinf(rad) * HELF_SIZE;
			Vec3 position = Vec3(x, y, 0.0f);
			Vec2 uvPos = Vec2(HELF_SIZE) + Vec2(x, -y);

			basecross::VertexPositionColorTexture v(position, DEFAULT_COLOR, uvPos);
			vertices.push_back(v);
		}

		for (int i = 0; i < squareNum; i++)
		{
			// 3の倍数を基準に設定
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
		}
	}

	/*!
	@brief 円型用頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (number) 数字
	*/
	void CircleVerticesIndices(basecross::VertexData& vertex, const int squareNum)
	{
		CircleVerticesIndices(vertex.vertices, vertex.indices, squareNum);
	}

	/*!
	@brief ワールド座標をスクリーン座標に変換(BaseCrossの座標用、それ以外はDirectX付属の関数を備考)
	@param (viewPtr) viewのshared_ptr
	@param (position) ワールド座標
	@return 変換されたスクリーン座標
	*/
	Vec3 ConvertToWorldPosition(const shared_ptr<basecross::ViewBase>& viewPtr, const Vec3& position)
	{
		// ビューからカメラとビューポートを取得
		const auto& ptrCamera = viewPtr->GetTargetCamera();
		const auto& viewPort = viewPtr->GetTargetViewport();

		// マトリックスの定義
		const Mat4x4 viewMat = ptrCamera->GetViewMatrix();
		const Mat4x4 projMat = ptrCamera->GetProjMatrix();
		Mat4x4 worldMat;
		worldMat.affineTransformation(
			Vec3(1.0f),
			Vec3(0.0f),
			Vec3(0.0f),
			Vec3(0.0f)
		);

		// 3D座標を2D座標に変換(左上0,0基準)
		XMVECTOR xmWinPos = DirectX::XMVector3Project(
			position,
			viewPort.TopLeftX,
			viewPort.TopLeftY,
			viewPort.Width,
			viewPort.Height,
			viewPort.MinDepth,
			viewPort.MaxDepth,
			projMat,
			viewMat,
			worldMat
		);

		// BaseCross用に変換(中央0,0基準)
		Vec3 windowPos = Vec3(xmWinPos);
		windowPos.x -= static_cast<float>(viewPort.Width) / 2.0f;
		windowPos.y -= static_cast<float>(viewPort.Height) / 2.0f;
		windowPos.y *= -1.0f;

		// 変換された座標
		return windowPos;
	}

	/*!
	@brief ビルボードをクォータニオンで作成し返す
	@param (Line)　カメラの注視点 - カメラの位置
	@return 作成されたクォータニオン
	*/
	Quat GetBillboardQuat(const Vec3& Line)
	{
		// 線分のコピー
		Vec3 line = Line;
		Vec3 defUp(UP_VEC);
		Vec2 lineVec(line.x, line.z);

		// 前後左右の長さが0.1f以下なら回転軸を変更
		if (lineVec.length() < 0.1f) defUp = BACK_VEC;
		line.normalize();

		// マトリックスの計算
		Mat4x4 rotMatrix = (Mat4x4)XMMatrixLookAtLH(Vec3(0.0f), line, defUp);
		rotMatrix.inverse();

		// クォータニオンに変換し、正規化
		Quat qt = rotMatrix.quatInMatrix();
		qt.normalize();
		return qt;
	}

	/*!
	@brief ビルボード状態から回転させたクォータニオンで作成し返す
	@param (cameraPos)　カメラの位置
	@param (objPos)　オブジェクトの位置
	@param (rotateVec)　回転量(Vec3)
	@return 作成されたクォータニオン
	*/
	Quat GetBillboardRotateQuat(const Vec3& Line, Vec3& rotateVec)
	{
		// 線分の計算
		Vec3 line = Line;
		Vec3 defUp(UP_VEC);
		Vec2 lineVec(line.x, line.z);

		// 前後左右の長さが0.1f以下なら回転軸を変更
		if (lineVec.length() < 0.1f) defUp = BACK_VEC;
		line.normalize();

		// 回転量が360度を超えてたら0度にする
		if (rotateVec.x >= XM_2PI) rotateVec.x -= XM_2PI;
		if (rotateVec.y >= XM_2PI) rotateVec.y -= XM_2PI;
		if (rotateVec.z >= XM_2PI) rotateVec.z -= XM_2PI;

		// マトリックスの計算
		Mat4x4 rotMatrix = (Mat4x4)XMMatrixLookAtLH(Vec3(0.0f), line, defUp);
		rotMatrix *= (Mat4x4)XMMatrixRotationRollPitchYawFromVector(rotateVec);
		rotMatrix.inverse();

		// 軸回転させたクォータニオンを返す
		return rotMatrix.quatInMatrix();
	}

	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	bool GetBetween(const Vec2& value, const Vec2& a, const Vec2& b)
	{
		return GetBetween(value.x, a.x, b.x) && GetBetween(value.y, a.y, b.y);
	}

	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	bool GetBetween(const Vec3& value, const Vec3& a, const Vec3& b)
	{
		return GetBetween(value.x, a.x, b.x) && GetBetween(value.y, a.y, b.y) && GetBetween(value.z, a.z, b.z);
	}

	float rotYatan2f(const Vec3& A, const Vec3& B)
	{
		return atan2f(B.z - A.z, B.x - A.x);
	}

	namespace CohenClip
	{
		const float xmin = -960.0f;
		const float xmax = 960.0f;
		const float ymin = -540.0f;
		const float ymax = 540.0f;

		// 座標のコード計算
		int ComputeOutCode(float x, float y) 
		{
			int code = INSIDE;

			if (x < xmin) code |= LEFT;
			else if (x > xmax) code |= RIGHT;

			if (y < ymin) code |= BOTTOM;				
			else if (y > ymax) code |= TOP;
				
			return code;
		}
	}

	Vec3 WindowClipLineVec(const Vec3& center, const Vec3& target)
	{
		float x0 = center.x;
		float y0 = center.y;
		float x1 = target.x;
		float y1 = target.y;

		int outcode0 = CohenClip::ComputeOutCode(center.x, center.y);
		int outcode1 = CohenClip::ComputeOutCode(target.x, target.y);
		bool accept = false;

		while (true) 
		{
			if (!(outcode0 | outcode1)) 
			{
				// 両端点がウィンドウ内にある場合
				accept = true;
				break;
			}
			else if (outcode0 & outcode1)
			{
				// 両端点がウィンドウの外側の同じ領域にある場合
				break;
			}
			else 
			{
				// クリッピングする必要がある場合
				Vec3 clipVec;
				int outcodeOut = outcode0 ? outcode0 : outcode1;
				
				// 上側に交差
				if (outcodeOut & CohenClip::TOP) 
				{           
					clipVec.x = x0 + (x1 - x0) * (CohenClip::ymax - y0) / (y1 - y0);
					clipVec.y = CohenClip::ymax;
				} 
				else if (outcodeOut & CohenClip::BOTTOM) // 下側に交差
				{
					clipVec.x = x0 + (x1 - x0) * (CohenClip::ymin - y0) / (y1 - y0);
					clipVec.y = CohenClip::ymin;
				}
				else if (outcodeOut & CohenClip::RIGHT) // 右側に交差
				{  
					clipVec.y = y0 + (y1 - y0) * (CohenClip::xmax - x0) / (x1 - x0);
					clipVec.x = CohenClip::xmax;
				}
				else if (outcodeOut & CohenClip::LEFT) // 左側に交差
				{   
					clipVec.y = y0 + (y1 - y0) * (CohenClip::xmin - x0) / (x1 - x0);
					clipVec.x = CohenClip::xmin;
				}

				// 交差点をクリップされた端点として設定
				if (outcodeOut == outcode0) 
				{
					x0 = clipVec.x;
					y0 = clipVec.y;
					outcode0 = CohenClip::ComputeOutCode(x0, y0);
				}
				else
				{
					x1 = clipVec.x;
					y1 = clipVec.y;
					outcode1 = CohenClip::ComputeOutCode(x1, y1);
				}
			}
		}

		if (accept)
		{
			return Vec3(x1, y1, target.z);
		}
		else 
		{
			return target; // クリッピングに失敗した場合も元の座標を返す
		}
	}

#endif
}