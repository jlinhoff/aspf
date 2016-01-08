// Copyright (C) 2015 Joe Linhoff - see license
#include "spmath.h"
#include <math.h>

float glueYUP[] = {0,1,0};

// JFL 07 Dec 10
float XYNormalizef(float *dst,float *xy)
{
   float a,b;
   a=xy[0]*xy[0];
   a+=xy[1]*xy[1];
   if(!(a=sqrtf(a))) b=0,a=1;
   else b=a,a=1.0f/a;
   dst[0]=xy[0]*a;
   dst[1]=xy[1]*a;
   return b;
} // XYNormalizef()

// JFL 31 Oct 06
float VecLenf(float *xyz)
{
   float a;
   a=xyz[0]*xyz[0];
   a+=xyz[1]*xyz[1];
   a+=xyz[2]*xyz[2];
   return sqrt(a);
} // VecLenf()

// JFL 29 Nov 13
double VecLend(double *xyz)
{
   double a;
   a=xyz[0]*xyz[0];
   a+=xyz[1]*xyz[1];
   a+=xyz[2]*xyz[2];
   return sqrt(a);
} // VecLend()

// JFL 31 Oct 06
// JFL 13 Jul 11
float VecLen2f(float *xyz)
{
   float a;
   a=xyz[0]*xyz[0];
   a+=xyz[1]*xyz[1];
   a+=xyz[2]*xyz[2];
   return a;
} // VecLen2f()

// JFL 29 Nov 13
double VecLen2d(double *xyz)
{
   double a;
   a=xyz[0]*xyz[0];
   a+=xyz[1]*xyz[1];
   a+=xyz[2]*xyz[2];
   return a;
} // VecLen2d()

// JFL 18 Aug 06
// JFL 15 Oct 07
// JFL 07 Aug 11; dst
float VecNormalizef(float *dst,float *vec)
{
   float a,aa;
   a=vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2];
   if(a==1)
   {
      VecCpyf(dst,vec);
      return 1;
   }
   if(!(a=sqrt(a)))
      return 0;
   aa=1/a;
   dst[0]=aa*vec[0];
   dst[1]=aa*vec[1];
   dst[2]=aa*vec[2];
   return a;
} // VecNormalizef()

// JFL 29 Dec 13
double VecNormalized(double *dst,double *vec)
{
   double a,aa;
   a=vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2];
   if(a==1)
   {
      VecCpyd(dst,vec);
      return 1;
   }
   if(!(a=sqrt(a)))
      return 0;
   aa=1/a;
   dst[0]=aa*vec[0];
   dst[1]=aa*vec[1];
   dst[2]=aa*vec[2];
   return a;
} // VecNormalized()

// JFL 26 Oct 11
// JFL 11 Sep 12; flipped
void glueM9BuildDirUpf(float *m9,float *dir,float *up)
{
   float c[3],d[3],u[3];

   VecNormalizef(d,dir);
   
   VecCross(c,up,d);
   VecNormalizef(c,c);

   VecCross(u,d,c);

   m9[M9_11]=c[0];
   m9[M9_21]=c[1];
   m9[M9_31]=c[2];

   m9[M9_12]=u[0];
   m9[M9_22]=u[1];
   m9[M9_32]=u[2];

   m9[M9_13]=d[0];
   m9[M9_23]=d[1];
   m9[M9_33]=d[2];

} // glueM9BuildDirUpf()

// JFL 10 Nov 06
// JFL 28 Dec 06; vectors
int glueCamLookAtM12f(float *m12,float *xyzEye,float *xyzCenter,float *xyzUp)
{
   float c[3],d[3],u[3];

   if(!xyzUp)
      xyzUp=glueYUP;

   SET3(d,xyzCenter[0]-xyzEye[0],xyzCenter[1]-xyzEye[1],xyzCenter[2]-xyzEye[2]);
   VecNormalizef(d,d);

   VecCross(c,d,xyzUp);
   VecNormalizef(c,c);

   VecCross(u,c,d);

   m12[M12_11]=c[0];
   m12[M12_21]=c[1];
   m12[M12_31]=c[2];

   m12[M12_12]=u[0];
   m12[M12_22]=u[1];
   m12[M12_32]=u[2];

   m12[M12_13]=-d[0];
   m12[M12_23]=-d[1];
   m12[M12_33]=-d[2];

   SET3(d,-xyzEye[0],-xyzEye[1],-xyzEye[2]);
   VecRotM9(m12+M12_X,d,m12+M12_11);

   return 0;
} // glueCamLookAtM12f()

