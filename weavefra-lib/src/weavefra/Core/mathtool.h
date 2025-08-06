#pragma once

#include "WFpch.h"

#include "CoreDefined.h"

namespace weavefra
{
    #ifndef WF_MathTool_Create
        #define WF_MathTool_Create

        template<typename T>
        struct _Enable_if_floating_helper;
        template<>
        struct _Enable_if_floating_helper<float> { using type = float; };
        template<>
        struct _Enable_if_floating_helper<double> { using type = double; };
        template<>
        struct _Enable_if_floating_helper<long double> { using type = long double; };

        template<typename T>
        using _Enable_if_floating = typename _Enable_if_floating_helper<T>::type;

        template<typename _Tp>
        inline constexpr _Tp pi_v = _Enable_if_floating<_Tp>(3.141592653589793238462643383279502884L);

        #ifndef M_PI
            #define M_PI pi_v<double>
        #endif

        struct WF_API vector2D
        {
            float x, y;

            bool operator == (const vector2D& other) const
            {
                return x == other.x && y == other.y;
            }
            void operator = (const vector2D& other)
            {
                x = other.x;
                y = other.y;
            }
            vector2D operator - (const vector2D& other) const
            {
                float x = this->x;
                float y = this->y;
                x -= other.x;
                y -= other.y;
                return { x, y };
            }
            vector2D operator + (const vector2D& other) const
            {
                return { x + other.x, y + other.y };
            }
            vector2D operator * (float scalar) const
            {
                return { x * scalar, y * scalar };
            }
            vector2D operator / (float scalar) const
            {
                return { x / scalar, y / scalar };
            }
            float length() const
            {
                return std::sqrt( x * x + y * y );
            }
            vector2D normalized() const
            {
                return { this->x / this->length(), this->y / this->length() };
            }
            float dot(const vector2D& other) const
            {
                return {x * other.x + y * other.y };
            }
            vector2D rotation(float degrees, vector2D origin) const
            {
                float radian = degrees * (M_PI / 180.0f);
                float cosA = std::cos(radian);
                float sinA = std::sin(radian);
                float dx = x - origin.x;
                float dy = y - origin.y;
                return {
                    (cosA * dx - sinA * dy) + origin.x,
                    (sinA * dx + cosA * dy) + origin.y
                };
            }
            float cross(const vector2D& other) const
            {
                return x * other.y - y * other.x;
            }
            vector2D() = default;
            vector2D(float x, float y) : x(x), y(y) {}
        };
        struct WF_API vector3D
        {
            int x, y, z;
            bool operator == (const vector3D& other) const
            {
                return x == other.x && y == other.y && z == other.z;
            }
            void operator = (const vector3D& other)
            {
                x = other.x;
                y = other.y;
                z = other.z;
            }
            void operator - (const vector3D& other)
            {
                x -= other.x;
                y -= other.y;
                z -= other.z;
            }
            void operator + (const vector3D& other)
            {
                x += other.x;
                y += other.y;
                z += other.z;
            }
            vector3D operator*(float scalar) const
            {

            }
            vector3D operator/(float scalar) const
            {

            }
            float length() const
            {

            }
            vector3D normalized() const
            {

            }
            float dot(const vector3D& other) const
            {

            }
            vector3D perpendicular() const
            {

            }

        };

        //std::array<float, 2> WF_API ProjectConvexPolygon(const vector2D& axis, const std::vector<vector2D>& vertices)
        //{
        //    std::array<float, 2> project = {std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest()};
        //    for(auto it : vertices)
        //    {
        //        float dot = axis.dot(it);
        //        project[0] = (project[0] < dot) ? dot : project[0];
        //        project[1] = (project[1] > dot) ? dot : project[1];
        //    }
        //    return project;
        //}
    #endif
}