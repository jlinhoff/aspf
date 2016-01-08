// spmath.h
#ifndef SPMATH_H
#define SPMATH_H

// Copyright 2016 Joe Linhoff
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef BASE_H
#include "base.h"
#endif // ndef BASE_H

#ifdef __cplusplus  // C in C++
extern "C" {        //
#endif              //

///////////////////////////////////////////////////////////////////////////////

#define PI 3.14159265358979323846264338327950288419716939937510
#define TWOPI (2.0*PI)

#define DEGTORAD(_deg_) (((_deg_)*PI)/180.0)
#define RADTODEG(_rad_) (((_rad_)*180.0)/PI)

#define SET2(_d_,_s1_,_s2_) (_d_)[0]=(_s1_),(_d_)[1]=(_s2_)
#define SET3(_d_,_s1_,_s2_,_s3_) (_d_)[0]=(_s1_), \
   (_d_)[1]=(_s2_),(_d_)[2]=(_s3_)
#define SET4(_d_,_s1_,_s2_,_s3_,_s4_) (_d_)[0]=(_s1_), \
   (_d_)[1]=(_s2_),(_d_)[2]=(_s3_),(_d_)[3]=(_s4_)
#define SET9(_d_,_s1_,_s2_,_s3_,_s4_,_s5_,_s6_,_s7_,_s8_,_s9_) \
   (_d_)[0]=(_s1_),(_d_)[1]=(_s2_),(_d_)[2]=(_s3_), \
   (_d_)[3]=(_s4_),(_d_)[4]=(_s5_),(_d_)[5]=(_s6_), \
   (_d_)[6]=(_s7_),(_d_)[7]=(_s8_),(_d_)[8]=(_s9_)
#define SET12(_d_,_s1_,_s2_,_s3_,_s4_,_s5_,_s6_,_s7_,_s8_,_s9_,_s10_,_s11_,_s12_) \
   (_d_)[0]=(_s1_),(_d_)[1]=(_s2_),(_d_)[2]=(_s3_), \
   (_d_)[3]=(_s4_),(_d_)[4]=(_s5_),(_d_)[5]=(_s6_), \
   (_d_)[6]=(_s7_),(_d_)[7]=(_s8_),(_d_)[8]=(_s9_), \
   (_d_)[9]=(_s10_),(_d_)[10]=(_s11_),(_d_)[11]=(_s12_)

#define CPY2(_d_,_s_) (_d_)[0]=(_s_)[0],(_d_)[1]=(_s_)[1]
#define CPY3(_d_,_s_) (_d_)[0]=(_s_)[0],(_d_)[1]=(_s_)[1],(_d_)[2]=(_s_)[2]
#define CPY4(_d_,_s_) (_d_)[0]=(_s_)[0],(_d_)[1]=(_s_)[1],(_d_)[2]=(_s_)[2],(_d_)[3]=(_s_)[3]
#define CPY9(_d_,_s_) (_d_)[0]=(_s_)[0],(_d_)[1]=(_s_)[1],(_d_)[2]=(_s_)[2],(_d_)[3]=(_s_)[3],\
   (_d_)[4]=(_s_)[4],(_d_)[5]=(_s_)[5],(_d_)[6]=(_s_)[6],(_d_)[7]=(_s_)[7],(_d_)[8]=(_s_)[8]
#define CPY12(_d_,_s_) (_d_)[0]=(_s_)[0],(_d_)[1]=(_s_)[1],(_d_)[2]=(_s_)[2],(_d_)[3]=(_s_)[3],\
   (_d_)[4]=(_s_)[4],(_d_)[5]=(_s_)[5],(_d_)[6]=(_s_)[6],(_d_)[7]=(_s_)[7],(_d_)[8]=(_s_)[8],\
   (_d_)[9]=(_s_)[9],(_d_)[10]=(_s_)[10],(_d_)[11]=(_s_)[11]
#define CPY(_d_,_s_,_n_) {int i=(_n_);for(i--;i>=0;i--)(_d_)[i]=(_s_)[i];}


//
// VECTORS, MATRICES, MATH
//

enum {
   // on paper, the matrix is:
   //  11 21 31
   //  12 22 32
   //  13 23 33
   // in memory, the matrix order is:
   //  11 12 13 21 22 23 31 32 33
   M9_11,
   M9_12,
   M9_13,
   M9_21,
   M9_22,
   M9_23,
   M9_31,
   M9_32,
   M9_33,
   M9_SIZE
};

enum {
   // on paper, the matrix is:
   //  11 21 31 41
   //  12 22 32 42
   //  13 23 33 43
   //  14 24 34 44
   // in memory, the matrix order is:
   //  11 12 13 14 21 22 23 24 31 32 33 34 41 42 43 44
   M16_11,
   M16_12,
   M16_13,
   M16_14,
   M16_21,
   M16_22,
   M16_23,
   M16_24,
   M16_31,
   M16_32,
   M16_33,
   M16_34,
   M16_41, // X
   M16_42, // Y
   M16_43, // Z
   M16_44,
   M16_SIZE
};

#define M16_X M16_41
#define M16_Y M16_42
#define M16_Z M16_43

enum {
   // on paper:
   //  X 11 21 31
   //  Y 12 22 32
   //  Z 13 23 33
   // in memory:
   //  X Y Z 11 12 13 21 22 23 31 32 33
   M12_X,
   M12_Y,
   M12_Z,
   M12_11,
   M12_12,
   M12_13,
   M12_21,
   M12_22,
   M12_23,
   M12_31,
   M12_32,
   M12_33,
   M12_SIZE
};

#define M12_M9 M12_11

// find cross product of two vectors
#define VecCross(_d_,_a_,_b_) \
   (_d_)[0]=(_a_)[1]*(_b_)[2]-(_a_)[2]*(_b_)[1], \
   (_d_)[1]=(_a_)[2]*(_b_)[0]-(_a_)[0]*(_b_)[2], \
   (_d_)[2]=(_a_)[0]*(_b_)[1]-(_a_)[1]*(_b_)[0]


// find dot product of two vectors
#define VecDot(_a_,_b_) \
   ((_a_)[0]*(_b_)[0]+(_a_)[1]*(_b_)[1]+(_a_)[2]*(_b_)[2])
#define XYDot(_a_,_b_) \
   ((_a_)[0]*(_b_)[0]+(_a_)[1]*(_b_)[1])

// subtract two vetors: d=a-b
#define VecSub(_d_,_a_,_b_) \
   (_d_)[0]=(_a_)[0]-(_b_)[0],(_d_)[1]=(_a_)[1]-(_b_)[1],(_d_)[2]=(_a_)[2]-(_b_)[2]

#define VecSubScaled(_d_,_a_,_b_,_mul_) \
   (_d_)[0]=(_a_)[0]-(_mul_)*(_b_)[0],(_d_)[1]=(_a_)[1]-(_mul_)*(_b_)[1],(_d_)[2]=(_a_)[2]-(_mul_)*(_b_)[2]

#define VecSquared(_v_) ((_v_)[0]*(_v_)[0]+(_v_)[1]*(_v_)[1]+(_v_)[2]*(_v_)[2])

// subtract two: d=a-b
#define XYSub(_d_,_a_,_b_) \
   (_d_)[0]=(_a_)[0]-(_b_)[0],(_d_)[1]=(_a_)[1]-(_b_)[1]

// add two vectors: d=a+b
#define VecAdd(_d_,_a_,_b_) \
   (_d_)[0]=(_a_)[0]+(_b_)[0],(_d_)[1]=(_a_)[1]+(_b_)[1],(_d_)[2]=(_a_)[2]+(_b_)[2]
#define XYAdd(_d_,_a_,_b_) \
   (_d_)[0]=(_a_)[0]+(_b_)[0],(_d_)[1]=(_a_)[1]+(_b_)[1]

// component multiply two vectors: d=a*b
#define VecMul(_d_,_a_,_b_) \
   (_d_)[0]=(_a_)[0]*(_b_)[0],(_d_)[1]=(_a_)[1]*(_b_)[1],(_d_)[2]=(_a_)[2]*(_b_)[2]
#define XYMul(_d_,_a_,_b_) \
   (_d_)[0]=(_a_)[0]*(_b_)[0],(_d_)[1]=(_a_)[1]*(_b_)[1]

// add a scaled vector to another vector: d=a+b*mul
#define VecAddScaled(_d_,_a_,_b_,_mul_) \
   (_d_)[0]=(_a_)[0]+(_mul_)*(_b_)[0],(_d_)[1]=(_a_)[1]+(_mul_)*(_b_)[1],(_d_)[2]=(_a_)[2]+(_mul_)*(_b_)[2]
#define XYAddScaled(_d_,_a_,_b_,_mul_) \
   (_d_)[0]=(_a_)[0]+(_mul_)*(_b_)[0],(_d_)[1]=(_a_)[1]+(_mul_)*(_b_)[1]
#define XYSubScaled(_d_,_a_,_b_,_mul_) \
   (_d_)[0]=(_a_)[0]-(_mul_)*(_b_)[0],(_d_)[1]=(_a_)[1]-(_mul_)*(_b_)[1]

// scale vector: d=a*mul
#define VecScale(_d_,_a_,_mul_) \
   (_d_)[0]=(_mul_)*(_a_)[0],(_d_)[1]=(_mul_)*(_a_)[1],(_d_)[2]=(_mul_)*(_a_)[2]
#define XYScale(_d_,_a_,_mul_) \
   (_d_)[0]=(_mul_)*(_a_)[0],(_d_)[1]=(_mul_)*(_a_)[1]

// copy the negative vector
#define VecNeg(_d_,_v_) (_d_)[0]=-(_v_)[0],(_d_)[1]=-(_v_)[1],(_d_)[2]=-(_v_)[2]
#define XYNeg(_d_,_v_) (_d_)[0]=-(_v_)[0],(_d_)[1]=-(_v_)[1]

// test for any non-zero
#define XYNotZero(_v_) (((int*)(_v_))[0]|((int*)(_v_))[1])
#define VecNotZero(_v_) (((int*)(_v_))[0]|((int*)(_v_))[1]|((int*)(_v_))[2])

// rotate vector by matrix
#define VecRotM9(_d_,_v_,_m_) \
   (_d_)[0]=(_v_)[0]*(_m_)[M9_11]+(_v_)[1]*(_m_)[M9_21] \
   +(_v_)[2]*(_m_)[M9_31], \
   (_d_)[1]=(_v_)[0]*(_m_)[M9_12]+(_v_)[1]*(_m_)[M9_22] \
   +(_v_)[2]*(_m_)[M9_32], \
   (_d_)[2]=(_v_)[0]*(_m_)[M9_13]+(_v_)[1]*(_m_)[M9_23] \
   +(_v_)[2]*(_m_)[M9_33]

// rotate vector by transpose of matrix
#define VecRotM9T(_d_,_v_,_m_) \
   (_d_)[0]=(_v_)[0]*(_m_)[M9_11]+(_v_)[1]*(_m_)[M9_12] \
   +(_v_)[2]*(_m_)[M9_13], \
   (_d_)[1]=(_v_)[0]*(_m_)[M9_21]+(_v_)[1]*(_m_)[M9_22] \
   +(_v_)[2]*(_m_)[M9_23], \
   (_d_)[2]=(_v_)[0]*(_m_)[M9_31]+(_v_)[1]*(_m_)[M9_32] \
   +(_v_)[2]*(_m_)[M9_33]

// rotate vector by matrix
#define VecRotM12(_d_,_v_,_m_) \
   (_d_)[0]=(_v_)[0]*(_m_)[M12_11]+(_v_)[1]*(_m_)[M12_21] \
   +(_v_)[2]*(_m_)[M12_31], \
   (_d_)[1]=(_v_)[0]*(_m_)[M12_12]+(_v_)[1]*(_m_)[M12_22] \
   +(_v_)[2]*(_m_)[M12_32], \
   (_d_)[2]=(_v_)[0]*(_m_)[M12_13]+(_v_)[1]*(_m_)[M12_23] \
   +(_v_)[2]*(_m_)[M12_33]

// transform vector by matrix
#define VecTransformM12(_d_,_v_,_m_) \
   (_d_)[0]=(_v_)[0]*(_m_)[M12_11]+(_v_)[1]*(_m_)[M12_21] \
   +(_v_)[2]*(_m_)[M12_31]+(_m_)[M12_X], \
   (_d_)[1]=(_v_)[0]*(_m_)[M12_12]+(_v_)[1]*(_m_)[M12_22] \
   +(_v_)[2]*(_m_)[M12_32]+(_m_)[M12_Y], \
   (_d_)[2]=(_v_)[0]*(_m_)[M12_13]+(_v_)[1]*(_m_)[M12_23] \
   +(_v_)[2]*(_m_)[M12_33]+(_m_)[M12_Z]

// build M16 from XYZ
#define M16FromXYZ(_dst_,_src_) \
   (_dst_)[M16_11]=(_dst_)[M16_22]=(_dst_)[M16_33]=(_dst_)[M16_44]=1, \
   (_dst_)[M16_12]=(_dst_)[M16_13]=(_dst_)[M16_14]= \
   (_dst_)[M16_21]=(_dst_)[M16_23]=(_dst_)[M16_24]= \
   (_dst_)[M16_31]=(_dst_)[M16_32]=(_dst_)[M16_34]=0, \
   (_dst_)[M16_41]=(_src_)[0],(_dst_)[M16_42]=(_src_)[1], \
   (_dst_)[M16_43]=(_src_)[2]

// build M16 from M9
#define M16FromM9(_dst_,_src_) (_dst_)[M16_14]=(_dst_)[M16_24]= \
   (_dst_)[M16_34]=0,(_dst_)[M16_44]=1,(_dst_)[M16_11]=(_src_)[M9_11], \
   (_dst_)[M16_21]=(_src_)[M9_21],(_dst_)[M16_31]=(_src_)[M9_31], \
   (_dst_)[M16_12]=(_src_)[M9_12],(_dst_)[M16_22]=(_src_)[M9_22], \
   (_dst_)[M16_32]=(_src_)[M9_32],(_dst_)[M16_13]=(_src_)[M9_13], \
   (_dst_)[M16_23]=(_src_)[M9_23],(_dst_)[M16_33]=(_src_)[M9_33], \
   (_dst_)[M16_X]=(_dst_)[M16_Y]=(_dst_)[M16_Z]=0

// build M16 from M12
#define M16FromM12(_dst_,_m12_) (_dst_)[M16_14]=(_dst_)[M16_24]= \
   (_dst_)[M16_34]=0,(_dst_)[M16_44]=1,(_dst_)[M16_11]=(_m12_)[M12_11], \
   (_dst_)[M16_21]=(_m12_)[M12_21],(_dst_)[M16_31]=(_m12_)[M12_31], \
   (_dst_)[M16_12]=(_m12_)[M12_12],(_dst_)[M16_22]=(_m12_)[M12_22], \
   (_dst_)[M16_32]=(_m12_)[M12_32],(_dst_)[M16_13]=(_m12_)[M12_13], \
   (_dst_)[M16_23]=(_m12_)[M12_23],(_dst_)[M16_33]=(_m12_)[M12_33], \
   (_dst_)[M16_X]=(_m12_)[M12_X],(_dst_)[M16_Y]=(_m12_)[M12_Y], \
   (_dst_)[M16_Z]=(_m12_)[M12_Z]

// build M12 from M16 -- truncates
#define M12FromM16(_m12_,_m16_) \
   (_m12_)[M12_11]=(_m16_)[M16_11], \
   (_m12_)[M12_12]=(_m16_)[M16_12], \
   (_m12_)[M12_13]=(_m16_)[M16_13], \
   (_m12_)[M12_21]=(_m16_)[M16_21], \
   (_m12_)[M12_22]=(_m16_)[M16_22], \
   (_m12_)[M12_23]=(_m16_)[M16_23], \
   (_m12_)[M12_31]=(_m16_)[M16_31], \
   (_m12_)[M12_32]=(_m16_)[M16_32], \
   (_m12_)[M12_33]=(_m16_)[M16_33], \
   (_m12_)[M12_X] =(_m16_)[M16_X], \
   (_m12_)[M12_Y] =(_m16_)[M16_Y], \
   (_m12_)[M12_Z] =(_m16_)[M16_Z]

// build M12 from M9
#define M12FromM9(_m12_,_m9_) \
   (_m12_)[M12_11]=(_m9_)[M9_11], \
   (_m12_)[M12_12]=(_m9_)[M9_12], \
   (_m12_)[M12_13]=(_m9_)[M9_13], \
   (_m12_)[M12_21]=(_m9_)[M9_21], \
   (_m12_)[M12_22]=(_m9_)[M9_22], \
   (_m12_)[M12_23]=(_m9_)[M9_23], \
   (_m12_)[M12_31]=(_m9_)[M9_31], \
   (_m12_)[M12_32]=(_m9_)[M9_32], \
   (_m12_)[M12_33]=(_m9_)[M9_33], \
   (_m12_)[M12_X] =(_m12_)[M12_Y] =(_m12_)[M12_Z]=0

#define M9Id(_m_) (_m_)[M9_11]=(_m_)[M9_22]= \
   (_m_)[M9_33]=1,(_m_)[M9_21]=(_m_)[M9_31]= \
   (_m_)[M9_12]=(_m_)[M9_32]=(_m_)[M9_13]=(_m_)[M9_23]=0

#define M12Id(_m_) (_m_)[M12_11]=(_m_)[M12_22]= \
   (_m_)[M12_33]=1,(_m_)[M12_21]=(_m_)[M12_31]= \
   (_m_)[M12_12]=(_m_)[M12_32]=(_m_)[M12_13]=(_m_)[M12_23]= \
   (_m_)[M12_X]=(_m_)[M12_Y]=(_m_)[M12_Z]=0

#define M16Id(_m_) \
   (_m_)[M16_11]=(_m_)[M16_22]=(_m_)[M16_33]=(_m_)[M16_44]=1, \
   (_m_)[M16_12]=(_m_)[M16_13]=(_m_)[M16_14]= \
   (_m_)[M16_21]=(_m_)[M16_23]=(_m_)[M16_24]= \
   (_m_)[M16_31]=(_m_)[M16_32]=(_m_)[M16_34]= \
   (_m_)[M16_41]=(_m_)[M16_42]=(_m_)[M16_43]=0

#define M9SetScale(_m_,_sx_,_sy_,_sz_) \
      (_m_)[M9_12]=(_m_)[M9_13]= \
      (_m_)[M9_21]=(_m_)[M9_23]=(_m_)[M9_31]=(_m_)[M9_32]=0, \
      (_m_)[M9_11]=(_sx_),(_m_)[M9_22]=(_sy_),(_m_)[M9_33]=(_sz_)

#define M12SetScale(_m_,_sx_,_sy_,_sz_) \
      (_m_)[M12_X]=(_m_)[M12_Y]=(_m_)[M12_Z]=(_m_)[M12_12]=(_m_)[M12_13]= \
      (_m_)[M12_21]=(_m_)[M12_23]=(_m_)[M12_31]=(_m_)[M12_32]=0, \
      (_m_)[M12_11]=(_sx_),(_m_)[M12_22]=(_sy_),(_m_)[M12_33]=(_sz_)

// transpose -- works w/same dst & src
#define M9Transpose(_d_,_m_,_tmp_) \
   (_d_)[M9_11]=(_m_)[M9_11],(_d_)[M9_22]=(_m_)[M9_22],(_d_)[M9_33]=(_m_)[M9_33], \
   (_tmp_)=(_m_)[M9_12],(_d_)[M9_12]=(_m_)[M9_21],(_d_)[M9_21]=(_tmp_), \
   (_tmp_)=(_m_)[M9_13],(_d_)[M9_13]=(_m_)[M9_31],(_d_)[M9_31]=(_tmp_), \
   (_tmp_)=(_m_)[M9_23],(_d_)[M9_23]=(_m_)[M9_32],(_d_)[M9_32]=(_tmp_)

#define QuatId(_d_) (_d_)[0]=(_d_)[1]=(_d_)[2]=0,(_d_)[3]=1

typedef struct {float m[2];} glueXYf;
#define XYCpyf(_d_,_s_) *((glueXYf*)(_d_))=*((glueXYf*)(_s_))

typedef struct {float m[3];} glueVecf;
#define VecCpyf(_d_,_s_) *((glueVecf*)(_d_))=*((glueVecf*)(_s_))

typedef struct {double m[3];} glueVecd;
#define VecCpyd(_d_,_s_) *((glueVecd*)(_d_))=*((glueVecd*)(_s_))

typedef struct {float m[9];} glueM9f;
#define M9Cpyf(_d_,_s_) *((glueM9f*)(_d_))=*((glueM9f*)(_s_))

typedef struct {float m[12];} glueM12f;
#define M12Cpyf(_d_,_s_) *((glueM12f*)(_d_))=*((glueM12f*)(_s_))

typedef struct {float m[16];} glueM16f;
#define M16Cpyf(_d_,_s_) *((glueM16f*)(_d_))=*((glueM16f*)(_s_))

typedef struct {float m[4];} glueQuatf;
#define QuatCpyf(_d_,_s_) *((glueQuatf*)(_d_))=*((glueQuatf*)(_s_))

SPF_API float XYNormalizef(float *xydst,float *xy); // normalize into dst
SPF_API float VecLenf(float *xyz);
SPF_API double VecLend(double *xyz);
SPF_API float VecLen2f(float *xyz); // squared len
SPF_API double VecLen2d(double *xyz); // squared len
SPF_API float VecNormalizef(float *dst,float *xyz); // normalize into dst
SPF_API double VecNormalized(double *dst,double *xyz); // normalize into dst
SPF_API void glueM9BuildDirUpf(float *m9,float *dir,float *up);
SPF_API int glueCamLookAtM12f(float *m12,float *xyzEye,float *xyzCenter,float *xyzUp);
SPF_API int glueCamPosDirUpM12f(float *m12,float *xyzEye,float *xyzDir,float *xyzUp);
SPF_API int glueCamDirUpFromM9f(float *dir,float *up,float *m9p);
SPF_API int glueM9Invf(float *dst,float *src);
SPF_API int glueM12Invf(float *dst,float *src);
SPF_API void glueM12Mulf(float *dst,float *cur,float *cat);
SPF_API void glueM16Mulf(float *dst,float *cur,float *cat);
SPF_API float glueMathCosSinf(float rad,float *sinp);
SPF_API void glueM9SetRotXf(float *mat3,float rad);
SPF_API void glueM9SetRotYf(float *mat3,float rad);
SPF_API void glueM9SetRotZf(float *mat3,float rad);
SPF_API void glueM9Buildf(float *m9,float *scale,float *rot);
SPF_API void glueM9FromQuatf(float *m,float *quat);
SPF_API void glueQuatFromM9f(float *dst,float *mat9);
SPF_API void glueQuatBuildAroundAxisf(float *dst,float rad,float x,float y,float z);
SPF_API void glueQuatBuildAngf(float *dst,float xa,float ya,float za);
SPF_API void glueQuatMulf(float *dst,float *qCur,float *qCat);
SPF_API void glueQuatVecAddf(float *dst,float *cur,float *vec);
SPF_API void glueQuatVecAddScaledf(float *dst,float *cur,float *vec,float scale);
SPF_API void glueOrtho(float *dst16,float left,float right,float bottom,float top,float znear,float zfar);
SPF_API void glueFrustum(float *dst16,float left,float right,float bottom,float top,float znear,float zfar);
SPF_API void gluePerspective(float *dst16,float fovyHalfRad,float wDivH,float nearClip,float farClip);

///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus  // C in C++
}                   //
#endif              //

#endif // SPMATH_H
