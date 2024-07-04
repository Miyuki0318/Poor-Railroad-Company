/*!
@file Billboard.cpp
@brief ビルボードオブジェクトの継承元
@author 小澤博貴
*/

#include "stdafx.h"
#include "Billboard.h"

namespace basecross
{
	/*!
	@brief 生成時に一度だけ呼び出される関数
	*/
	void Billboard::OnCreate()
	{
		DrawObject::OnCreate();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PCTStaticDraw>();
		m_ptrDraw->SetOriginalMeshUse(true);
		m_ptrDraw->CreateOriginalMesh(m_vertex);
		m_ptrDraw->SetTextureResource(m_texWstr);
		m_ptrDraw->SetSamplerState(SamplerState::LinearWrap);

		// 透明色の描画を可能に
		SetAlphaActive(true);
	}

	/*!
	@brief ビルボード更新関数
	*/
	void Billboard::UpdateBillboard()
	{
		// カメラを元にビルボード関数でクォータニオンを設定
		const auto& ptrCamera = GetStage()->GetView()->GetTargetCamera();
		Quat qt = Utility::GetBillboardQuat(ptrCamera->GetAt() - ptrCamera->GetEye());
		m_ptrTrans->SetQuaternion(qt);
	}

	/*!
	@brief ディフューズ色を使って色を変更する関数
	@param 色
	*/
	void Billboard::SetDiffuseColor(const Col4& color)
	{
		// 描画コンポーネントの取得してディヒューズ色に設定
		m_ptrDraw->SetDiffuse(color);

		// 色を保持
		m_diffuseColor = color;
	}

	/*!
	@brief エミッシブ色を使って色を変更する関数
	@param 色
	*/
	void Billboard::SetEmissiveColor(const Col4& color)
	{
		// 描画コンポーネントの取得してエミッシブ色に設定
		m_ptrDraw->SetEmissive(color);

		// 色を保持
		m_emissiveColor = color;
	}

	/*!
	@brief 頂点色を使って色を変更する関数
	@param 色
	*/
	void Billboard::SetVerticesColor(const Col4& color)
	{
		// 色を頂点色に設定
		for (auto& v : m_vertex.vertices)
		{
			v.color = color;
		}

		// 描画コンポーネントの取得して頂点データの更新
		m_ptrDraw->UpdateVertices(m_vertex.vertices);

		// 色を保持
		m_verticesColor = color;
	}

	/*!
	@brief テクスチャを変更する関数
	@param テクスチャキー
	*/
	void Billboard::SetTexture(const wstring& textureKey)
	{
		m_ptrDraw->SetTextureResource(textureKey);
	}

	/*!
	@brief 描画変更設定関数
	@param eVerticesRect
	*/
	void Billboard::SetVerticesRect(eRectType rect)
	{
		// 頂点座標の初期化
		Utility::SimpleVertices(m_vertex.vertices);

		// 加算量用変数
		const float HELF = 0.5f;
		Vec3 addPos;

		// 向きに応じて設定
		switch (rect)
		{
		case eRectType::UpRight: // 右上なら
			addPos = Vec2(HELF, HELF);
			break;

		case eRectType::UpLeft: // 左上なら
			addPos = Vec2(-HELF, HELF);
			break;

		case eRectType::DownRight: // 右下なら
			addPos = Vec2(HELF, -HELF);
			break;

		case eRectType::DownLeft: // 左下なら
			addPos = Vec2(-HELF, -HELF);
			break;

		default:
			break;
		}

		// 頂点データの座標を加算
		for (auto& v : m_vertex.vertices)
		{
			v.position += addPos;
		}

		// 頂点データの更新
		m_ptrDraw->UpdateVertices(m_vertex.vertices);
	}

	/*!
	@brief テクスチャをベロシティに応じて回転させる関数
	*/
	void Billboard::RotateTexture()
	{
		// 頂点の数分ループ
		for (auto& v : m_vertex.vertices)
		{
			// 頂点のUV座標を移動量×デルタタイムで加算
			v.textureCoordinate += m_velocity * DELTA_TIME;
		}

		// 頂点の更新
		m_ptrDraw->UpdateVertices(m_vertex.vertices);
	}
}