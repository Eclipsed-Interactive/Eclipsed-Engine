#pragma once 

#include <unordered_map>
#include <string>

#include "CoreEngine/Math/Vector/Vector2.h"
//#include "Utilities/Math/Vector/Vector3.h"
#include "CoreEngine/Math/Vector/Vector4.h"

namespace Eclipse
{
    enum class UniformType
    {
        Bool,

        Float,
        Vector2f,
        Vector3f,
        Vector4f,

        // Double,
        // Vector2d,
        // Vector3d,
        // Vector4d,

        Int,
        // Vector2i,
        // Vector3i,
        // Vector4i,

        // UnsignedInt,
        // Vector2ui,
        // Vector3ui,
        // Vector4ui,

        Matrix2x2f,
        Matrix3x3f,
        Matrix4x4f
    };

    class UniformVariableManager
    {
    private:

        friend class OpenGLGraphicsEngine;

        struct GlobalUniformValueInt
        {
            int value;
        };

        struct GlobalUniformValueFloat
        {
            float value;
        };

        struct GlobalUniformValueVector2Float
        {
            Math::Vector2f value;
        };

        struct GlobalUniformValueVector3Float
        {
            Math::Vector4f value;
        };

        struct GlobalUniformValueVector4Float
        {
            Math::Vector4f value;
        };

        struct GlobalUniformValueMatrixMatrix2x2Float
        {
            float value[4];
        };

        struct GlobalUniformValueMatrixMatrix3x3Float
        {
            float value[9];
        };

        struct GlobalUniformValueMatrixMatrix4x4Float
        {
            float value[16];
        };

        std::unordered_map<std::string, GlobalUniformValueInt> myGlobalUniformsInt;

        std::unordered_map<std::string, GlobalUniformValueFloat> myGlobalUniformsFloat;

        std::unordered_map<std::string, GlobalUniformValueVector2Float> myGlobalUniformsVector2Float;
        std::unordered_map<std::string, GlobalUniformValueVector3Float> myGlobalUniformsVector3Float;
        std::unordered_map<std::string, GlobalUniformValueVector4Float> myGlobalUniformsVector4Float;

        std::unordered_map<std::string, GlobalUniformValueMatrixMatrix2x2Float> myGlobalUniformsMatrix2x2Float;
        std::unordered_map<std::string, GlobalUniformValueMatrixMatrix3x3Float> myGlobalUniformsMatrix3x3Float;
        std::unordered_map<std::string, GlobalUniformValueMatrixMatrix4x4Float> myGlobalUniformsMatrix4x4Float;

        void SetGlobalUniforms(unsigned aShaderProgram);


        void AddInt(const char* aUniformName, int* aValue) { myGlobalUniformsInt.emplace(aUniformName, *aValue); }

        void AddFloat(const char* aUniformName, float* aValue) { myGlobalUniformsFloat.emplace(aUniformName, *aValue); }

        void AddVec2Float(const char* aUniformName, Math::Vector2f* aValue) { myGlobalUniformsVector2Float.emplace(aUniformName, *aValue); }
        void AddVec3Float(const char* aUniformName, Math::Vector4f* aValue) { myGlobalUniformsVector3Float.emplace(aUniformName, *aValue); }
        void AddVec4Float(const char* aUniformName, Math::Vector4f* aValue) { myGlobalUniformsVector4Float.emplace(aUniformName, *aValue); }

        void AddMat2x2(const char* aUniformName, float* aValue)
        {
            myGlobalUniformsMatrix2x2Float.emplace(aUniformName,
                GlobalUniformValueMatrixMatrix2x2Float{
                    aValue[0], aValue[1], aValue[2], aValue[3]
                }
            );
        }
        void AddMat3x3(const char* aUniformName, float* aValue)
        {
            myGlobalUniformsMatrix3x3Float.emplace(aUniformName,
                GlobalUniformValueMatrixMatrix3x3Float{
                    aValue[0], aValue[1], aValue[2], aValue[3], aValue[4], aValue[5], aValue[6], aValue[7], aValue[8]
                }
            );
        }
        void AddMat4x4(const char* aUniformName, float* aValue)
        {
            myGlobalUniformsMatrix4x4Float.emplace(aUniformName,
                GlobalUniformValueMatrixMatrix4x4Float{
                    aValue[0], aValue[1], aValue[2], aValue[3], aValue[4], aValue[5], aValue[6], aValue[7],
                    aValue[8], aValue[9], aValue[10], aValue[11], aValue[12], aValue[13], aValue[14], aValue[15]
                });
        }




        void UpdateInt(const char* aUniformName, int* aValue)
        {
            if (myGlobalUniformsInt.find(aUniformName) == myGlobalUniformsInt.end())
                AddInt(aUniformName, aValue);

            myGlobalUniformsInt.at(aUniformName).value = *aValue;
        }

        void UpdateFloat(const char* aUniformName, float* aValue)
        {
            if (myGlobalUniformsFloat.find(aUniformName) == myGlobalUniformsFloat.end())
                AddFloat(aUniformName, aValue);

            myGlobalUniformsFloat.at(aUniformName).value = *aValue;
        }