// JFL 10 Nov 06
// JFL 28 Dec 06; vectors
int glueCamPosDirUpM12f(float *m12,float *xyzEye,float *xyzDir,float *xyzUp)
{
   float c[3],d[3],u[3];

   if(!xyzUp)
      xyzUp=glueYUP;

   CPY3(d,xyzDir);

   VecCross(c,d,xyzUp);
   VecNormalizef(c,c);

   VecCross(u,c,d);

   m12[M12_11]=c[0];
   m12[M12_21]=c[1];
   m12[M12_31]=c[2];

   m12[M12_12]=u[0];
   m12[M12_22]=u[1];
   m12[M12_32]=u[2];

   m12[M12_13]=-d[0];
   m12[M12_23]=-d[1];
   m12[M12_33]=-d[2];

   SET3(d,-xyzEye[0],-xyzEye[1],-xyzEye[2]);
   VecRotM9(m12+M12_X,d,m12+M12_11);

   return 0;
} // glueCamPosDirUpM12f()

// JFL 26 Jan 12
int glueCamDirUpFromM9f(float *dir,float *up,float *m9p)
{
   dir[0]=-m9p[M9_13];
   dir[1]=-m9p[M9_23];
   dir[2]=-m9p[M9_33];

   up[0]=m9p[M9_12];
   up[1]=m9p[M9_22];
   up[2]=m9p[M9_32];

   return 0;
} // glueCamDirUpFromM9f()

// JFL 18 Aug 06
void glueM9Mulf(float *dst,float *cur,float *cat)
{
   float tmp[9],*sav=0;

   if((dst==cur)||(dst==cat))
      sav=dst,dst=tmp;

   dst[0]=cat[0]*cur[0]+cat[1]*cur[3]+cat[2]*cur[6];
   dst[1]=cat[0]*cur[1]+cat[1]*cur[4]+cat[2]*cur[7];
   dst[2]=cat[0]*cur[2]+cat[1]*cur[5]+cat[2]*cur[8];
   dst[3]=cat[3]*cur[0]+cat[4]*cur[3]+cat[5]*cur[6];
   dst[4]=cat[3]*cur[1]+cat[4]*cur[4]+cat[5]*cur[7];
   dst[5]=cat[3]*cur[2]+cat[4]*cur[5]+cat[5]*cur[8];
   dst[6]=cat[6]*cur[0]+cat[7]*cur[3]+cat[8]*cur[6];
   dst[7]=cat[6]*cur[1]+cat[7]*cur[4]+cat[8]*cur[7];
   dst[8]=cat[6]*cur[2]+cat[7]*cur[5]+cat[8]*cur[8];

   #if 0
   uint8_t i,j;
         dst[i*3+j]=cat[i*3+0]*cur[0*3+j]
         +cat[i*3+1]*cur[1*3+j]
         +cat[i*3+2]*cur[2*3+j];

   for(i=0;i<9;i+=3)
      for(j=0;j<3;j++)
         dst[i+j]=cat[i]*cur[j]+cat[i+1]*cur[j+3]+cat[i+2]*cur[j+6];
   #endif

   if(sav)
      M9Cpyf(sav,tmp);
} // glueM9Mulf()

