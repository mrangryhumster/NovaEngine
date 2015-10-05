#ifndef matrix_H
#define matrix_H


#include "NTypes.h"
#include "NMath.h"

#include "vector2.h"
#include "vector3.h"

#include <cstring>

namespace novaengine
{
namespace core
{

template<class T>
class matrix
{
public:

    matrix(bool skip_indetify = false)
    {
        if(!skip_indetify)
            makeIndetify();
    }
    matrix(const matrix<T>& other)
    {
        memcpy(native_matrix, other.native_matrix, 16*sizeof(T));
    }

    matrix(const T* in_native_matrix)
    {
        memcpy(native_matrix, in_native_matrix, 16*sizeof(T));
    }

    virtual ~matrix()
    {

    }

    inline void makeIndetify()
    {
        memset(native_matrix,0,16*sizeof(T));
        native_matrix[0]  = native_matrix[5] = native_matrix[10] = native_matrix[15] = 1;
    }

    bool operator == (const matrix<T> &other ) const
    {
        if(memcmp(native_matrix,other.native_matrix,16*sizeof(T)) != 0)
            return false;
        return true;
    }

    bool operator !=  (const matrix<T> &other ) const
    {
        if(memcmp(native_matrix,other.native_matrix,16*sizeof(T)) == 0)
            return false;
        return false;
    }

    inline matrix<T>& operator= (const matrix<T>& other)
    {
        memcpy(native_matrix, other.native_matrix, 16*sizeof(T));
        return *this;
    }

    inline T& getElement(u32 index)
    {
        return native_matrix[index];
    }

    inline T& getElement(u32 X_index,u32 Y_index)
    {
        return native_matrix[4 * Y_index + X_index];
    }

    inline T& operator[](s32 index)
    {
        return native_matrix[index];
    }

    inline operator T*()
    {
        return native_matrix;
    }

    inline matrix<T> operator+ (const matrix<T>& other) const
    {
        matrix<T> temp;
        temp.native_matrix[0]  = native_matrix[0]+other.native_matrix[0];
        temp.native_matrix[1]  = native_matrix[1]+other.native_matrix[1];
        temp.native_matrix[2]  = native_matrix[2]+other.native_matrix[2];
        temp.native_matrix[3]  = native_matrix[3]+other.native_matrix[3];
        temp.native_matrix[4]  = native_matrix[4]+other.native_matrix[4];
        temp.native_matrix[5]  = native_matrix[5]+other.native_matrix[5];
        temp.native_matrix[6]  = native_matrix[6]+other.native_matrix[6];
        temp.native_matrix[7]  = native_matrix[7]+other.native_matrix[7];
        temp.native_matrix[8]  = native_matrix[8]+other.native_matrix[8];
        temp.native_matrix[9]  = native_matrix[9]+other.native_matrix[9];
        temp.native_matrix[10] = native_matrix[10]+other.native_matrix[10];
        temp.native_matrix[11] = native_matrix[11]+other.native_matrix[11];
        temp.native_matrix[12] = native_matrix[12]+other.native_matrix[12];
        temp.native_matrix[13] = native_matrix[13]+other.native_matrix[13];
        temp.native_matrix[14] = native_matrix[14]+other.native_matrix[14];
        temp.native_matrix[15] = native_matrix[15]+other.native_matrix[15];
        return temp;
    }

    inline matrix<T>& operator+= (const matrix<T>& other)
    {
        native_matrix[0]+=other.native_matrix[0];
        native_matrix[1]+=other.native_matrix[1];
        native_matrix[2]+=other.native_matrix[2];
        native_matrix[3]+=other.native_matrix[3];
        native_matrix[4]+=other.native_matrix[4];
        native_matrix[5]+=other.native_matrix[5];
        native_matrix[6]+=other.native_matrix[6];
        native_matrix[7]+=other.native_matrix[7];
        native_matrix[8]+=other.native_matrix[8];
        native_matrix[9]+=other.native_matrix[9];
        native_matrix[10]+=other.native_matrix[10];
        native_matrix[11]+=other.native_matrix[11];
        native_matrix[12]+=other.native_matrix[12];
        native_matrix[13]+=other.native_matrix[13];
        native_matrix[14]+=other.native_matrix[14];
        native_matrix[15]+=other.native_matrix[15];

        return *this;
    }