        void UpdateVec2Float(const char* aUniformName, Math::Vector2f* aValue)
        {
            if (myGlobalUniformsVector2Float.find(aUniformName) == myGlobalUniformsVector2Float.end())
                AddVec2Float(aUniformName, aValue);

            myGlobalUniformsVector2Float.at(aUniformName).value = *aValue;
        }
        void UpdateVec3Float(const char* aUniformName, Math::Vector4f* aValue)
        {
            if (myGlobalUniformsVector3Float.find(aUniformName) == myGlobalUniformsVector3Float.end())
                AddVec3Float(aUniformName, aValue);

            myGlobalUniformsVector3Float.at(aUniformName).value = *aValue;
        }
        void UpdateVec4Float(const char* aUniformName, Math::Vector4f* aValue)
        {
            if (myGlobalUniformsVector4Float.find(aUniformName) == myGlobalUniformsVector4Float.end())
                AddVec4Float(aUniformName, aValue);

            myGlobalUniformsVector4Float.at(aUniformName).value = *aValue;
        }

        void UpdateMat2x2(const char* aUniformName, float* aValue)
        {
            if (myGlobalUniformsMatrix2x2Float.find(aUniformName) == myGlobalUniformsMatrix2x2Float.end())
                AddMat2x2(aUniformName, aValue);

            std::memcpy(&myGlobalUniformsMatrix2x2Float.at(aUniformName).value, aValue, sizeof(GlobalUniformValueMatrixMatrix2x2Float));
        }
        void UpdateMat3x3(const char* aUniformName, float* aValue)
        {
            if (myGlobalUniformsMatrix3x3Float.find(aUniformName) == myGlobalUniformsMatrix3x3Float.end())
                AddMat3x3(aUniformName, aValue);

            std::memcpy(&myGlobalUniformsMatrix3x3Float.at(aUniformName).value, aValue, sizeof(GlobalUniformValueMatrixMatrix3x3Float));
        }
        void UpdateMat4x4(const char* aUniformName, float* aValue)
        {
            if (myGlobalUniformsMatrix4x4Float.find(aUniformName) == myGlobalUniformsMatrix4x4Float.end())
                AddMat4x4(aUniformName, aValue);

            std::memcpy(&myGlobalUniformsMatrix4x4Float.at(aUniformName).value, aValue, sizeof(GlobalUniformValueMatrixMatrix4x4Float));
        }




        void GetInt(const char* aUniformName, int* aValue)
        {
            if (myGlobalUniformsInt.find(aUniformName) == myGlobalUniformsInt.end())
                return;

            *aValue = myGlobalUniformsInt.at(aUniformName).value;
        }

        void GetFloat(const char* aUniformName, float* aValue)
        {
            if (myGlobalUniformsFloat.find(aUniformName) == myGlobalUniformsFloat.end())
                return;

            *aValue = myGlobalUniformsFloat.at(aUniformName).value;
        }

        void GetVec2Float(const char* aUniformName, Math::Vector2f* aValue)
        {
            if (myGlobalUniformsVector2Float.find(aUniformName) == myGlobalUniformsVector2Float.end())
                return;

            *aValue = myGlobalUniformsVector2Float.at(aUniformName).value;
        }
        void GetVec3Float(const char* aUniformName, Math::Vector4f* aValue)
        {
            if (myGlobalUniformsVector3Float.find(aUniformName) == myGlobalUniformsVector3Float.end())
                return;

            *aValue = myGlobalUniformsVector3Float.at(aUniformName).value;
        }
        void GetVec4Float(const char* aUniformName, Math::Vector4f* aValue)
        {
            if (myGlobalUniformsVector4Float.find(aUniformName) == myGlobalUniformsVector4Float.end())
                return;

            *aValue = myGlobalUniformsVector4Float.at(aUniformName).value;
        }

        void GetMat2x2(const char* aUniformName, float* aValue)
        {
            if (myGlobalUniformsMatrix2x2Float.find(aUniformName) == myGlobalUniformsMatrix2x2Float.end())
                return;

            std::memcpy(aValue, &myGlobalUniformsMatrix2x2Float.at(aUniformName).value, sizeof(GlobalUniformValueMatrixMatrix2x2Float));
        }
        void GetMat3x3(const char* aUniformName, float* aValue)
        {
            if (myGlobalUniformsMatrix3x3Float.find(aUniformName) == myGlobalUniformsMatrix3x3Float.end())
                return;

            std::memcpy(aValue, &myGlobalUniformsMatrix3x3Float.at(aUniformName).value, sizeof(GlobalUniformValueMatrixMatrix3x3Float));
        }
        void GetMat4x4(const char* aUniformName, float* aValue)
        {
            if (myGlobalUniformsMatrix4x4Float.find(aUniformName) == myGlobalUniformsMatrix4x4Float.end())
                return;

            std::memcpy(aValue, &myGlobalUniformsMatrix4x4Float.at(aUniformName).value, sizeof(GlobalUniformValueMatrixMatrix4x4Float));
        }





        void SetUniformInt(const char* aUniformName, unsigned aShaderProgram, const int* aValue) const;

        void SetUniformFloat(const char* aUniformName, unsigned aShaderProgram, const float* aValue) const;

        void SetUniformVec2Float(const char* aUniformName, unsigned aShaderProgram, const Math::Vector2f* aValue) const;
        void SetUniformVec3Float(const char* aUniformName, unsigned aShaderProgram, const Math::Vector4f* aValue) const;
        void SetUniformVec4Float(const char* aUniformName, unsigned aShaderProgram, const Math::Vector4f* aValue) const;

        void SetUniformMat2x2(const char* aUniformName, unsigned aShaderProgram, const float* aValue) const;
        void SetUniformMat3x3(const char* aUniformName, unsigned aShaderProgram, const float* aValue) const;
        void SetUniformMat4x4(const char* aUniformName, unsigned aShaderProgram, const float* aValue) const;
    };
}