// JFL 05 Aug 06
// JFL 02 Nov 07; return error
int glueM9Invf(float *dst,float *src)
{
	float det;
   float savmem[M9_SIZE],*sav=0;
	
	if(dst==src)
      sav=dst,dst=savmem;

   det = src[M9_11] * (src[M9_22] * src[M9_33] - src[M9_32] * src[M9_23])
      - src[M9_21] * (src[M9_12] * src[M9_33] - src[M9_32] * src[M9_13])
      + src[M9_31] * (src[M9_12] * src[M9_23] - src[M9_22] * src[M9_13]);
   if(!det)
      return -1;
   det = 1.0 / det;
   dst[M9_11] =   det * (src[M9_22] * src[M9_33] - src[M9_32] * src[M9_23]);
   dst[M9_21] = - det * (src[M9_21] * src[M9_33] - src[M9_31] * src[M9_23]);
   dst[M9_31] =   det * (src[M9_21] * src[M9_32] - src[M9_31] * src[M9_22]);
   dst[M9_12] = - det * (src[M9_12] * src[M9_33] - src[M9_32] * src[M9_13]);
   dst[M9_22] =   det * (src[M9_11] * src[M9_33] - src[M9_31] * src[M9_13]);
   dst[M9_32] = - det * (src[M9_11] * src[M9_32] - src[M9_31] * src[M9_12]);
   dst[M9_13] =   det * (src[M9_12] * src[M9_23] - src[M9_22] * src[M9_13]);
   dst[M9_23] = - det * (src[M9_11] * src[M9_23] - src[M9_21] * src[M9_13]);
   dst[M9_33] =   det * (src[M9_11] * src[M9_22] - src[M9_21] * src[M9_12]);

	if(sav)
		M9Cpyf(sav,savmem);

   return 0;
} // glueM9Invf()

// JFL 05 Aug 06
// JFL 01 Sep 07
int glueM12Invf(float *dst,float *src)
{
	float det;
   float savmem[M12_SIZE],*sav=0;

	if(dst==src)
      sav=dst,dst=savmem;

	/*
	 * ANSI C code from the article
	 * "Fast Inversion of Length- and Angle-Preserving Matrices"
	 * by Kevin Wu, Kevin.Wu@eng.sun.com
	 * in "Graphics Gems IV", Academic Press, 1994
	 */

	/*
	 *
	 * angle_preserving_matrix_inverse
	 *
	 * Computes the inverse of a 3-D angle-preserving matrix.
	 *
	 * This procedure treats the 4 by 4 angle-preserving matrix as a block
	 * matrix and calculates the inverse of one submatrix for a significant
	 * performance improvement over a general procedure that can invert any
	 * nonsingular matrix:
	 *
	 *  ------          ------
	 * | A   C| -1  =  |      |
	 * | 0   1|        |      |
	 *  ------          ------
	 *
	 * where      M is a 4 by 4 angle-preserving matrix,
	 *	      A is the 3 by 3 upper-left submatrix of M,
	 *	      C is the 3 by 1 upper-right submatrix of M.
	 *
	 */

   det = src[M12_11] * (src[M12_22] * src[M12_33] - src[M12_32] * src[M12_23])
      - src[M12_21] * (src[M12_12] * src[M12_33] - src[M12_32] * src[M12_13])
      + src[M12_31] * (src[M12_12] * src[M12_23] - src[M12_22] * src[M12_13]);
   if(!det)
      return -1;
   det = 1.0 / det;
   dst[M12_11] =   det * (src[M12_22] * src[M12_33] - src[M12_32] * src[M12_23]);
   dst[M12_21] = - det * (src[M12_21] * src[M12_33] - src[M12_31] * src[M12_23]);
   dst[M12_31] =   det * (src[M12_21] * src[M12_32] - src[M12_31] * src[M12_22]);
   dst[M12_12] = - det * (src[M12_12] * src[M12_33] - src[M12_32] * src[M12_13]);
   dst[M12_22] =   det * (src[M12_11] * src[M12_33] - src[M12_31] * src[M12_13]);
   dst[M12_32] = - det * (src[M12_11] * src[M12_32] - src[M12_31] * src[M12_12]);
   dst[M12_13] =   det * (src[M12_12] * src[M12_23] - src[M12_22] * src[M12_13]);
   dst[M12_23] = - det * (src[M12_11] * src[M12_23] - src[M12_21] * src[M12_13]);
   dst[M12_33] =   det * (src[M12_11] * src[M12_22] - src[M12_21] * src[M12_12]);
   dst[M12_X] = -(src[M12_X] * dst[M12_11] + src[M12_Y] * dst[M12_21] + src[M12_Z] * dst[M12_31]);
   dst[M12_Y] = -(src[M12_X] * dst[M12_12] + src[M12_Y] * dst[M12_22] + src[M12_Z] * dst[M12_32]);
   dst[M12_Z] = -(src[M12_X] * dst[M12_13] + src[M12_Y] * dst[M12_23] + src[M12_Z] * dst[M12_33]);

	if(sav)
		M12Cpyf(sav,savmem);

   return 0;
} // glueM12Invf()

