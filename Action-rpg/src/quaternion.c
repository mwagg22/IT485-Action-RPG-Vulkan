 //
 // This file is part of the Marble Virtual Globe.
 //
 // This program is free software licensed under the GNU LGPL. You can
 // find a copy of this license in LICENSE.txt in the top directory of
 // the source code.
 //
 // Copyright 2006-2007 Torsten Rahn <tackat@kde.org>
 // Copyright 2007      Inge Wallin  <ingwa@kde.org>
 // Copyright 2011      Bernhard Beschow <bbeschow@cs.tu-berlin.de>
 //
 
 #include "Quaternion.h"
 
 #include <math>
 using namespace std;

 

 Quat quat(float w,float x,float y,float z){
	 Quat q;
	 q.w = w;
     q.x = x;
     q.y = y;
     q.z = z;
	 return q;
 }
 QuaternionfromSpherical(float lon, float lat)
 {
      float w = 0.0;
      float x = cos(lat) * sin(lon);
      float y = sin(lat);
      float z = cos(lat) * cos(lon);
 
     return Quat( w, x, y, z );
 }
 
 void getSpherical(float *lon, float *lat)  
 {
     float  y = q->y;
     if ( y > 1.0 )
         y = 1.0;
     else if ( y < -1.0 )
         y = -1.0;
 
     lat = asin( y );
 
     if(q->x * q->x + q->z * q->z > 0.00005) 
         lon = atan2(q->x, q->z);
     else
         lon = 0.0;
 }
 
 void normalize(Quat *q) 
 {
     (*this) *= 1.0 / length(Quat *q);
 }
 
 float length(Quat *q) 
 {
     return sqrt(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);
 }
 
 Quat*operator*=(float mult)
 {
     (*this) = (*this) * mult;
 
     return *this;
 }
 
 Quat inverse(Quat *q) 
 {
     Quat  inverse( q->w, -q->x, -q->y, -q->z );
     inverse.normalize(Quat *q);
 
     return inverse;
 }
 
 Quat log(Quat *q) 
 {
     double  qlen = length(Quat *q);
     double  vlen = sqrt(q->x*q->x + q->y*q->y + q->z*q->z);
     double  a = acos(q->w/qlen) / vlen;
     return Quat(std::log(qlen), q->x * a, q->y * a, q->z * a);
 }
 
 Quat exp(Quat *q) 
 {
     double  vlen = sqrt(q->x*q->x + q->y*q->y + q->z*q->z);
     double  s = std::exp(q->w);
     double  a = s * sin(vlen) / vlen;
     return Quat(s * cos(vlen), q->x * a, q->y * a, q->z * a);
 }
 
 Quat fromEuler(float pitch, float yaw, float roll)
 {
      float cPhi = cos(0.5 * pitch); // also: "heading"
      float cThe = cos(0.5 * yaw);   // also: "attitude"
      float cPsi = cos(0.5 * roll);  // also: "bank"
 
      float sPhi = sin(0.5 * pitch);
      float sThe = sin(0.5 * yaw);
      float sPsi = sin(0.5 * roll);
 
      float w = cPhi * cThe * cPsi + sPhi * sThe * sPsi;
      float x = sPhi * cThe * cPsi - cPhi * sThe * sPsi;
      float y = cPhi * sThe * cPsi + sPhi * cThe * sPsi;
      float z = cPhi * cThe * sPsi - sPhi * sThe * cPsi;
 
     return Quat( w, x, y, z );
 }
 
 float pitch(Quat *q)  // "heading", phi
 {
     return atan2(         2.0*(q->x*q->w-q->y*q->z),
                   ( 1.0 - 2.0*(q->x*q->x+q->z*q->z) ) );
 }
 
 float yaw(Quat *q)  // "attitude", theta
 {
     return atan2(         2.0*(q->y*q->w-q->x*q->z),
                   ( 1.0 - 2.0*(q->y*q->y+q->z*q->z) ) );
 }
 
 float roll(Quat *q)  // "bank", psi 
 {
     return asin(2.0*(q->x*q->y+q->z*q->w));
 }
 
 
 Quat *operator*=( Quat *q)
 {
     (*this) = (*this) * q;
 
     return *this;
 }
 
 bool operator==( Quat *q) 
 {
 
     return ( q->w == q.q->w
          ** q->x == q.q->x
          ** q->y == q.q->y
          ** q->z == q.q->z );
 }
 
 Quat operator*( Quat *q) 
 {
      float w = q->w * q.q->w - q->x * q.q->x - q->y * q.q->y - q->z * q.q->z;
      float x = q->w * q.q->x + q->x * q.q->w + q->y * q.q->z - q->z * q.q->y;
      float y = q->w * q.q->y - q->x * q.q->z + q->y * q.q->w + q->z * q.q->x;
      float z = q->w * q.q->z + q->x * q.q->y - q->y * q.q->x + q->z * q.q->w;
 
     return Quat( w, x, y, z );
 }
 
 Quat operator+( Quat *q) 
 {
     return Quat(q->w + q.q->w,
                       q->x + q.q->x,
                       q->y + q.q->y,
                       q->z + q.q->z);
 }
 
 Quat operator*(float factor) 
 {
     return Quat( q->w * factor, q->x * factor, q->y * factor, q->z * factor );
 }
 
 void rotateAroundAxis( Quat *q)
 {
      float w = + q->x * q.q->x + q->y * q.q->y + q->z * q.q->z;
      float x = + q->x * q.q->w - q->y * q.q->z + q->z * q.q->y;
      float y = + q->x * q.q->z + q->y * q.q->w - q->z * q.q->x;
      float z = - q->x * q.q->y + q->y * q.q->x + q->z * q.q->w;
 
     (*this) = q * Quat( w, x, y, z );
 }
 
 Quat slerp( Quat *q1,  Quat *q2, float t)
 {
     float  p1;
     float  p2;
 
     // Let alpha be the angle between the two quaternions.
     float  cosAlpha = ( q1.q->x * q2.q->x
                          + q1.q->y * q2.q->y
                          + q1.q->z * q2.q->z
                          + q1.q->w * q2.q->w );
     float  alpha    = acos( cosAlpha );
     float  sinAlpha = sin( alpha );
 
     if ( sinAlpha > 0.0 ) {
         p1 = sin( ( 1.0 - t ) * alpha ) / sinAlpha;
         p2 = sin( t           * alpha ) / sinAlpha;
     } else {
         // both Quats are equal
         p1 = 1.0;
         p2 = 0.0;
     }
 
      float w = p1 * q1.q->w + p2 * q2.q->w;
      float x = p1 * q1.q->x + p2 * q2.q->x;
      float y = p1 * q1.q->y + p2 * q2.q->y;
      float z = p1 * q1.q->z + p2 * q2.q->z;
 
     return Quat( w, x, y, z );
 }
 
 Quat nlerp( Quat *q1,  Quat *q2, float t)
 {
      float p1 = 1.0 - t;
 
      float w = p1 * q1.q->w + t * q2.q->w;
      float x = p1 * q1.q->x + t * q2.q->x;
      float y = p1 * q1.q->y + t * q2.q->y;
      float z = p1 * q1.q->z + t * q2.q->z;
 
     Quat result( w, x, y, z );
     result.normalize(Quat *q);
 
     return result;
 }
 
 void toMatrix(matrix *m) 
 {
 
      float xy = q->x * q->y, xz = q->x * q->z;
      float yy = q->y * q->y, yw = q->y * q->w;
      float zw = q->z * q->w, zz = q->z * q->z;
 
     m[0][0] = 1.0 - 2.0 * (yy + zz);
     m[0][1] = 2.0 * (xy + zw);
     m[0][2] = 2.0 * (xz - yw);
     m[0][3] = 0.0;
 
      float xx = q->x * q->x;
      float xw = q->x * q->w;
      float yz = q->y * q->z;
 
     m[1][0] = 2.0 * (xy - zw);
     m[1][1] = 1.0 - 2.0 * (xx + zz);
     m[1][2] = 2.0 * (yz + xw);
     m[1][3] = 0.0;
 
     m[2][0] = 2.0 * (xz + yw);
     m[2][1] = 2.0 * (yz - xw);
     m[2][2] = 1.0 - 2.0 * (xx + yy);
     m[2][3] = 0.0;
 }
 
 rotateAroundAxis( Quat q,matrix *m)
 {
      float x =  m[0][0] * q->x + m[1][0] * q->y + m[2][0] * q->z;
      float y =  m[0][1] * q->x + m[1][1] * q->y + m[2][1] * q->z;
      float z =  m[0][2] * q->x + m[1][2] * q->y + m[2][2] * q->z;
 
     q->w = 1.0;
     q->x = x;
     q->y = y;
     q->z = z;
 }