    inline matrix<T> operator- (const matrix<T>& other) const
    {
        matrix<T> temp;
        temp.native_matrix[0]  = native_matrix[0]-other.native_matrix[0];
        temp.native_matrix[1]  = native_matrix[1]-other.native_matrix[1];
        temp.native_matrix[2]  = native_matrix[2]-other.native_matrix[2];
        temp.native_matrix[3]  = native_matrix[3]-other.native_matrix[3];
        temp.native_matrix[4]  = native_matrix[4]-other.native_matrix[4];
        temp.native_matrix[5]  = native_matrix[5]-other.native_matrix[5];
        temp.native_matrix[6]  = native_matrix[6]-other.native_matrix[6];
        temp.native_matrix[7]  = native_matrix[7]-other.native_matrix[7];
        temp.native_matrix[8]  = native_matrix[8]-other.native_matrix[8];
        temp.native_matrix[9]  = native_matrix[9]-other.native_matrix[9];
        temp.native_matrix[10] = native_matrix[10]-other.native_matrix[10];
        temp.native_matrix[11] = native_matrix[11]-other.native_matrix[11];
        temp.native_matrix[12] = native_matrix[12]-other.native_matrix[12];
        temp.native_matrix[13] = native_matrix[13]-other.native_matrix[13];
        temp.native_matrix[14] = native_matrix[14]-other.native_matrix[14];
        temp.native_matrix[15] = native_matrix[15]-other.native_matrix[15];
        return temp;
    }

    inline matrix<T>& operator-= (const matrix<T>& other)
    {
        native_matrix[0]-=other.native_matrix[0];
        native_matrix[1]-=other.native_matrix[1];
        native_matrix[2]-=other.native_matrix[2];
        native_matrix[3]-=other.native_matrix[3];
        native_matrix[4]-=other.native_matrix[4];
        native_matrix[5]-=other.native_matrix[5];
        native_matrix[6]-=other.native_matrix[6];
        native_matrix[7]-=other.native_matrix[7];
        native_matrix[8]-=other.native_matrix[8];
        native_matrix[9]-=other.native_matrix[9];
        native_matrix[10]-=other.native_matrix[10];
        native_matrix[11]-=other.native_matrix[11];
        native_matrix[12]-=other.native_matrix[12];
        native_matrix[13]-=other.native_matrix[13];
        native_matrix[14]-=other.native_matrix[14];
        native_matrix[15]-=other.native_matrix[15];

        return *this;
    }

    inline matrix<T> operator* (T val) const
    {
        matrix<T> temp;
        temp.native_matrix[0]=native_matrix[0]*val;
        temp.native_matrix[1]=native_matrix[1]*val;
        temp.native_matrix[2]=native_matrix[2]*val;
        temp.native_matrix[3]=native_matrix[3]*val;
        temp.native_matrix[4]=native_matrix[4]*val;
        temp.native_matrix[5]=native_matrix[5]*val;
        temp.native_matrix[6]=native_matrix[6]*val;
        temp.native_matrix[7]=native_matrix[7]*val;
        temp.native_matrix[8]=native_matrix[8]*val;
        temp.native_matrix[9]=native_matrix[9]*val;
        temp.native_matrix[10]=native_matrix[10]*val;
        temp.native_matrix[11]=native_matrix[11]*val;
        temp.native_matrix[12]=native_matrix[12]*val;
        temp.native_matrix[13]=native_matrix[13]*val;
        temp.native_matrix[14]=native_matrix[14]*val;
        temp.native_matrix[15]=native_matrix[15]*val;

        return *this;
    }

    inline matrix<T>& operator *= (T val)
    {

        native_matrix[0]*=val;
        native_matrix[1]*=val;
        native_matrix[2]*=val;
        native_matrix[3]*=val;
        native_matrix[4]*=val;
        native_matrix[5]*=val;
        native_matrix[6]*=val;
        native_matrix[7]*=val;
        native_matrix[8]*=val;
        native_matrix[9]*=val;
        native_matrix[10]*=val;
        native_matrix[11]*=val;
        native_matrix[12]*=val;
        native_matrix[13]*=val;
        native_matrix[14]*=val;
        native_matrix[15]*=val;

        return *this;
    }