// JFL 12 Oct 06
// JFL 13 Dec 06
void glueM12Mulf(float *dst,float *cur,float *cat)
{
   float tmp[12],*sav=0;
   if((dst==cur)||(dst==cat))
      sav=dst,dst=tmp;

   dst+=3;
   cat+=3;
   cur+=3;

   dst[0]=cat[0]*cur[0]+cat[1]*cur[3]+cat[2]*cur[6];
   dst[1]=cat[0]*cur[1]+cat[1]*cur[4]+cat[2]*cur[7];
   dst[2]=cat[0]*cur[2]+cat[1]*cur[5]+cat[2]*cur[8];
   dst[3]=cat[3]*cur[0]+cat[4]*cur[3]+cat[5]*cur[6];
   dst[4]=cat[3]*cur[1]+cat[4]*cur[4]+cat[5]*cur[7];
   dst[5]=cat[3]*cur[2]+cat[4]*cur[5]+cat[5]*cur[8];
   dst[6]=cat[6]*cur[0]+cat[7]*cur[3]+cat[8]*cur[6];
   dst[7]=cat[6]*cur[1]+cat[7]*cur[4]+cat[8]*cur[7];
   dst[8]=cat[6]*cur[2]+cat[7]*cur[5]+cat[8]*cur[8];

   dst-=3;
   cat-=3;
   cur-=3;

   dst[M12_X]=cur[M12_11]*cat[M12_X]
      +cur[M12_21]*cat[M12_Y]+cur[M12_31]*cat[M12_Z]+cur[M12_X];
   dst[M12_Y]=cur[M12_12]*cat[M12_X]
      +cur[M12_22]*cat[M12_Y]+cur[M12_32]*cat[M12_Z]+cur[M12_Y];
   dst[M12_Z]=cur[M12_13]*cat[M12_X]
      +cur[M12_23]*cat[M12_Y]+cur[M12_33]*cat[M12_Z]+cur[M12_Z];

   if(sav)
      M12Cpyf(sav,tmp);
} // glueM12Mulf()

// JFL 05 Aug 06
// JFL 12 Oct 06
void glueM16Mulf(float *dst,float *cur,float *cat)
{
   uint8_t i,j;
   float tmp[16],*sav=0;
   if((dst==cur)||(dst==cat))
      sav=dst,dst=tmp;

   for(i=0;i<4;i++)
      for(j=0;j<4;j++)
      dst[i*4+j]=cat[i*4+0]*cur[0*4+j]
         +cat[i*4+1]*cur[1*4+j]
         +cat[i*4+2]*cur[2*4+j]
         +cat[i*4+3]*cur[3*4+j];

   if(sav)
      M16Cpyf(sav,tmp);
} // glueM16Mulf()

