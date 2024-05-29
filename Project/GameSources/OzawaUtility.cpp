/*!
@file OzawaUtility.cpp
@brief ���[�e�B���e�B
@author ���V���M
*/

#include "stdafx.h"
#include "OzawaUtility.h"

namespace Utility
{
	/*!
	@brief �l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
	*/
	bool GetBetween(int value, int a, int b)
	{
		return a > b ? value <= a && value >= b : value <= b && value >= a;
	}

	/*!
	@brief �l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
	*/
	bool GetBetween(float value, float a, float b)
	{
		return a > b ? value <= a && value >= b : value <= b && value >= a;
	}

#if BASECROSS

	/*!
	@brief �V���v���Ȓ��_�f�[�^���쐬
	@param (vertices) ���_�f�[�^
	*/
	void SimpleVertices(vector<basecross::VertexPositionColorTexture>& vertices, const Col4& color)
	{
		// �����̃T�C�Y
		const float HALF_SIZE = 0.5f;

		// ������
		vertices.clear();

		// ���_�f�[�^
		vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), color, Vec2(0.0f, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), color , Vec2(1.0f, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), color, Vec2(0.0f, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), color, Vec2(1.0f, 1.0f)},
		};
	}

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	*/
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices)
	{
		// �V���v���Ȓ��_�f�[�^�̍쐬
		SimpleVertices(vertices);

		// ���_�̕��я�
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex)
	{
		// �V���v���Ȓ��_�f�[�^�̍쐬
		SimpleVertices(vertex.vertices);

		// ���_�̕��я�
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (color) �Œ�F
	*/
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Col4& color)
	{
		// �V���v���Ȓ��_�f�[�^�̍쐬
		SimpleVertices(vertices, color);

		// ���_�̕��я�
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �V���v���Ȓ��_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex, const Col4& color)
	{
		// �V���v���Ȓ��_�f�[�^�̍쐬
		SimpleVertices(vertex.vertices, color);

		// ���_�̕��я�
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �����p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (number) ����
	*/
	void NumberVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int number)
	{
		// �����̃T�C�Y�ƃf�t�H���g�̐F
		const float HALF_SIZE = 0.5f;
		const float UV_LEFT = 1.0f / 10.0f * number;
		const float UV_RIGHT = 1.0f / 10.0f * (number + 1);
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// ���_�f�[�^
		vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_LEFT, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(UV_LEFT, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
		};

		// ���_�̕��я�
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief �����p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertex) ���_�f�[�^�ƒ��_�C���f�b�N�X�\����
	@param (number) ����
	*/
	void NumberVerticesIndices(basecross::VertexData& vertex, const int number)
	{
		// �����̃T�C�Y�ƃf�t�H���g�̐F
		const float HALF_SIZE = 0.5f;
		const float UV_LEFT = 1.0f / 10.0f * number;
		const float UV_RIGHT = 1.0f / 10.0f * (number + 1);
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// ���_�f�[�^
		vertex.vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_LEFT, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(UV_LEFT, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
		};

		// ���_�̕��я�
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief ���{���^���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (point) �ߖڂ̍��W�z��
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (axisVec) ���s���x�N�g��
	@param (weight) ��
	@param (texLoop) �e�N�X�`�����[�v
	@return �����o�������̐^�U
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Vec3& axisVec, float weight, int texLoop)
	{
		// �ߖڂ̐���0�Ȃ�I��
		const int& size = static_cast<int>(point.size()); 
		if (size == 0) return false;

		vertices.clear(); // ���_���W�̏�����

		// �ߖڂ̐����[�v
		for (int i = 0; i < size; i++)
		{
			// �e�N�X�`�������[�v���邩
			const int loop = texLoop > 0 ? texLoop : 1;

			int front = (i - 1) < 0 ? size - 1 : i - 1; // ��O
			int rear = (i + 1) % size; // ���

			// ��������O�ς����߂�
			Vec3 dir = point.at(front) - point.at(rear);
			Vec3 cross = dir.cross(axisVec).normalize();
		
			// ���������������č��W�𒸓_�Ƃ��Ēǉ�
			Vec3 pos = point.at(i) - cross * weight;
			basecross::VertexPositionColorTexture vertexLeft(pos, COL_WHITE, Vec2(0.0f, static_cast<float>(i) / size * loop));
			vertices.push_back(vertexLeft);

			// ���������Z�������W�𒸓_�Ƃ��Ēǉ�
			pos = point.at(i) + cross * weight;
			basecross::VertexPositionColorTexture vertexRight(pos, COL_WHITE, Vec2(1.0f, static_cast<float>(i) / size * loop));
			vertices.push_back(vertexRight);
		}

		// ���_�C���f�b�N�X�̌�
		const vector<uint16_t> baseIndices = {
			0, 1, 2,
			2, 1, 3
		};

		indices.clear(); // �C���f�b�N�X�̏�����

		// �T�C�Y������񏭂Ȃ������[�v
		for (int i = 0; i < size - 1; i++)
		{
			// ���T�C�Y�����[�v
			for (int j = 0; j < baseIndices.size(); j++)
			{
				// ���̃C���f�b�N�X��2�̔{�������Z�����Ēǉ�
				indices.push_back(baseIndices.at(j) + (2 * i));
			}
		}

		return true; // �����o�����̂�true��Ԃ�
	}

	/*!
	@brief ���{���^���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (point) �ߖڂ̍��W�z��
	@param (vertex) ���_�f�[�^�ƒ��_�C���f�b�N�X
	@param (axisVec) ���s���x�N�g��
	@param (weight) ��
	@param (texLoop) �e�N�X�`�����[�v
	@return �����o�������̐^�U
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, basecross::VertexData& vertex, const Vec3& axisVec, float weight, int texLoop)
	{
		// ���_�f�[�^�\���̂𕪉����Đ����֐������s
		return RibonVerticesIndices(point, vertex.vertices, vertex.indices, axisVec, weight, texLoop);
	}

	/*!
	@brief �~�^�p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (number) ����
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
			// 3�̔{������ɐݒ�
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
		}
	}

	/*!
	@brief �~�^�p���_�f�[�^�ƒ��_�C���f�b�N�X���쐬
	@param (vertices) ���_�f�[�^
	@param (indices) ���_�C���f�b�N�X
	@param (number) ����
	*/
	void CircleVerticesIndices(basecross::VertexData& vertex, const int squareNum)
	{
		CircleVerticesIndices(vertex.vertices, vertex.indices, squareNum);
	}

	/*!
	@brief ���[���h���W���X�N���[�����W�ɕϊ�(BaseCross�̍��W�p�A����ȊO��DirectX�t���̊֐�����l)
	@param (viewPtr) view��shared_ptr
	@param (position) ���[���h���W
	@return �ϊ����ꂽ�X�N���[�����W
	*/
	Vec3 ConvertToWorldPosition(const shared_ptr<basecross::ViewBase>& viewPtr, const Vec3& position)
	{
		// �r���[����J�����ƃr���[�|�[�g���擾
		const auto& ptrCamera = viewPtr->GetTargetCamera();
		const auto& viewPort = viewPtr->GetTargetViewport();

		// �}�g���b�N�X�̒�`
		const Mat4x4 viewMat = ptrCamera->GetViewMatrix();
		const Mat4x4 projMat = ptrCamera->GetProjMatrix();
		Mat4x4 worldMat;
		worldMat.affineTransformation(
			Vec3(1.0f),
			Vec3(0.0f),
			Vec3(0.0f),
			Vec3(0.0f)
		);

		// 3D���W��2D���W�ɕϊ�(����0,0�)
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

		// BaseCross�p�ɕϊ�(����0,0�)
		Vec3 windowPos = Vec3(xmWinPos);
		windowPos.x -= static_cast<float>(viewPort.Width) / 2.0f;
		windowPos.y -= static_cast<float>(viewPort.Height) / 2.0f;
		windowPos.y *= -1.0f;

		// �ϊ����ꂽ���W
		return windowPos;
	}

	/*!
	@brief �r���{�[�h���N�H�[�^�j�I���ō쐬���Ԃ�
	@param (Line)�@�J�����̒����_ - �J�����̈ʒu
	@return �쐬���ꂽ�N�H�[�^�j�I��
	*/
	Quat GetBillboardQuat(const Vec3& Line)
	{
		// �����̃R�s�[
		Vec3 temp = Line;

		// �}�g���b�N�X�̐錾
		Mat4x4 rotMatrix;

		// ��]���̐ݒ�
		Vec3 defUp(0, 1.0f, 0);

		// ������Vec2�ɕϊ�
		Vec2 tempVec(temp.x, temp.z);

		// �O�㍶�E�̒�����0.1f�ȉ��Ȃ�
		if (tempVec.length() < 0.1f)
		{
			// ��]����ύX
			defUp = Vec3(0, 0, 1.0f);
		}
		
		// �����̐��K��
		temp.normalize();

		// �}�g���b�N�X�̌v�Z
		rotMatrix = XMMatrixLookAtLH(Vec3(0, 0, 0), temp, defUp);
		rotMatrix.inverse();

		// �N�H�[�^�j�I���ɕϊ����A���K��
		Quat qt = rotMatrix.quatInMatrix();
		qt.normalize();
		return qt;
	}

	/*!
	@brief �l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
	*/
	bool GetBetween(const Vec2& value, const Vec2& a, const Vec2& b)
	{
		return GetBetween(value.x, a.x, b.x) && GetBetween(value.y, a.y, b.y);
	}

	/*!
	@brief �l���͈͓�����Ԃ�
	@param (value)�@�m�F����l
	@param (a)�@�͈͂̒l�P
	@param (b)�@�͈͂̒l�Q
	@return �͈͓����̐^�U
	*/
	bool GetBetween(const Vec3& value, const Vec3& a, const Vec3& b)
	{
		return GetBetween(value.x, a.x, b.x) && GetBetween(value.y, a.y, b.y) && GetBetween(value.z, a.z, b.z);
	}


	Vec3 SLerp(const Vec3& start, const Vec3& end, float t) {
		return start * (1.0f - t) + end * t;
	}

	Vec3 QuadraticBezier(const Vec3& A, const Vec3& B, const Vec3& C, float t) {
		Vec3 P0P1 = SLerp(A, B, t);
		Vec3 P1P2 = SLerp(B, C, t);
		return SLerp(P0P1, P1P2, t);
	}

	Vec3 CurveLerp(const Vec3& A, const Vec3& B, const Vec3& C, float totalTime) {
		return QuadraticBezier(A, B, C, totalTime);
	}

#endif
}