    inline matrix<T> operator*(const matrix<T> &other)
    {
        matrix result;
        result = multiply(*this,other);
        return result;
    }

    inline matrix<T>& operator*= (const matrix<T> &other)
    {
        *this = multiply(*this,other);
        return *this;
    }

    inline matrix<T>& buildOrthographicProjectionMatrix(f32 left,f32 right,f32 top,f32 bottom,f32 znear,f32 zfar)
    {
        makeIndetify();
        native_matrix[0]  = (T)  2 / (right - left);
        native_matrix[5]  = (T)  2 / (top - bottom);
        native_matrix[10] = (T) -2 / (zfar - znear);
        native_matrix[12] = (T) -((right + left) / (right - left));
        native_matrix[13] = (T) -((top + bottom) / (top - bottom));
        native_matrix[14] = (T) -((zfar + znear) / (zfar - znear));
        native_matrix[15] = 1;
        return *this;
    }

    inline matrix<T>& buildPerspectiveProjectionMatrix(f32 fov,f32 aspect,f32 znear,f32 zfar)
    {
        makeIndetify();
        double right,left,top,bottom;
        double temp, temp2, temp3, temp4;

        top = znear * tanf(fov * math::PI / 360.0);
        bottom = -top;
        right = top * aspect;
        left = -right;

        temp =  2.0 * znear;
        temp2 = right - left;

        temp3 = -(top - bottom);
        temp4 = zfar - znear;



        native_matrix[0]  = (T)(temp / temp2);
        native_matrix[5]  = (T)(temp / temp3);
        native_matrix[8]  = (T)((right + left) / temp2);
        native_matrix[9]  = (T)((top + bottom) / temp3);
        native_matrix[10] = (T)((-zfar - znear) / temp4);
        native_matrix[11] = (T)(-1.0);
        native_matrix[14] = (T)((-temp * zfar) / temp4);
        native_matrix[15] = (T)(0.0);

        return *this;
    }

    inline matrix<T>& buildLookAtMatrix(vector3<T> Position,vector3<T> Target,vector3<T> UpVector)
    {
        /*
         zaxis = normal(At - Eye)
         xaxis = normal(cross(Up, zaxis))
         yaxis = cross(zaxis, xaxis)

         xaxis.x           yaxis.x           zaxis.x          0
         xaxis.y           yaxis.y           zaxis.y          0
         xaxis.z           yaxis.z           zaxis.z          0
        -dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  l
        */

        //! Temp fix
        if(Position == Target)
        {
            Target.z--;
        }

        //! --------
        makeIndetify();

        vector3<T> XAxis,YAxis,ZAxis;

        ZAxis = Position-Target;
        ZAxis.normalize();

        XAxis = UpVector.cross_product(ZAxis);
        XAxis.normalize();

        YAxis = XAxis.cross_product(ZAxis);

        native_matrix[0]  = (T)XAxis.x;
        native_matrix[1]  = (T)YAxis.x;
        native_matrix[2]  = (T)ZAxis.x;
        native_matrix[3]  = 0;

        native_matrix[4]  = (T)XAxis.y;
        native_matrix[5]  = (T)YAxis.y;
        native_matrix[6]  = (T)ZAxis.y;
        native_matrix[7]  = 0;

        native_matrix[8]  = (T)XAxis.z;
        native_matrix[9]  = (T)YAxis.z;
        native_matrix[10] = (T)ZAxis.z;
        native_matrix[11] = 0;

        native_matrix[12] = (T)-XAxis.dot_product(Position);
        native_matrix[13] = (T)-YAxis.dot_product(Position);
        native_matrix[14] = (T)-ZAxis.dot_product(Position);
        native_matrix[15] = 1;

        return *this;
    }


