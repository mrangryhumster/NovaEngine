#ifndef QUATERNION_H
#define QUATERNION_H

#include "NTypes.h"
#include "matrix.h"
//#include "log/NLogger.h"

namespace novaengine
{
namespace core
{

template <class T>
class quaternion
{
public:
    quaternion<T>()
    {}
    quaternion<T>(const T x_,const T y_,const T z_,const T w_):
        x(x_),y(y_),z(z_),w(w_)
    {}

    quaternion<T>(const vector3<T>& v) :
        x(v.x),y(v.y),z(v.z),w(0)
    {}

    inline quaternion<T>& set(const T x_,const T y_,const T z_,const T w_)
    {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
        return *this;
    }

    inline quaternion<T>& set(const quaternion<T>& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }

    inline quaternion<T> operator* (const quaternion<T>& q) const
    {
        return quaternion<T>
               (
                   (f32)(w*q.x + x*q.w + y*q.z - z*q.y),
                   (f32)(w*q.y + y*q.w + z*q.x - x*q.z),
                   (f32)(w*q.z + z*q.w + x*q.y - y*q.x),
                   (f32)(w*q.w - x*q.x - y*q.y - z*q.z)
               );
    }

    inline quaternion<T> & operator *= (const quaternion<T>& q)
    {
        quaternion<T> tmp
        (
            y * q.z - z * q.y + w * q.x + x * q.w,
            z * q.x - x * q.z + w * q.y + y * q.w,
            x * q.y - y * q.x + w * q.z + z * q.w,
            w * q.w - x * q.x - y * q.y - z * q.z
        );

        x = tmp.x;
        y = tmp.y;
        z = tmp.z;
        w = tmp.w;

        return tmp;
    }


    inline	quaternion<T>& setRotationDegrees(core::vector3<T> Rotation)
    {
        return setRotationRadians(Rotation * math::DEGTORAD);
    }

    inline	quaternion<T>& setRotationRadians(core::vector3<T> Rotation)
    {
        T angle;

        angle = Rotation.x * 0.5;
        const T sr = sin(angle);
        const T cr = cos(angle);

        angle = -Rotation.y * 0.5;
        const T sp = sin(angle);
        const T cp = cos(angle);

        angle = Rotation.z * 0.5;
        const T sy = sin(angle);
        const T cy = cos(angle);

        const T cpcy = cp * cy;
        const T spcy = sp * cy;
        const T cpsy = cp * sy;
        const T spsy = sp * sy;

        x = (T)(sr * cpcy - cr * spsy);
        y = (T)(cr * spcy + sr * cpsy);
        z = (T)(cr * cpsy - sr * spcy);
        w = (T)(cr * cpcy + sr * spsy);

        return *this;
    }

    inline vector3<T> getRotationDegrees()
    {
        return getRotationRadians() * math::RADTODEG;
    }

    inline vector3<T> getRotationRadians() const
    {
        vector3<T> euler;
        const f64 sqw = w*w;
        const f64 sqx = x*x;
        const f64 sqy = y*y;
        const f64 sqz = z*z;
        const f64 test = 2.0 * (y*w - x*z);

        if (math::is_equal(test, 1.0, 0.000001))
        {
            euler.z = (f32) (-2.0*atan2(x, w));
            euler.x = 0;
            euler.y = (f32) (math::PI64/2.0);
        }
        else if (math::is_equal(test, -1.0, 0.000001))
        {
            euler.z = (f32) (2.0*atan2(x, w));
            euler.x = 0;
            euler.y = (f32) (math::PI64/-2.0);
        }
        else
        {
            euler.z = (f32) atan2(2.0 * (x*y +z*w),(sqx - sqy - sqz + sqw));
            euler.x = (f32) atan2(2.0 * (y*z +x*w),(-sqx - sqy + sqz + sqw));
            euler.y = (f32) asin((test < 1.0 ? 1.0 : test) < 1.0 ? (test < 1.0 ? 1.0 : test) : 1.0);
        }
        return euler;
    }