// JFL 26 Oct 06
// JFL 02 Nov 07; return error
int glueM16Invf(float *dst,float *src)
{
	float det;
   float savmem[M16_SIZE],*sav=0;
	
	if(dst==src)
      sav=dst,dst=savmem;

	/*
	 * ANSI C code from the article
	 * "Fast Inversion of Length- and Angle-Preserving Matrices"
	 * by Kevin Wu, Kevin.Wu@eng.sun.com
	 * in "Graphics Gems IV", Academic Press, 1994
	 */

	/*
	 *
	 * angle_preserving_matrix_inverse
	 *
	 * Computes the inverse of a 3-D angle-preserving matrix.
	 *
	 * This procedure treats the 4 by 4 angle-preserving matrix as a block
	 * matrix and calculates the inverse of one submatrix for a significant
	 * performance improvement over a general procedure that can invert any
	 * nonsingular matrix:
	 *
	 *  ------          ------
	 * | A   C| -1  =  |      |
	 * | 0   1|        |      |
	 *  ------          ------
	 *
	 * where      M is a 4 by 4 angle-preserving matrix,
	 *	      A is the 3 by 3 upper-left submatrix of M,
	 *	      C is the 3 by 1 upper-right submatrix of M.
	 *
	 */

   det = src[0] * (src[5] * src[10] - src[9] * src[6])
      - src[4] * (src[1] * src[10] - src[9] * src[2])
      + src[8] * (src[1] * src[ 6] - src[5] * src[2]);
   if(!det)
      return -1;
   det = 1.0 / det;
   dst[ 0] =   det * (src[5] * src[10] - src[9] * src[6]);
   dst[ 4] = - det * (src[4] * src[10] - src[8] * src[6]);
   dst[ 8] =   det * (src[4] * src[ 9] - src[8] * src[5]);
   dst[ 1] = - det * (src[1] * src[10] - src[9] * src[2]);
   dst[ 5] =   det * (src[0] * src[10] - src[8] * src[2]);
   dst[ 9] = - det * (src[0] * src[ 9] - src[8] * src[1]);
   dst[ 2] =   det * (src[1] * src[ 6] - src[5] * src[2]);
   dst[ 6] = - det * (src[0] * src[ 6] - src[4] * src[2]);
   dst[10] =   det * (src[0] * src[ 5] - src[4] * src[1]);
   dst[ 3] = 0.0f;
   dst[ 7] = 0.0f;
   dst[11] = 0.0f;
   dst[15] = 1.0f;
   dst[12] = -(src[12] * dst[0] + src[13] * dst[4] + src[14] * dst[ 8]);
   dst[13] = -(src[12] * dst[1] + src[13] * dst[5] + src[14] * dst[ 9]);
   dst[14] = -(src[12] * dst[2] + src[13] * dst[6] + src[14] * dst[10]);

	if(sav)
		M16Cpyf(sav,savmem);

   return 0;
} // glueM16Invf()

// JFL 08 Aug 06
float glueMathCosSinf(float rad,float *sinp)
{
   if(sinp)
      *sinp=sin(rad);
   return cos(rad);
} // glueMathCosSinf()

// JFL 03 Aug 06
void glueM9SetRotXf(float *mat3,float rad)
{
   float s,c;
   c=glueMathCosSinf(rad,&s);

   // from RB755
   // 1  0  0   11 21 31
   // 0  c -s   12 22 32
   // 0  s  c   13 23 33
   mat3[M9_11]=1;
   mat3[M9_12]=mat3[M9_13]=mat3[M9_21]=mat3[M9_31]=0;
   mat3[M9_22]=c;
   mat3[M9_32]=-s;
   mat3[M9_23]=s;
   mat3[M9_33]=c;
} // glueM9SetRotXf()

// JFL 03 Aug 06
void glueM9SetRotYf(float *mat3,float rad)
{
   float s,c;
   c=glueMathCosSinf(rad,&s);

   // from RB755
   //  c  0  s   11 21 31
   //  0  1  0   12 22 32
   // -s  0  c   13 23 33
   mat3[M9_11]=c;
   mat3[M9_31]=s;
   mat3[M9_13]=-s;
   mat3[M9_33]=c;
   mat3[M9_12]=mat3[M9_21]=mat3[M9_23]=mat3[M9_32]=0;
   mat3[M9_22]=1;
} // glueM9SetRotYf()

// JFL 03 Aug 06
void glueM9SetRotZf(float *mat3,float rad)
{
   float s,c;
   c=glueMathCosSinf(rad,&s);

   // from RB755
   //  c -s  0   11 21 31
   //  s  c  0   12 22 32
   //  0  0  1   13 23 33
   mat3[M9_11]=c;
   mat3[M9_21]=-s;
   mat3[M9_12]=s;
   mat3[M9_22]=c;
   mat3[M9_13]=mat3[M9_23]=mat3[M9_31]=mat3[M9_32]=0;
   mat3[M9_33]=1;
} // glueM9SetRotZf()

