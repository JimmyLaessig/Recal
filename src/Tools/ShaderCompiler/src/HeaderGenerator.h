#ifndef SHADERCOMPILER_HEADERGENERATOR_H_
#define SHADERCOMPILER_HEADERGENERATOR_H_

#include "GLSLShaderCompiler.h"

#include <optional>
#include <string>


//template<typename FLOAT>
//struct Vec2
//{
//	FLOAT value[2];
//};
//
//
//template<typename FLOAT>
//struct Vec3
//{
//	FLOAT value[3];
//	FLOAT padding;
//};
//
//
//template<typename FLOAT>
//struct Vec4
//{
//	FLOAT value[4];
//};
//
//
//template<typename FLOAT>
//struct Mat4
//{
//	FLOAT values[4 * 4];
//};
//
//
//template<typename FLOAT>
//struct Mat3
//{
//	Vec3<FLOAT> values[3];
//};
//
//
//template<typename T>
//struct PaddedStruct
//{
//	T value;
//	sizeof(T) % 4 
//};
//
//struct X
//{
//	float x;
//	float y;
//	float z;
//};
//
//
//sizeof(X) = 3
//
//-> padding = 1;
//
//
//4 - sizeof(T) % 4 

class HeaderGenerator
{
public:

	static std::string buildShaderHeader(const std::string& className, 
										 const std::optional<std::string>& classNamespace, 
										 const ShaderProgram& shaderProgram);
};

#endif // SHADERCOMPILER_HEADERGENERATOR_H_