    inline matrix<T>& transpose()
    {
        matrix result;

        result.native_matrix[1]  = native_matrix[4];
        result.native_matrix[2]  = native_matrix[8];
        result.native_matrix[3]  = native_matrix[12];
        result.native_matrix[7]  = native_matrix[13];
        result.native_matrix[11] = native_matrix[14];
        result.native_matrix[9]  = native_matrix[6];

        result.native_matrix[4]  = native_matrix[1];
        result.native_matrix[8]  = native_matrix[2];
        result.native_matrix[12] = native_matrix[3];
        result.native_matrix[13] = native_matrix[7];
        result.native_matrix[14] = native_matrix[11];
        result.native_matrix[6]  = native_matrix[9];

        memcpy(native_matrix, result.native_matrix, 16*sizeof(T));
        return *this;
    }

    inline matrix<T>& inverse()
    {

        double inv[16], det;
        int i;

        inv[0] = native_matrix[5]  * native_matrix[10] * native_matrix[15] - native_matrix[5]  * native_matrix[11] * native_matrix[14] - native_matrix[9]  * native_matrix[6]  * native_matrix[15] + native_matrix[9]  * native_matrix[7]  * native_matrix[14] + native_matrix[13] * native_matrix[6]  * native_matrix[11] - native_matrix[13] * native_matrix[7]  * native_matrix[10];
        inv[4] = -native_matrix[4]  * native_matrix[10] * native_matrix[15] + native_matrix[4]  * native_matrix[11] * native_matrix[14] + native_matrix[8]  * native_matrix[6]  * native_matrix[15] - native_matrix[8]  * native_matrix[7]  * native_matrix[14] - native_matrix[12] * native_matrix[6]  * native_matrix[11] + native_matrix[12] * native_matrix[7]  * native_matrix[10];
        inv[8] = native_matrix[4]  * native_matrix[9] * native_matrix[15] -native_matrix[4]  * native_matrix[11] * native_matrix[13] -native_matrix[8]  * native_matrix[5] * native_matrix[15] +native_matrix[8]  * native_matrix[7] * native_matrix[13] +native_matrix[12] * native_matrix[5] * native_matrix[11] -native_matrix[12] * native_matrix[7] * native_matrix[9];
        inv[12] = -native_matrix[4]  * native_matrix[9] * native_matrix[14] +native_matrix[4]  * native_matrix[10] * native_matrix[13] +native_matrix[8]  * native_matrix[5] * native_matrix[14] -native_matrix[8]  * native_matrix[6] * native_matrix[13] -native_matrix[12] * native_matrix[5] * native_matrix[10] +native_matrix[12] * native_matrix[6] * native_matrix[9];

        inv[1] = -native_matrix[1]  * native_matrix[10] * native_matrix[15] +native_matrix[1]  * native_matrix[11] * native_matrix[14] +native_matrix[9]  * native_matrix[2] * native_matrix[15] -native_matrix[9]  * native_matrix[3] * native_matrix[14] -native_matrix[13] * native_matrix[2] * native_matrix[11] +native_matrix[13] * native_matrix[3] * native_matrix[10];
        inv[5] = native_matrix[0]  * native_matrix[10] * native_matrix[15] -native_matrix[0]  * native_matrix[11] * native_matrix[14] -native_matrix[8]  * native_matrix[2] * native_matrix[15] +native_matrix[8]  * native_matrix[3] * native_matrix[14] +native_matrix[12] * native_matrix[2] * native_matrix[11] -native_matrix[12] * native_matrix[3] * native_matrix[10];
        inv[9] = -native_matrix[0]  * native_matrix[9] * native_matrix[15] +native_matrix[0]  * native_matrix[11] * native_matrix[13] +native_matrix[8]  * native_matrix[1] * native_matrix[15] -native_matrix[8]  * native_matrix[3] * native_matrix[13] -native_matrix[12] * native_matrix[1] * native_matrix[11] +native_matrix[12] * native_matrix[3] * native_matrix[9];
        inv[13] = native_matrix[0]  * native_matrix[9] * native_matrix[14] -native_matrix[0]  * native_matrix[10] * native_matrix[13] -native_matrix[8]  * native_matrix[1] * native_matrix[14] +native_matrix[8]  * native_matrix[2] * native_matrix[13] +native_matrix[12] * native_matrix[1] * native_matrix[10] -native_matrix[12] * native_matrix[2] * native_matrix[9];

        inv[2] = native_matrix[1]  * native_matrix[6] * native_matrix[15] -native_matrix[1]  * native_matrix[7] * native_matrix[14] -native_matrix[5]  * native_matrix[2] * native_matrix[15] +native_matrix[5]  * native_matrix[3] * native_matrix[14] +native_matrix[13] * native_matrix[2] * native_matrix[7] -native_matrix[13] * native_matrix[3] * native_matrix[6];
        inv[6] = -native_matrix[0]  * native_matrix[6] * native_matrix[15] +native_matrix[0]  * native_matrix[7] * native_matrix[14] +native_matrix[4]  * native_matrix[2] * native_matrix[15] -native_matrix[4]  * native_matrix[3] * native_matrix[14] -native_matrix[12] * native_matrix[2] * native_matrix[7] +native_matrix[12] * native_matrix[3] * native_matrix[6];
        inv[10] = native_matrix[0]  * native_matrix[5] * native_matrix[15] -native_matrix[0]  * native_matrix[7] * native_matrix[13] -native_matrix[4]  * native_matrix[1] * native_matrix[15] +native_matrix[4]  * native_matrix[3] * native_matrix[13] +native_matrix[12] * native_matrix[1] * native_matrix[7] -native_matrix[12] * native_matrix[3] * native_matrix[5];
        inv[14] = -native_matrix[0]  * native_matrix[5] * native_matrix[14] +native_matrix[0]  * native_matrix[6] * native_matrix[13] +native_matrix[4]  * native_matrix[1] * native_matrix[14] -native_matrix[4]  * native_matrix[2] * native_matrix[13] -native_matrix[12] * native_matrix[1] * native_matrix[6] +native_matrix[12] * native_matrix[2] * native_matrix[5];

        inv[3] = -native_matrix[1] * native_matrix[6] * native_matrix[11] +native_matrix[1] * native_matrix[7] * native_matrix[10] +native_matrix[5] * native_matrix[2] * native_matrix[11] -native_matrix[5] * native_matrix[3] * native_matrix[10] -native_matrix[9] * native_matrix[2] * native_matrix[7] +native_matrix[9] * native_matrix[3] * native_matrix[6];
        inv[7] = native_matrix[0] * native_matrix[6] * native_matrix[11] -native_matrix[0] * native_matrix[7] * native_matrix[10] -native_matrix[4] * native_matrix[2] * native_matrix[11] +native_matrix[4] * native_matrix[3] * native_matrix[10] +native_matrix[8] * native_matrix[2] * native_matrix[7] -native_matrix[8] * native_matrix[3] * native_matrix[6];
        inv[11] = -native_matrix[0] * native_matrix[5] * native_matrix[11] +native_matrix[0] * native_matrix[7] * native_matrix[9] +native_matrix[4] * native_matrix[1] * native_matrix[11] -native_matrix[4] * native_matrix[3] * native_matrix[9] -native_matrix[8] * native_matrix[1] * native_matrix[7] +native_matrix[8] * native_matrix[3] * native_matrix[5];
        inv[15] = native_matrix[0] * native_matrix[5] * native_matrix[10] - native_matrix[0] * native_matrix[6] * native_matrix[9] - native_matrix[4] * native_matrix[1] * native_matrix[10] + native_matrix[4] * native_matrix[2] * native_matrix[9] + native_matrix[8] * native_matrix[1] * native_matrix[6] - native_matrix[8] * native_matrix[2] * native_matrix[5];

        det = native_matrix[0] * inv[0] + native_matrix[1] * inv[4] + native_matrix[2] * inv[8] + native_matrix[3] * inv[12];

        if (det == 0)
            return *this;

        det = 1.0 / det;

        for (i = 0; i < 16; i++)
            native_matrix[i] = inv[i] * det;

        return *this;
    }