// JFL 15 Dec 06
// JFL 06 Mar 12
// JFL 18 May 14
void glueM9Buildf(float *m9,float *scale,float *rot)
{
   float d[12],e[12],*aa;

   // create a matrix that can be applied to the current OpenGL matrix
   // that produces the same result as in Maya with xyz rot order

   // -- very inefficient right now --

   if(!rot)
   {
      if(scale)
         M9SetScale(m9,scale[0],scale[1],scale[2]);
      else
         M9Id(m9);
   }
   else
   { // rot
      float A,B,C,D,E,F,AD,BD;
      // simplifird from:
      // glueM9SetRotZf(a,rot[2]);
      // glueM9SetRotYf(b,rot[1]);
      // glueM9SetRotXf(c,rot[0]);
      // glueM9Mulf(d,a,b);
      // glueM9Mulf(b,d,c);
      // glueM9Mulf(m9,m9,b);

      if(scale)
         aa=e;
      else
         aa=m9;

      A = glueMathCosSinf(rot[0],&B);
      C = glueMathCosSinf(rot[1],&D);
      E = glueMathCosSinf(rot[2],&F);
      AD = A*D; BD=B*D;
      aa[M9_11]=C*E;
      aa[M9_12]=C*F;
      aa[M9_13]=-D;

      aa[M9_21]=-A*F+BD*E;
      aa[M9_22]=A*E+BD*F;
      aa[M9_23]=B*C;

      aa[M9_31]=B*F+AD*E;
      aa[M9_32]=-B*E+AD*F;
      aa[M9_33]=A*C;

      if(scale)
      {
         // glScalef(scale[0],scale[1],scale[2]);
         d[M9_12]=d[M9_13]=d[M9_21]=d[M9_23]=d[M9_31]=d[M9_32]=0;
         d[M9_11]=scale[0];
         d[M9_22]=scale[1];
         d[M9_33]=scale[2];
         glueM9Mulf(m9,aa,d);
      }
   } // rot

} // glueM9Buildf()

// JFL 15 Dec 06
// JFL 18 May 14
void glueM12Buildf(float *m12,float *scale,float *rot,float *trans)
{
   // create a matrix that can be applied to the current OpenGL matrix
   // that produces the same result as in Maya with xyz rot order

   if(trans)
   {
      M9Id(m12+3);
      SET3(m12,trans[0],trans[1],trans[2]);
   }
   else
      M12Id(m12);

   if(rot||scale)
      glueM9Buildf(3+m12,scale,rot);

} // glueM12Buildf()


void glueM9FromQuatf(float *m,float *quat)
{
    float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

    // calculate coefficients
    x2 = quat[0] + quat[0]; 
    y2 = quat[1] + quat[1];
    z2 = quat[2] + quat[2];
    xx = quat[0] * x2; 
    xy = quat[0] * y2; 
    xz = quat[0] * z2;
    yy = quat[1] * y2; 
    yz = quat[1] * z2; 
    zz = quat[2] * z2;
    wx = quat[3] * x2; 
    wy = quat[3] * y2; 
    wz = quat[3] * z2;

    m[M9_11] = 1.0 - (yy + zz); 
    m[M9_21] = xy - wz;
    m[M9_31] = xz + wy; 

    m[M9_12] = xy + wz; 
    m[M9_22] = 1.0 - (xx + zz);
    m[M9_32] = yz - wx; 

    m[M9_13] = xz - wy; 
    m[M9_23] = yz + wx;
    m[M9_33] = 1.0 - (xx + yy); 
} // glueM9FromQuatf()


// JFL 28 Mar 10
void glueQuatFromM9f(float *dst,float *mat9)
{
   // from Watt, Advanced Animation and Rendering Techniques 1992, p363
   float trace,s,*m,*q;
   int i,j,k;

   // on paper:
   //  a d g
   //  b e h
   //  c f i
   //            0 1 2 3 4 5 6 7 8
   // in memory: a b c d e f g h i

   trace = mat9[0]+mat9[4]+mat9[8];
   if(trace>0)
   {
      s=sqrt(trace+1);
      dst[3]=s*0.5f;
      s=0.5f/s;
      dst[0]=(mat9[5]-mat9[7])*s;
      dst[1]=(mat9[6]-mat9[2])*s;
      dst[2]=(mat9[1]-mat9[3])*s;
   }
   else
   {
      m=mat9;
      q=dst;
      i=0;
      if(m[4]>m[0]) i=1;
      if(m[8]>m[i<<2]) i=2;
      if((j=i+1)>2)j=0;
      if((k=j+1)>2)k=0;
      s=sqrt((m[i<<2]-(m[j<<2]+m[k<<2]))+1);
      q[i]=s*0.5f;
      s=0.5f/s;
      dst[3]=(m[j*3+k]-m[k*3+j])*s;
      q[j]=(m[i*3+j]+m[j*3+i])*s;
      q[k]=(m[i*3+k]+m[k*3+i])*s;
   }

} // glueQuatFromM9f()