    inline	core::matrix<T> get_rotation_matrix(core::vector3<T> RotationCenter = core::vector3<T>(0,0,0))
    {
        core::matrix<T> matrix;

        matrix[0]  = 1.0f - 2.0f * (y*y + z*z);
        matrix[4]  = 2.0 * (x*y - w*z);
        matrix[8]  = 2.0 * (x*z + w*y);
        matrix[12] = 0;

        matrix[1]  = 2.0 * (x*y + w*z);
        matrix[5]  = 1.0f - 2.0f * (x*x + z*z);
        matrix[9]  = 2.0 * (y*z - w*x);
        matrix[13] = 0;

        matrix[2]  = 2.0 * (x*z - w*y);
        matrix[6]  = 2.0 * (y*z + w*x);
        matrix[10] = 1.0f - 2.0f * (x*x + y*y);
        matrix[14] = 0;

        matrix[3]  = RotationCenter.x;
        matrix[7]  = RotationCenter.y;
        matrix[11] = RotationCenter.z;
        matrix[15] = 1;

        return matrix;
    }

    inline vector3<T> rotate_vector(vector3<T> vector)
    {
              quaternion<T> p(vector);
        const quaternion<T> q_conj(-x,-y,-z,w);

        p = *this * p * q_conj;
        return vector3<T>(p.x,p.y,p.z);
    }

    inline quaternion<T>& conj ()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    inline	quaternion<T>& normalize()
    {
        const float l = 1.0f/sqrt(x*x + y*y + z*z + w*w);
        x *= l;
        y *= l;
        z *= l;
        w *= l;
        return *this;
    }

    inline quaternion<T> slerp(const quaternion<T>& q1,const quaternion<T>& q2,float t)
    {
        float	dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;


        if ( 1 + dot > math::FLT_EPSILON)
        {
            if ( 1 - dot > math::FLT_EPSILON )
            {
                float	omega   = (float) acos ( dot );
                float	invSine = 1.0f / (float) sin ( omega );
                float	scale1  = invSine * (float) sin ( ( 1.0f - t ) * omega );
                float	scale2  = invSine * (float) sin ( t * omega );

                return quaternion<T>  ( scale1 * q1.x + scale2 * q2.x,
                                        scale1 * q1.y + scale2 * q2.y,
                                        scale1 * q1.z + scale2 * q2.z,
                                        scale1 * q1.w + scale2 * q2.w );
            }
            else
            {
                float	scale1 = 1.0f - t;
                float	scale2 = t;

                return quaternion<T>  ( scale1 * q1.x + scale2 * q2.x,
                                        scale1 * q1.y + scale2 * q2.y,
                                        scale1 * q1.z + scale2 * q2.z,
                                        scale1 * q1.w + scale2 * q2.w );
            }
        }


        // quaternions are nearly opposite, create a perpendicual quaternion and slerp it
        float	scale1 = (float) sin ( ( 1.0f - t ) * math::PI * 0.5f );
        float	scale2 = (float) sin ( t * math::PI * 0.5f );

        return quaternion<T>  ( scale1 * q1.x + scale2 * ( -q2.y ),
                                scale1 * q1.y + scale2 * q2.x,
                                scale1 * q1.z + scale2 * ( -q2.w ),
                                scale1 * q1.w + scale2 * q2.w );
    }

    inline void slerp_this(const quaternion<T>& q1,const quaternion<T>& q2,float t)
    {
        float      dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
        float	scale1;
        float	scale2;
        if ( 1 + dot > math::FLT_EPSILON)
        {

            if ( 1 - dot > math::FLT_EPSILON )
            {
                float	omega   = (float) acos ( dot );
                float	invSine = 1.0f / (float) sin ( omega );
                scale1 = invSine * (float) sin ( ( 1.0f - t ) * omega );
                scale2 = invSine * (float) sin ( t * omega );
            }
            else
            {
                scale1 = 1.0f - t;
                scale2 = t;
            }
            x = scale1 * q1.x + scale2 * q2.x;
            y = scale1 * q1.y + scale2 * q2.y;
            z = scale1 * q1.z + scale2 * q2.z;
            w = scale1 * q1.w + scale2 * q2.w;
            return;
        }
        else
        {
            // quaternions are nearly opposite, create a perpendicual quaternion and slerp it
            scale1 = (float) sin ( ( 1.0f - t ) * math::PI * 0.5f );
            scale2 = (float) sin ( t * math::PI * 0.5f );

            x = scale1 * q1.x + scale2 * ( -q2.y );
            y = scale1 * q1.y + scale2 * q2.x;
            z = scale1 * q1.z + scale2 * ( -q2.w );
            w = scale1 * q1.w + scale2 * q2.w ;
            return;
        }
    }


    T x;
    T y;
    T z;
    T w;

protected:
private:

};

}
}
#endif // QUATERNION_H