    inline matrix<T>& setTranslate(vector2<T> vec)
    {
        native_matrix[12] = vec.x;
        native_matrix[13] = vec.y;
        return *this;
    }


    inline matrix<T>& setRotationDegrees(vector2<T> rotation)
    {
        rotation *= math::DEGTORAD64;
        vector3<T> vec3d = vector3f(rotation.x,rotation.y,0);
        setRotationRadians(vec3d);
    }

    inline void setScale(vector2<T> scale)
    {
        if(scale == core::vector2<T>(0,0))
            return;

        matrix<T> sm;
        sm.native_matrix[0]  = (T)scale.x;
        sm.native_matrix[5]  = (T)scale.y;
        *this = multiply(*this,sm);
    }

    inline void rotate_vector(vector2<T>& vect)
    {
        vector2<T> tmp = vect;
        vect.x =   tmp.x*native_matrix[0] + tmp.y*native_matrix[4];
        vect.y =   tmp.x*native_matrix[1] + tmp.y*native_matrix[5];
    }

    inline void translate_vector(vector2<T>& vect)
    {
        vect.x += native_matrix[3];
        vect.y += native_matrix[7];
    }

    inline void transform_vector(vector2<T>& vect)
    {
        T vector[2];

        vector[0] = vect.x*native_matrix[0] + vect.y*native_matrix[4] + native_matrix[12];
        vector[1] = vect.x*native_matrix[1] + vect.y*native_matrix[5] + native_matrix[13];

        vect.x = vector[0];
        vect.y = vector[1];
    }