// JFL 05 Aug 06
void glueQuatBuildAroundAxisf(float *dst,float rad,float x,float y,float z)
{
   float res= sin( rad / 2.0f );
	dst[3] = cos( rad / 2.0f );
	dst[0] = x * res;
	dst[1] = y * res;
	dst[2] = z * res;
} // glueQuatBuildAroundAxisf()

// JFL 05 Aug 06
void glueQuatBuildAngf(float *dst,float xa,float ya,float za)
{
   float cr, cp, cy, sr, sp, sy, cpcy, spsy;

   // calculate trig identities
   cr = cos(xa/2);
	cp = cos(ya/2);
	cy = cos(za/2);

	sr = sin(xa/2);
	sp = sin(ya/2);
	sy = sin(za/2);
	
	cpcy = cp * cy;
	spsy = sp * sy;

	dst[3] = cr * cpcy + sr * spsy;
	dst[0] = sr * cpcy - cr * spsy;
	dst[1] = cr * sp * cy + sr * cp * sy;
	dst[2] = cr * cp * sy - sr * sp * cy;
} // glueQuatBuildAngf()

// JFL 05 Aug 06
// JFL 10 Dec 07; sav mem
void glueQuatMulf(float *dst,float *qCur,float *qCat)
{
   float savemem[4],*sav=0;
   if(dst==qCur||dst==qCat)
      sav=dst,dst=savemem;
 	dst[3] = qCur[3]*qCat[3] - qCur[0]*qCat[0] - qCur[1]*qCat[1] - qCur[2]*qCat[2];
	dst[0] = qCur[3]*qCat[0] + qCur[0]*qCat[3] + qCur[1]*qCat[2] - qCur[2]*qCat[1];
	dst[1] = qCur[3]*qCat[1] + qCur[1]*qCat[3] + qCur[2]*qCat[0] - qCur[0]*qCat[2];
	dst[2] = qCur[3]*qCat[2] + qCur[2]*qCat[3] + qCur[0]*qCat[1] - qCur[1]*qCat[0];
   if(sav)
      QuatCpyf(sav,dst);
} // glueQuatMulf()

// JFL 26 Dec 07
void glueQuatVecAddf(float *dst,float *cur,float *vec)
{
   float q[4],qq[4];
   SET4(q,vec[0],vec[1],vec[2],0);
   glueQuatMulf(qq,q,cur);
   dst[0]=cur[0]+qq[0]*0.5;
   dst[1]=cur[1]+qq[1]*0.5;
   dst[2]=cur[2]+qq[2]*0.5;
   dst[3]=cur[3]+qq[3]*0.5;
} // glueQuatVecAddf()

// JFL 10 Dec 07
void glueQuatVecAddScaledf(float *dst,float *cur,float *vec,float scale)
{
   float q[4],qq[4];
   SET4(q,vec[0]*scale,vec[1]*scale,vec[2]*scale,0);
   glueQuatMulf(qq,q,cur);
   dst[0]=cur[0]+qq[0]*0.5;
   dst[1]=cur[1]+qq[1]*0.5;
   dst[2]=cur[2]+qq[2]*0.5;
   dst[3]=cur[3]+qq[3]*0.5;
} // QuatVecAddScaledf()

#define EPSILON 0.0001

// JFL 11 Dec 07
void glueQuatNormalizef(float *dst,float *src)
{
   float d=src[0]*src[0];
   d+=src[1]*src[1];
   d+=src[2]*src[2];
   d+=src[3]*src[3];
   d=sqrt(d);
   if(d<EPSILON)
   {
      dst[0]=dst[1]=dst[2]=0;
      dst[3]=1;
      return;
   }
   d=1.0/d;
   dst[0]=src[0]*d;
   dst[1]=src[1]*d;
   dst[2]=src[2]*d;
   dst[3]=src[3]*d;
} // glueQuatNormalizef()

