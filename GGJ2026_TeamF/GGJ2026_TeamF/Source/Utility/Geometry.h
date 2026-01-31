#pragma once

/// <summary>
/// 2次元ベクトルクラス
/// </summary>
class Vector2D
{
public:
	float x;
	float y;

public:
	// コンストラクタ・デストラクタ
	Vector2D() : x(0.0f), y(0.0f) {}
	Vector2D(float s) : x(s), y(s) {}
	Vector2D(float sx, float sy) : x(sx), y(sy) {}
	~Vector2D() = default;

public:
	// 演算子オーバーロード
	void operator += (const Vector2D& v)
	{
		this->x += v.x;
		this->y += v.y;
	}
	void operator -= (const Vector2D& v)
	{
		this->x -= v.x;
		this->y -= v.y;
	}
	void operator *= (float s)
	{
		this->x *= s;
		this->y *= s;
	}
	Vector2D operator * (const float s) const
	{
		return Vector2D(this->x * s, this->y * s);
	}
	Vector2D operator - () const
	{
		return Vector2D(-x, -y);
	}

public:
	// ベクトルの大きさを返します
	const float Magnitude() const;
	// 正規化します
	void Normalize();
	// 正規化ベクトルを返す
	Vector2D Normalized() const;

public:
	// int型に変換する
	void ToInt(int* x, int* y) const
	{
		*x = static_cast<int>(this->x);
		*y = static_cast<int>(this->y);
	}

public:
	// 内積
	static float Dot(const Vector2D& va, const Vector2D& vb)
	{
		return (va.x * vb.x + va.y * vb.y);
	}
	// 外積
	static float Cross(const Vector2D& va, const Vector2D& vb)
	{
		return (va.x * vb.y - vb.x * va.y);
	}

	// 線形補間
	static Vector2D Lerp(const Vector2D& va, const Vector2D& vb, float t);

	// 反射ベクトルの計算
	static Vector2D Reflect(const Vector2D& v, const Vector2D& n);
};

inline Vector2D operator + (const Vector2D& va, const Vector2D& vb)
{
	return Vector2D(va.x + vb.x, va.y + vb.y);
}

inline Vector2D operator - (const Vector2D& va, const Vector2D& vb)
{
	return Vector2D(va.x - vb.x, va.y - vb.y);
}

inline float operator * (const Vector2D& va, const Vector2D& vb)
{
	return Vector2D::Dot(va, vb);
}

inline float operator % (const Vector2D& va, const Vector2D& vb)
{
	return Vector2D::Cross(va, vb);
}

// 位置座標として使えるようにする
using Location2D = Vector2D;

// 矩形のサイズとして使えるようにする
using Rect2D = Vector2D;