    inline void setTranslate(vector3<T> vec)
    {
        native_matrix[12] = vec.x;
        native_matrix[13] = vec.y;
        native_matrix[14] = vec.z;
    }

    inline void applyRotationDegrees(vector3<T> rotation)
    {
        applyRotationRadians(rotation * math::DEGTORAD);
    }

    inline void applyRotationRadians(vector3<T> rotation)
    {
        matrix<T> RotationMatrix;
        RotationMatrix.setRotationRadians(rotation);
        *this = multiply(*this,RotationMatrix);
    }

    inline void setRotationDegrees(vector3<T> rotation)
    {
        setRotationRadians(rotation * math::DEGTORAD64);
    }

    inline void setRotationRadians(vector3<T> rotation)
    {
        const f64 cr = cos(rotation.x);
        const f64 sr = sin(rotation.x);
        const f64 cp = cos(rotation.y);
        const f64 sp = sin(rotation.y);
        const f64 cy = cos(rotation.z);
        const f64 sy = sin(rotation.z);

        const f64 srsp = sr*sp;
        const f64 crsp = cr*sp;


        native_matrix[0] = (T)(cp*cy);
        native_matrix[4] = (T)(cp*sy);
        native_matrix[8] = (T)(-sp);

        native_matrix[1] = (T)(srsp*cy-cr*sy);
        native_matrix[5] = (T)(srsp*sy+cr*cy);
        native_matrix[9] = (T)(sr*cp);

        native_matrix[2] = (T)(crsp*cy+sr*sy);
        native_matrix[6] = (T)(crsp*sy-sr*cy);
        native_matrix[10]= (T)(cr*cp);
    }

    inline void setScale(vector3<T> scale)
    {
        if(scale == core::vector3<T>(0,0,0))
            return;

        matrix<T> sm;
        sm.native_matrix[0]  = (T)scale.x;
        sm.native_matrix[5]  = (T)scale.y;
        sm.native_matrix[10] = (T)scale.z;
        *this = multiply(*this,sm);
    }

    inline void rotate_vector(vector3<T>& vect)
    {
        vector3<T> tmp = vect;
        vect.x =   (tmp.x*native_matrix[0] + tmp.y*native_matrix[4] + tmp.z*native_matrix[8]);
        vect.y =   (tmp.x*native_matrix[1] + tmp.y*native_matrix[5] + tmp.z*native_matrix[9]);
        vect.z =   (tmp.x*native_matrix[2] + tmp.y*native_matrix[6] + tmp.z*native_matrix[10]);
    }

    inline void translate_vector(vector3<T>& vect)
    {
        vect.x += native_matrix[3];
        vect.y += native_matrix[7];
        vect.z += native_matrix[11];
    }