#define DELTA 0.001
void glueQuatSlerpf(float *res,float *from, float *to, float t)
{
   float to1[4];
   double omega, cosom, sinom, scale0, scale1;

   // calc cosine
   cosom = from[0] * to[0] + from[1] * to[1] + from[2] * to[2]
      + from[3] * to[3];

   // adjust signs (if necessary)
   if ( cosom <0.0 ){ cosom = -cosom; to1[0] = - to[0];
      to1[1] = - to[1];
      to1[2] = - to[2];
      to1[3] = - to[3];
   } else  {
      to1[0] = to[0];
      to1[1] = to[1];
      to1[2] = to[2];
      to1[3] = to[3];
   }

   // calculate coefficients

   if ( (1.0 - cosom) > DELTA ) {
      // standard case (slerp)
      omega = acos(cosom);
      sinom = sin(omega);
      scale0 = sin((1.0 - t) * omega) / sinom;
      scale1 = sin(t * omega) / sinom;
   } else {        
      // "from" and "to" quaternions are very close 
      //  ... so we can do a linear interpolation
      scale0 = 1.0 - t;
      scale1 = t;
   }
   // calculate final values
   res[0] = scale0 * from[0] + scale1 * to1[0];
   res[1] = scale0 * from[1] + scale1 * to1[1];
   res[2] = scale0 * from[2] + scale1 * to1[2];
   res[3] = scale0 * from[3] + scale1 * to1[3];
} // glueQuatSlerpf()


// JFL 22 Mar 11
void glueOrtho(float *dst16,float left,float right,float bottom,float top,float znear,float zfar)
{
   float a = 2.0/(right-left);
   float b = 2.0/(top-bottom);
   float c = -2.0/(zfar-znear);
   float tx = -(right+left)/(right-left);
   float ty = -(top+bottom)/(top-bottom);
   float tz = -(zfar+znear)/(zfar-znear);
   
   dst16[M16_11]=a;
   dst16[M16_22]=b;
   dst16[M16_33]=c;
   dst16[M16_44]=1;

   dst16[M16_41]=tx;
   dst16[M16_42]=ty;
   dst16[M16_43]=tz;

   dst16[M16_12]=
   dst16[M16_13]=
   dst16[M16_14]=
   dst16[M16_21]=
   dst16[M16_23]=
   dst16[M16_24]=
   dst16[M16_31]=
   dst16[M16_32]=
   dst16[M16_34]=0;

} // glueOrtho()

// JFL 22 Mar 11
void glueFrustum(float *dst16,float left,float right,float bottom,float top,float znear,float zfar)
{
   float invrl = 1.0/(right-left);
   float invtb = 1.0/(top-bottom);
   float invfn = 1.0/(znear-zfar);
   float a = (right+left)*invrl;
   float b = (top+bottom)*invtb;
   float c = (zfar+znear)*invfn;
   float d = (2*zfar*znear)*invfn;

   dst16[M16_11]=2*znear*invrl;
   dst16[M16_22]=2*znear*invtb;
   dst16[M16_31]=a;
   dst16[M16_32]=b;
   dst16[M16_33]=c;
   dst16[M16_43]=d;
   dst16[M16_34]=-1;

   dst16[M16_12]=
   dst16[M16_13]=
   dst16[M16_14]=
   dst16[M16_21]=
   dst16[M16_23]=
   dst16[M16_24]=
   dst16[M16_41]=
   dst16[M16_42]=
   dst16[M16_44]=0;
} // glueFrustum()

// JFL 04 Dec 09; same as gluPerspective
void gluePerspective(float *dst16,float fovyHalfRad,float wDivH,
   float nearClip,float farClip)
{
   float nearHeight = tanf(fovyHalfRad)*nearClip;
   float a = nearHeight*wDivH;
   glueFrustum(dst16,-a,a,-nearHeight,nearHeight,nearClip,farClip);
} // gluePerspective()

// EOF
