#pragma once

//#include "util/vector.hpp"

#define		MATRIX_INLINE	__forceinline

namespace cloture//::util::math::matrix
{
namespace util
{
namespace math
{
namespace matrix
{
	using columnType = cloture::util::math::vector::vector4f;
	
	struct alignas(mSimdAlign)
	matrix4D
	{
		using matType	=	matrix4D;
		static constexpr size_t numberOfRows = 4;
		static constexpr size_t numberOfColumns = 4;
		
		#if		mNativeVectorSize == 32
			union
			{
				columnType	m[numberOfRows];
				__m256		matrixRegisters[2];
			};
		#elif	mNativeVectorSize == 64
			union
			{
				columnType	m[numberOfRows];
				__m512		matrixRegister;
			};
		#else
			columnType m[numberOfRows];
		#endif
		
		
		MATRIX_INLINE void copyFrom(const matType* RESTRICT const in) 
		{
			//m = (in->m);
			m[0] = in->m[0];
			m[1] = in->m[1];
			m[2] = in->m[2];
			m[3] = in->m[3];
		}
			
		MATRIX_INLINE void copyRotateOnly (const matType* RESTRICT const in)  
		{
			m[0][0] = in->m[0][0];
			m[0][1] = in->m[0][1];
			m[0][2] = in->m[0][2];
			m[0][3] = .0f;
			m[1][0] = in->m[1][0];
			m[1][1] = in->m[1][1];
			m[1][2] = in->m[1][2];
			m[1][3] = .0f;
			m[2][0] = in->m[2][0];
			m[2][1] = in->m[2][1];
			m[2][2] = in->m[2][2];
			m[2][3] = .0f;
			m[3][0] = .0f;
			m[3][1] = .0f;
			m[3][2] = .0f;
			m[3][3] = 1.0f;
		}

		MATRIX_INLINE void copyTranslateOnly (const matType* RESTRICT const in) 
		{

			m[0][0] = 1.0f;
			m[0][1] = .0f;
			m[0][2] = .0f;
			m[0][3] = in->m[0][3];
			m[1][0] = .0f;
			m[1][1] = 1.0f;
			m[1][2] = .0f;
			m[1][3] = in->m[1][3];
			m[2][0] = .0f;
			m[2][1] = .0f;
			m[2][2] = 1.0f;
			m[2][3] = in->m[2][3];
			m[3][0] = .0f;
			m[3][1] = .0f;
			m[3][2] = .0f;
			m[3][3] = 1.0f;

		}
		MATRIX_INLINE void concat(const matType *RESTRICT const in1, const matType *RESTRICT const in2) 
		{

			m[0][0] = in1->m[0][0] * in2->m[0][0] + in1->m[0][1] * in2->m[1][0] + in1->m[0][2] * in2->m[2][0] + in1->m[0][3] * in2->m[3][0];
			m[0][1] = in1->m[0][0] * in2->m[0][1] + in1->m[0][1] * in2->m[1][1] + in1->m[0][2] * in2->m[2][1] + in1->m[0][3] * in2->m[3][1];
			m[0][2] = in1->m[0][0] * in2->m[0][2] + in1->m[0][1] * in2->m[1][2] + in1->m[0][2] * in2->m[2][2] + in1->m[0][3] * in2->m[3][2];
			m[0][3] = in1->m[0][0] * in2->m[0][3] + in1->m[0][1] * in2->m[1][3] + in1->m[0][2] * in2->m[2][3] + in1->m[0][3] * in2->m[3][3];
			m[1][0] = in1->m[1][0] * in2->m[0][0] + in1->m[1][1] * in2->m[1][0] + in1->m[1][2] * in2->m[2][0] + in1->m[1][3] * in2->m[3][0];
			m[1][1] = in1->m[1][0] * in2->m[0][1] + in1->m[1][1] * in2->m[1][1] + in1->m[1][2] * in2->m[2][1] + in1->m[1][3] * in2->m[3][1];
			m[1][2] = in1->m[1][0] * in2->m[0][2] + in1->m[1][1] * in2->m[1][2] + in1->m[1][2] * in2->m[2][2] + in1->m[1][3] * in2->m[3][2];
			m[1][3] = in1->m[1][0] * in2->m[0][3] + in1->m[1][1] * in2->m[1][3] + in1->m[1][2] * in2->m[2][3] + in1->m[1][3] * in2->m[3][3];
			m[2][0] = in1->m[2][0] * in2->m[0][0] + in1->m[2][1] * in2->m[1][0] + in1->m[2][2] * in2->m[2][0] + in1->m[2][3] * in2->m[3][0];
			m[2][1] = in1->m[2][0] * in2->m[0][1] + in1->m[2][1] * in2->m[1][1] + in1->m[2][2] * in2->m[2][1] + in1->m[2][3] * in2->m[3][1];
			m[2][2] = in1->m[2][0] * in2->m[0][2] + in1->m[2][1] * in2->m[1][2] + in1->m[2][2] * in2->m[2][2] + in1->m[2][3] * in2->m[3][2];
			m[2][3] = in1->m[2][0] * in2->m[0][3] + in1->m[2][1] * in2->m[1][3] + in1->m[2][2] * in2->m[2][3] + in1->m[2][3] * in2->m[3][3];
			m[3][0] = in1->m[3][0] * in2->m[0][0] + in1->m[3][1] * in2->m[1][0] + in1->m[3][2] * in2->m[2][0] + in1->m[3][3] * in2->m[3][0];
			m[3][1] = in1->m[3][0] * in2->m[0][1] + in1->m[3][1] * in2->m[1][1] + in1->m[3][2] * in2->m[2][1] + in1->m[3][3] * in2->m[3][1];
			m[3][2] = in1->m[3][0] * in2->m[0][2] + in1->m[3][1] * in2->m[1][2] + in1->m[3][2] * in2->m[2][2] + in1->m[3][3] * in2->m[3][2];
			m[3][3] = in1->m[3][0] * in2->m[0][3] + in1->m[3][1] * in2->m[1][3] + in1->m[3][2] * in2->m[2][3] + in1->m[3][3] * in2->m[3][3];

		}
		
		MATRIX_INLINE void transpose (const matType *RESTRICT const in1) 
		{
			m[0][0] = in1->m[0][0];
			m[0][1] = in1->m[1][0];
			m[0][2] = in1->m[2][0];
			m[0][3] = in1->m[3][0];
			m[1][0] = in1->m[0][1];
			m[1][1] = in1->m[1][1];
			m[1][2] = in1->m[2][1];
			m[1][3] = in1->m[3][1];
			m[2][0] = in1->m[0][2];
			m[2][1] = in1->m[1][2];
			m[2][2] = in1->m[2][2];
			m[2][3] = in1->m[3][2];
			m[3][0] = in1->m[0][3];
			m[3][1] = in1->m[1][3];
			m[3][2] = in1->m[2][3];
			m[3][3] = in1->m[3][3];
		}
		MATRIX_INLINE int invertFull(const matType *RESTRICT const in1) 
		{
			float det;

			// note: orientation does not matter, as transpose(invert(transpose(m))) == invert(m), proof:
			//   transpose(invert(transpose(m))) * m
			// = transpose(invert(transpose(m))) * transpose(transpose(m))
			// = transpose(transpose(m) * invert(transpose(m)))
			// = transpose(identity)
			// = identity

			// this seems to help gcc's common subexpression elimination, and also makes the code look nicer
			float   m00 = in1->m[0][0], m01 = in1->m[0][1], m02 = in1->m[0][2], m03 = in1->m[0][3],
					m10 = in1->m[1][0], m11 = in1->m[1][1], m12 = in1->m[1][2], m13 = in1->m[1][3],
					m20 = in1->m[2][0], m21 = in1->m[2][1], m22 = in1->m[2][2], m23 = in1->m[2][3],
					m30 = in1->m[3][0], m31 = in1->m[3][1], m32 = in1->m[3][2], m33 = in1->m[3][3];

			// calculate the adjoint
			m[0][0] =  (m11*(m22*m33 - m23*m32) - m21*(m12*m33 - m13*m32) + m31*(m12*m23 - m13*m22));
			m[0][1] = -(m01*(m22*m33 - m23*m32) - m21*(m02*m33 - m03*m32) + m31*(m02*m23 - m03*m22));
			m[0][2] =  (m01*(m12*m33 - m13*m32) - m11*(m02*m33 - m03*m32) + m31*(m02*m13 - m03*m12));
			m[0][3] = -(m01*(m12*m23 - m13*m22) - m11*(m02*m23 - m03*m22) + m21*(m02*m13 - m03*m12));
			m[1][0] = -(m10*(m22*m33 - m23*m32) - m20*(m12*m33 - m13*m32) + m30*(m12*m23 - m13*m22));
			m[1][1] =  (m00*(m22*m33 - m23*m32) - m20*(m02*m33 - m03*m32) + m30*(m02*m23 - m03*m22));
			m[1][2] = -(m00*(m12*m33 - m13*m32) - m10*(m02*m33 - m03*m32) + m30*(m02*m13 - m03*m12));
			m[1][3] =  (m00*(m12*m23 - m13*m22) - m10*(m02*m23 - m03*m22) + m20*(m02*m13 - m03*m12));
			m[2][0] =  (m10*(m21*m33 - m23*m31) - m20*(m11*m33 - m13*m31) + m30*(m11*m23 - m13*m21));
			m[2][1] = -(m00*(m21*m33 - m23*m31) - m20*(m01*m33 - m03*m31) + m30*(m01*m23 - m03*m21));
			m[2][2] =  (m00*(m11*m33 - m13*m31) - m10*(m01*m33 - m03*m31) + m30*(m01*m13 - m03*m11));
			m[2][3] = -(m00*(m11*m23 - m13*m21) - m10*(m01*m23 - m03*m21) + m20*(m01*m13 - m03*m11));
			m[3][0] = -(m10*(m21*m32 - m22*m31) - m20*(m11*m32 - m12*m31) + m30*(m11*m22 - m12*m21));
			m[3][1] =  (m00*(m21*m32 - m22*m31) - m20*(m01*m32 - m02*m31) + m30*(m01*m22 - m02*m21));
			m[3][2] = -(m00*(m11*m32 - m12*m31) - m10*(m01*m32 - m02*m31) + m30*(m01*m12 - m02*m11));
			m[3][3] =  (m00*(m11*m22 - m12*m21) - m10*(m01*m22 - m02*m21) + m20*(m01*m12 - m02*m11));

			// calculate the determinant (as inverse == 1/det * adjoint, adjoint * m == identity * det, so this calculates the det)
			det = m00*m[0][0] + m10*m[0][1] + m20*m[0][2] + m30*m[0][3];
			if (det == .0f)
					return 0;

			// multiplications are faster than divisions, usually
			det = 1.0f / det;

			// manually unrolled loop to multiply all matrix elements by 1/det
			m[0][0] *= det; m[0][1] *= det; m[0][2] *= det; m[0][3] *= det;
			m[1][0] *= det; m[1][1] *= det; m[1][2] *= det; m[1][3] *= det;
			m[2][0] *= det; m[2][1] *= det; m[2][2] *= det; m[2][3] *= det;
			m[3][0] *= det; m[3][1] *= det; m[3][2] *= det; m[3][3] *= det;

			return 1;
		}

		MATRIX_INLINE void invertSimple(const matType *RESTRICT const in1) 
		{
			// we only support uniform scaling, so assume the first row is enough
			// (note the lack of sqrt here, because we're trying to undo the scaling,
			// this means multiplying by the inverse scale twice - squaring it, which
			// makes the sqrt a waste of time)
			{
				const float scale = 1.0f / (in1->m[0][0] * in1->m[0][0] + in1->m[0][1] * in1->m[0][1] + in1->m[0][2] * in1->m[0][2]);

				// invert the rotation by transposing and multiplying by the squared
				// recipricol of the input matrix scale as described above
				m[0][0] = in1->m[0][0] * scale;
				m[0][1] = in1->m[1][0] * scale;
				m[0][2] = in1->m[2][0] * scale;
				m[1][0] = in1->m[0][1] * scale;
				m[1][1] = in1->m[1][1] * scale;
				m[1][2] = in1->m[2][1] * scale;
				m[2][0] = in1->m[0][2] * scale;
				m[2][1] = in1->m[1][2] * scale;
				m[2][2] = in1->m[2][2] * scale;
			}

			// invert the translate
			m[0][3] = -(in1->m[0][3] * m[0][0] + in1->m[1][3] * m[0][1] + in1->m[2][3] * m[0][2]);
			m[1][3] = -(in1->m[0][3] * m[1][0] + in1->m[1][3] * m[1][1] + in1->m[2][3] * m[1][2]);
			m[2][3] = -(in1->m[0][3] * m[2][0] + in1->m[1][3] * m[2][1] + in1->m[2][3] * m[2][2]);

			// don't know if there's anything worth doing here
			m[3][0] = .0f;
			m[3][1] = .0f;
			m[3][2] = .0f;
			m[3][3] = 1.0f;
		}
		
		MATRIX_INLINE void interpolate(const matType *RESTRICT const in1, const matType *RESTRICT const in2, const float frac) 
		{
			//const float frac_ = frac;
			vectorizeLoop
			for (size_t i = 0; i < numberOfRows; i++)
			vectorizeLoop
				for (size_t j = 0; j < numberOfColumns; j++)
					m[i][j] = in1->m[i][j] + frac * (in2->m[i][j] - in1->m[i][j]);
		}

		MATRIX_INLINE void clear() 
		{
			vectorizeLoop
			for (size_t i = 0; i < numberOfRows; i++)
			vectorizeLoop
				for (size_t j = 0; j < numberOfColumns; j++)
					m[i][j] = .0f;
		}

		MATRIX_INLINE void accumulate( const matType *RESTRICT const in, const float weight) 
		{
			vectorizeLoop
			for (size_t i = 0; i < numberOfRows; i++)
			vectorizeLoop
				for (size_t j = 0; j < numberOfColumns; j++)
					m[i][j] += in->m[i][j] * weight;
		}
		MATRIX_INLINE void scale(const float rotatescale, const float originscale)
		{
			m[0][0] *= rotatescale;
			m[0][1] *= rotatescale;
			m[0][2] *= rotatescale;
			m[1][0] *= rotatescale;
			m[1][1] *= rotatescale;
			m[1][2] *= rotatescale;
			m[2][0] *= rotatescale;
			m[2][1] *= rotatescale;
			m[2][2] *= rotatescale;
			m[0][3] *= originscale;
			m[1][3] *= originscale;
			m[2][3] *= originscale;
		}
		MATRIX_INLINE void normalize(const matType *RESTRICT const in1) 
		{
			// scale rotation matrix vectors to a length of 1
			// note: this is only designed to undo uniform scaling
			//double scale = 1.0 / sqrt(
			const float _scale = 1.0f / sqrtf(
			in1->m[0][0] * in1->m[0][0] + in1->m[0][1] * in1->m[0][1] + in1->m[0][2] * in1->m[0][2]);
			//*out = *in1;
			copyFrom(in1);
			//Matrix4x4_Scale(this, _scale, 1);
			scale(_scale, 1.0f);
		}

		MATRIX_INLINE void normalize3(const matType *RESTRICT const in1) 
		{
			// scale each rotation matrix vector to a length of 1
			// intended for use after Matrix4x4_Interpolate or Matrix4x4_Accumulate
			//*out = *in1;
			copyFrom(in1);
			vectorizeLoop
			for (size_t i = 0; i < 3; i++)
			{
				//double scale = sqrt(
				float scale = sqrtf(
				in1->m[0][i] * in1->m[0][i] + in1->m[1][i] * in1->m[1][i] + in1->m[2][i] * in1->m[2][i]);
				if (scale)
					scale = 1.0f / scale;//1.0 / scale;
				m[0][i] *= scale;
				m[1][i] *= scale;
				m[2][i] *= scale;
			}
		}

		MATRIX_INLINE void reflect(const float normalx, const float normaly, const float normalz, const float dist, const float axisscale) 
		{
			//float p[4], p2[4];
			vector::vector4f p, p2;
			p[0] = normalx;
			p[1] = normaly;
			p[2] = normalz;
			p[3] = -dist;
			p2[0] = p[0] * axisscale;
			p2[1] = p[1] * axisscale;
			p2[2] = p[2] * axisscale;
			p2[3] = .0f;
			
			vectorizeLoop
			for (size_t i = 0; i < numberOfColumns; i++)
			{
				const float d = m[0][i] * p[0] + m[1][i] * p[1] + m[2][i] * p[2] + m[3][i] * p[3];
				m[0][i] += p2[0] * d;
				m[1][i] += p2[1] * d;
				m[2][i] += p2[2] * d;
			}
		}

		MATRIX_INLINE void createIdentity() 
		{
			#if mNativeVectorSize >= 64
				matrixRegister = _mm512_setzero();
				m[3][3] = m[2][2] = m[1][1] = m[0][0] = 1.0f;
			#elif mNativeVectorSize == 32
				matrixRegisters[1] = matrixRegisters[0] = _mm256_setzero_ps();
				m[3][3] = m[2][2] = m[1][1] = m[0][0] = 1.0f;
			#else
				m[0][0]		=	1.0f;
				m[0][1]		=	.0f;
				m[0][2]		=	.0f;
				m[0][3]		=	.0f;
				m[1][0]		=	.0f;
				m[1][1]		=	1.0f;
				m[1][2]		=	.0f;
				m[1][3]		=	.0f;
				m[2][0]		=	.0f;
				m[2][1]		=	.0f;
				m[2][2]		=	1.0f;
				m[2][3]		=	.0f;
				m[3][0]		=	.0f;
				m[3][1]		=	.0f;
				m[3][2]		=	.0f;
				m[3][3]		=	1.0f;
			#endif
		}

		MATRIX_INLINE void createTranslate(const float x, const float y, const float z) 
		{
			m[0][0]		=	1.0f;
			m[0][1]		=	.0f;
			m[0][2]		=	.0f;
			m[0][3]		=	x;
			m[1][0]		=	.0f;
			m[1][1]		=	1.0f;
			m[1][2]		=	.0f;
			m[1][3]		=	y;
			m[2][0]		=	.0f;
			m[2][1]		=	.0f;
			m[2][2]		=	1.0f;
			m[2][3]		=	z;
			m[3][0]		=	.0f;
			m[3][1]		=	.0f;
			m[3][2]		=	.0f;
			m[3][3]		=	1.0f;
		}
		MATRIX_INLINE void fabs() 
		{
			#if mNativeVectorSize >= 64
				matrixRegister = _mm512_abs_ps(matrixRegister);
			#elif mNativeVectorSize == 32
				matrixRegisters[0] = _mm256_abs_ps(matrixRegisters[0]);
				matrixRegisters[0] = _mm256_abs_ps(matrixRegisters[1]);
			#else
				m[0][0] = fabsf(	m[0][0]	);
				m[0][1] = fabsf(	m[0][1]	);
				m[0][2] = fabsf(	m[0][2]	);
				m[1][0] = fabsf(	m[1][0]	);
				m[1][1] = fabsf(	m[1][1]	);
				m[1][2] = fabsf(	m[1][2]	);
				m[2][0] = fabsf(	m[2][0]	);
				m[2][1] = fabsf(	m[2][1]	);
				m[2][2] = fabsf(	m[2][2]	);
			#endif
		}
		
		MATRIX_INLINE __pseudopure vector::vector3f transform(const vector::vector3f v) const
		{
			return vector::vector3f
			(
				v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m[0][2] + m[0][3],
				v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m[1][2] + m[1][3],
				v[0] * m[2][0] + v[1] * m[2][1] + v[2] * m[2][2] + m[2][3]
			);
		}
		MATRIX_INLINE __pseudopure vector::vector3f transform3x3(const vector::vector3f v) const
		{
			return vector::vector3f
			(
				v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m[0][2],
				v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m[1][2],
				v[0] * m[2][0] + v[1] * m[2][1] + v[2] * m[2][2]
			);
		}
		MATRIX_INLINE matrix4D(const matrix4D& other) 
		{
			#if mNativeVectorSize >= 64
				matrixRegister = other.matrixRegister;
			#elif mNativeVectorSize >= 32
				matrixRegisters[0] = other.matrixRegisters[0];
				matrixRegisters[1] = other.matrixRegisters[1];
			#else
				m[0] = other.m[0];
				m[1] = other.m[1];
				m[2] = other.m[2];
				m[3] = other.m[3];
			#endif
		}
		MATRIX_INLINE matrix4D() : m{}
		{}
		MATRIX_INLINE matrix4D(float *m0, float *m1, float* m2, float *m3)
		{
			using cloture::util::math::vector::vector4f;
			m[0] = vector4f(m0[0], m0[1], m0[2], m0[3]);
			m[1] = vector4f(m1[0], m1[1], m1[2], m1[3]);
			m[2] = vector4f(m2[0], m2[1], m2[2], m2[3]);
			m[3] = vector4f(m3[0], m3[1], m3[2], m3[3]);
		}

		#if mNativeVectorSize >= 16
			MATRIX_INLINE matrix4D(const __m128 m0, const __m128 m1, const __m128 m2, const __m128 m3)
			{
				m[0] = m0;
				m[1] = m1;
				m[2] = m2;
				m[3] = m3;
			}
		#endif

		#if mNativeVectorSize >= 32
			MATRIX_INLINE matrix4D(const __m256 m01, const __m256 m23)
			{
				matrixRegisters[0] 	= m01;
				matrixRegisters[1] 	= m23;
			}
		#endif
		#if mNativeVectorSize >= 64
			MATRIX_INLINE matrix4D(const __m512 mat)
			{
				matrixRegister = mat;
			}
		#endif

		MATRIX_INLINE operator void*() 
		{
			return &m[0];
		}
		MATRIX_INLINE operator __m128*()
		{
			return reinterpret_cast<__m128*>(&m[0]);
		}
		#if mNativeVectorSize >= 32
			MATRIX_INLINE operator __m256*()
			{
				return &matrixRegisters[0];
			}
		#endif
		#if mNativeVectorSize >= 64
			MATRIX_INLINE operator __m512*()
			{
				return &matrixRegister;
			}
			MATRIX_INLINE operator __m512()
			{
				return matrixRegister;
			}
		#endif
	};
}
}
}
}