    inline void transform_vector(vector3<T>& vect)
    {
        T tmp_vector[3];

        tmp_vector[0] = vect.x*native_matrix[0] + vect.y*native_matrix[4] + vect.z*native_matrix[8] + native_matrix[12];
        tmp_vector[1] = vect.x*native_matrix[1] + vect.y*native_matrix[5] + vect.z*native_matrix[9] + native_matrix[13];
        tmp_vector[2] = vect.x*native_matrix[2] + vect.y*native_matrix[6] + vect.z*native_matrix[10] + native_matrix[14];

        vect.x = tmp_vector[0];
        vect.y = tmp_vector[1];
        vect.z = tmp_vector[2];
    }

    inline vector3<T> getTranslate()
    {
        return vector3<T>(native_matrix[12],native_matrix[13],native_matrix[14]);
    }
    inline vector3<T> getRotationDegrees()
    {
        return getRotationRadians() * math::RADTODEG;
    }
    inline vector3<T> getRotationRadians()
    {
        vector3<T> Rotation;
        Rotation.x = atan2f(native_matrix[9],native_matrix[10]);
        Rotation.y = atan2f(-native_matrix[8],sqrtf(native_matrix[9] * native_matrix[9] + native_matrix[10] * native_matrix[10]));
        Rotation.z = atan2f(native_matrix[4],native_matrix[0]);
        return Rotation;
    }

    inline vector3<T> getScale()
    {
        return vector3<T>(
                   sqrtf( (native_matrix[0] * native_matrix[0]) + (native_matrix[4] * native_matrix[4]) + (native_matrix[ 8] * native_matrix[ 8]) ),
                   sqrtf( (native_matrix[1] * native_matrix[1]) + (native_matrix[5] * native_matrix[5]) + (native_matrix[ 9] * native_matrix[ 9]) ),
                   sqrtf( (native_matrix[2] * native_matrix[2]) + (native_matrix[6] * native_matrix[6]) + (native_matrix[10] * native_matrix[10]) )
               );
    }

    T* getPointer()
    {
        return native_matrix;
    }

    inline matrix<T> multiply(const matrix<T>& mtx1,const matrix<T>& mtx2)
    {

        matrix<T> mtx3(true);

        T *m1 = (T*)    &mtx1.native_matrix;
        T *m2 = (T*)    &mtx2.native_matrix;
        T *m3 = (T*)    &mtx3.native_matrix;

        m3[0]  = m1[0]*m2[0]  + m1[4]*m2[1]  + m1[8]*m2[2]  + m1[12]*m2[3];
        m3[4]  = m1[0]*m2[4]  + m1[4]*m2[5]  + m1[8]*m2[6]  + m1[12]*m2[7];
        m3[8]  = m1[0]*m2[8]  + m1[4]*m2[9]  + m1[8]*m2[10] + m1[12]*m2[11];
        m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];

        m3[1]  = m1[1]*m2[0]  + m1[5]*m2[1]  + m1[9]*m2[2]   + m1[13]*m2[3];
        m3[5]  = m1[1]*m2[4]  + m1[5]*m2[5]  + m1[9]*m2[6]   + m1[13]*m2[7];
        m3[9]  = m1[1]*m2[8]  + m1[5]*m2[9]  + m1[9]*m2[10]  + m1[13]*m2[11];
        m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14]  + m1[13]*m2[15];

        m3[2]  = m1[2]*m2[0]  + m1[6]*m2[1]  + m1[10]*m2[2]  + m1[14]*m2[3];
        m3[6]  = m1[2]*m2[4]  + m1[6]*m2[5]  + m1[10]*m2[6]  + m1[14]*m2[7];
        m3[10] = m1[2]*m2[8]  + m1[6]*m2[9]  + m1[10]*m2[10] + m1[14]*m2[11];
        m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];

        m3[3]  = m1[3]*m2[0]  + m1[7]*m2[1]  + m1[11]*m2[2]  + m1[15]*m2[3];
        m3[7]  = m1[3]*m2[4]  + m1[7]*m2[5]  + m1[11]*m2[6]  + m1[15]*m2[7];
        m3[11] = m1[3]*m2[8]  + m1[7]*m2[9]  + m1[11]*m2[10] + m1[15]*m2[11];
        m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

        return mtx3;
    }

    T native_matrix[16];
protected:
private:

};
}
}
#endif // matrix_H
