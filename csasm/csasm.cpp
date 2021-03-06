#include "asf.h"

#include "platform.h"

#include <angelscript.h>

// for bytecode translation features
#include <../source/as_scriptengine.h>

#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>
#include <scriptany/scriptany.h>
#include <weakref/weakref.h>

#include "json.hpp"

#include <fstream>
#include <iostream>
#include <string>

template<typename... A>
static std::string fmtString(const std::string &format, A... args)
{
	// Thread safe
	static thread_local char sFormatBuf[2048];
	snprintf(sFormatBuf, sizeof(sFormatBuf), format.c_str(), args...);
	return std::string(sFormatBuf);
}

void AngelScriptMessageCallback(const asSMessageInfo *msg, void *param)
{
	std::string type;
	if (msg->type == asMSGTYPE_INFORMATION)
	{
		type = "INFO";
	}
	else if (msg->type == asMSGTYPE_WARNING)
	{
		type = "WARN";
	}
	else
	{
		type = "ERR ";
	}
	std::cout << msg->section
	          << " (" << msg->row << ", " << msg->col << ") : "
	          << type << " : " << msg->message << std::endl;
}

void RegisterScriptCafeTypedefs(asIScriptEngine *engine)
{
	engine->RegisterTypedef("f64",      "double");
	engine->RegisterTypedef("f32",      "float");
	engine->RegisterTypedef("s64",      "int64");
	engine->RegisterTypedef("u64",      "uint64");
	engine->RegisterTypedef("s32",      "int");
	engine->RegisterTypedef("u32",      "uint");
	engine->RegisterTypedef("s16",      "int16");
	engine->RegisterTypedef("u16",      "uint16");
	engine->RegisterTypedef("s8",       "int8");
	engine->RegisterTypedef("u8",       "uint8");
	engine->RegisterTypedef("AsHandle", "uint64");
	engine->RegisterTypedef("OSTime",   "int64");
}

void RegisterScriptMathTypeDeclarations(asIScriptEngine *engine)
{
	engine->SetDefaultNamespace("nw::math");
	RegisterScriptCafeTypedefs(engine);
	engine->RegisterObjectType("VEC2",   8, asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
	engine->RegisterObjectType("VEC3",  12, asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
	engine->RegisterObjectType("VEC4",  16, asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
	engine->RegisterObjectType("MTX22", 16, asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
	engine->RegisterObjectType("MTX23", 24, asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
	engine->RegisterObjectType("MTX33", 36, asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
	engine->RegisterObjectType("MTX34", 48, asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
	engine->RegisterObjectType("MTX43", 48, asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
	engine->RegisterObjectType("MTX44", 64, asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
	engine->RegisterObjectType("QUAT",  16, asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
	engine->SetDefaultNamespace("");
}

void RegisterMathVEC2(asIScriptEngine *engine)
{
	engine->RegisterObjectProperty("VEC2", "f32 x", 0);
	engine->RegisterObjectProperty("VEC2", "f32 y", 4);

	engine->RegisterObjectBehaviour("VEC2", asBEHAVE_CONSTRUCT, "void f()",               asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("VEC2", asBEHAVE_CONSTRUCT, "void f(const VEC2 &in)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("VEC2", asBEHAVE_CONSTRUCT, "void f(f32, f32)",       asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("VEC2", asBEHAVE_DESTRUCT,  "void f()",               asFUNCTION(0), asCALL_CDECL_OBJLAST);

	engine->RegisterObjectMethod("VEC2", "const VEC2 &Zero()",                                     asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("VEC2", "VEC2 &opAssign(const VEC2 &in)",                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 &opAddAssign(const VEC2 &in)",                      asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 &opSubAssign(const VEC2 &in)",                      asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 &opMulAssign(f32)",                                 asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 &opDivAssign(f32)",                                 asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 &opNeg() const",                                    asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 opAdd(const VEC2 &in) const",                       asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 opSub(const VEC2 &in) const",                       asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 opMul(f32) const",                                  asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 opDiv(f32) const",                                  asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 &Lerp(const VEC2 &in, const VEC2 &in, f32) const",  asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "f32 Dot(const VEC2 &in) const",                          asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "f32 LengthSquare() const",                               asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "f32 Length() const",                                     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 &Normalize()",                                      asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 &SetNormalize(const VEC2 &in)",                     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 &SafeNormalize(const VEC2 &in)",                    asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 &SetSafeNormalize(const VEC2 &in,const VEC2 &in)",  asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "f32 DistanceSquare(const VEC2 &in)",                     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 &SetMaximize(const VEC2 &in, const VEC2 &in)",      asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "VEC2 &SetMinimize(const VEC2 &in, const VEC2 &in)",      asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "void Set(f32, f32)",                                     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "bool opEquals(const VEC2 &in) const",                    asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC2", "bool IsZero() const",                                    asFUNCTION(0), asCALL_THISCALL);
}

void RegisterMathVEC3(asIScriptEngine *engine)
{
	engine->RegisterObjectProperty("VEC3", "f32 x", 0);
	engine->RegisterObjectProperty("VEC3", "f32 y", 4);
	engine->RegisterObjectProperty("VEC3", "f32 z", 8);

	engine->RegisterObjectBehaviour("VEC3", asBEHAVE_CONSTRUCT, "void f()",               asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("VEC3", asBEHAVE_CONSTRUCT, "void f(const VEC3 &in)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("VEC3", asBEHAVE_CONSTRUCT, "void f(f32, f32, f32)",  asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("VEC3", asBEHAVE_DESTRUCT,  "void f()",               asFUNCTION(0), asCALL_CDECL_OBJLAST);

	engine->RegisterObjectMethod("VEC3", "const VEC3 &Zero()",                                        asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("VEC3", "VEC3 &opAssign(const VEC3 &in)",                            asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &opAddAssign(const VEC3 &in)",                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &opSubAssign(const VEC3 &in)",                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &opMulAssign(const VEC3 &in)",                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &opMulAssign(f32)",                                    asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &opDivAssign(const VEC3 &in)",                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &opDivAssign(f32)",                                    asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &opNeg() const",                                       asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 opAdd(const VEC3 &in) const",                          asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 opSub(const VEC3 &in) const",                          asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 opMul(f32) const",                                     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 opDiv(f32) const",                                     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &Lerp(const VEC3 &in, const VEC3 &in, f32) const",     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "f32 Dot(const VEC3 &in) const",                             asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "f32 LengthSquare() const",                                  asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "f32 Length() const",                                        asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &Normalize()",                                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &SetNormalize(const VEC3 &in)",                        asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &SafeNormalize(const VEC3 &in)",                       asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &SetSafeNormalize(const VEC3 &in,const VEC3 &in)",     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "f32 DistanceSquare(const VEC3 &in)",                        asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "f32 Distance(const VEC3 &in)",                              asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &SetMaximize(const VEC3 &in, const VEC3 &in)",         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &SetMinimize(const VEC3 &in, const VEC3 &in)",         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &Cross(const VEC3 &in)",                               asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &SetCross(const VEC3 &in, const VEC3 &in)",            asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &SetTransform(const MTX33 &in, const VEC3 &in)",       asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("VEC3", "VEC3 &Transform(const MTX33 &in)",                          asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &SetTransform(const MTX34 &in, const VEC3 &in)",       asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("VEC3", "VEC3 &Transform(const MTX34 &in)",                          asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &SetTransform(const MTX44 &in, const VEC3 &in)",       asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("VEC3", "VEC3 &Transform(const MTX44 &in)",                          asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "VEC3 &SetTransformNormal(const MTX34 &in, const VEC3 &in)", asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("VEC3", "VEC3 &TransformNormal(const MTX34 &in)",                    asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "void Set(f32, f32, f32)",                                   asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "void Set(const VEC3 &in)",                                  asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "bool opEquals(const VEC3 &in) const",                       asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC3", "bool IsZero() const",                                       asFUNCTION(0), asCALL_THISCALL);
}

void RegisterMathVEC4(asIScriptEngine *engine)
{
	engine->RegisterObjectProperty("VEC4", "f32 x", 0);
	engine->RegisterObjectProperty("VEC4", "f32 y", 4);
	engine->RegisterObjectProperty("VEC4", "f32 z", 8);
	engine->RegisterObjectProperty("VEC4", "f32 w", 12);

	engine->RegisterObjectBehaviour("VEC4", asBEHAVE_CONSTRUCT, "void f()",                   asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("VEC4", asBEHAVE_CONSTRUCT, "void f(const VEC4 &in)",     asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("VEC4", asBEHAVE_CONSTRUCT, "void f(f32, f32, f32, f32)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("VEC4", asBEHAVE_DESTRUCT,  "void f()",                   asFUNCTION(0), asCALL_CDECL_OBJLAST);

	engine->RegisterObjectMethod("VEC4", "const VEC4 &Zero()",                                        asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("VEC4", "const VEC4 &ZeroWOne()",                                    asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("VEC4", "VEC4 &opAssign(const VEC4 &in)",                            asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &opAddAssign(const VEC4 &in)",                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &opSubAssign(const VEC4 &in)",                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &opMulAssign(f32)",                                    asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &opDivAssign(f32)",                                    asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &opNeg() const",                                       asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 opAdd(const VEC4 &in) const",                          asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 opSub(const VEC4 &in) const",                          asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 opMul(f32) const",                                     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 opDiv(f32) const",                                     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &Lerp(const VEC4 &in, const VEC4 &in, f32) const",     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "f32 Dot(const VEC4 &in) const",                             asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "f32 LengthSquare() const",                                  asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "f32 Length() const",                                        asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &Normalize()",                                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &SetNormalize(const VEC4 &in)",                        asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &SafeNormalize(const VEC4 &in)",                       asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &SetSafeNormalize(const VEC4 &in, const VEC4 &in)",    asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "f32 DistanceSquare(const VEC4 &in)",                        asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &SetMaximize(const VEC4 &in, const VEC4 &in)",         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &SetMinimize(const VEC4 &in, const VEC4 &in)",         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &SetTransform(const MTX34 &in, const VEC4 &in)",       asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("VEC4", "VEC4 &Transform(const MTX34 &in)",                          asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "VEC4 &SetTransform(const MTX44 &in, const VEC4 &in)",       asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("VEC4", "VEC4 &Transform(const MTX44 &in)",                          asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "void Set(f32, f32, f32, f32)",                              asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "bool opEquals(const VEC4 &in) const",                       asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "bool IsZero() const",                                       asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("VEC4", "bool IsZeroWOne() const",                                   asFUNCTION(0), asCALL_THISCALL);
}

void RegisterMathMTX22(asIScriptEngine *engine)
{
	engine->RegisterObjectProperty("MTX22", "f32 _00", 0);
	engine->RegisterObjectProperty("MTX22", "f32 _01", 4);
	engine->RegisterObjectProperty("MTX22", "f32 _10", 8);
	engine->RegisterObjectProperty("MTX22", "f32 _11", 12);

	engine->RegisterObjectBehaviour("MTX22", asBEHAVE_CONSTRUCT, "void f()",                   asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX22", asBEHAVE_CONSTRUCT, "void f(const MTX22 &in)",    asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX22", asBEHAVE_CONSTRUCT, "void f(f32, f32, f32, f32)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX22", asBEHAVE_DESTRUCT,  "void f()",                   asFUNCTION(0), asCALL_CDECL_OBJLAST);

	engine->RegisterObjectMethod("MTX22", "const MTX22& Identity()",              asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("MTX22", "MTX22 &opAssign(const MTX22 &in)",     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX22", "const VEC2 &GetRow(int) const",        asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX22", "VEC2 GetColumn(int) const",            asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX22", "MTX22 &SetIdentity()",                 asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX22", "bool opEquals(const MTX22 &in) const", asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX22", "bool IsIdentity() const",              asFUNCTION(0), asCALL_THISCALL);
}

void RegisterMathMTX23(asIScriptEngine *engine)
{
	engine->RegisterObjectProperty("MTX23", "f32 _00", 0);
	engine->RegisterObjectProperty("MTX23", "f32 _01", 4);
	engine->RegisterObjectProperty("MTX23", "f32 _02", 8);
	engine->RegisterObjectProperty("MTX23", "f32 _10", 12);
	engine->RegisterObjectProperty("MTX23", "f32 _11", 16);
	engine->RegisterObjectProperty("MTX23", "f32 _12", 20);

	engine->RegisterObjectBehaviour("MTX23", asBEHAVE_CONSTRUCT, "void f()",                             asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX23", asBEHAVE_CONSTRUCT, "void f(f32, f32, f32, f32, f32, f32)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX23", asBEHAVE_CONSTRUCT, "void f(const MTX23 &in)",              asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX23", asBEHAVE_DESTRUCT,  "void f()",                             asFUNCTION(0), asCALL_CDECL_OBJLAST);

	engine->RegisterObjectMethod("MTX23", "const MTX23& Identity()",                              asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("MTX23", "MTX23 &opAssign(const MTX23 &in)",                     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX23", "MTX23 &opAddAssign(const MTX23 &in)",                  asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX23", "MTX23 &opSubAssign(const MTX23 &in)",                  asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX23", "MTX23 &opMulAssign(f32)",                              asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX23", "MTX23 &opDivAssign(f32)",                              asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX23", "MTX23 &opNeg() const",                                 asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX23", "MTX23 opAdd(const MTX23 &in) const",                   asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX23", "MTX23 opSub(const MTX23 &in) const",                   asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX23", "MTX23 opMul(f32) const",                               asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX23", "MTX23 opDiv(f32) const",                               asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX23", "const VEC3 &GetRow(int) const",                        asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX23", "VEC2 GetColumn(int) const",                            asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX23", "MTX23 &SetIdentity()",                                 asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX23", "MTX23 &SetScale(const MTX23 &in, const VEC2 &in)",     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX23", "MTX23 &SetTranslate(const MTX23 &in, const VEC2 &in)", asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX23", "MTX23 &SetRotate(f32)",                                asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX23", "MTX23 &SetRotate(const VEC2 &in, f32)",                asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX23", "bool opEquals(const MTX23 &in) const",                 asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX23", "bool IsIdentity() const",                              asFUNCTION(0), asCALL_THISCALL);
}

void RegisterMathMTX33(asIScriptEngine *engine)
{
	engine->RegisterObjectProperty("MTX33", "f32 _00", 0);
	engine->RegisterObjectProperty("MTX33", "f32 _01", 4);
	engine->RegisterObjectProperty("MTX33", "f32 _02", 8);
	engine->RegisterObjectProperty("MTX33", "f32 _10", 12);
	engine->RegisterObjectProperty("MTX33", "f32 _11", 16);
	engine->RegisterObjectProperty("MTX33", "f32 _12", 20);
	engine->RegisterObjectProperty("MTX33", "f32 _20", 24);
	engine->RegisterObjectProperty("MTX33", "f32 _21", 28);
	engine->RegisterObjectProperty("MTX33", "f32 _22", 32);

	engine->RegisterObjectBehaviour("MTX33", asBEHAVE_CONSTRUCT, "void f()",                                            asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX33", asBEHAVE_CONSTRUCT, "void f(f32, f32, f32, f32, f32, f32, f32, f32, f32)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX33", asBEHAVE_CONSTRUCT, "void f(const MTX34 &in)",                             asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX33", asBEHAVE_CONSTRUCT, "void f(const MTX33 &in)",                             asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX33", asBEHAVE_DESTRUCT,  "void f()",                                            asFUNCTION(0), asCALL_CDECL_OBJLAST);

	engine->RegisterObjectMethod("MTX33", "const MTX33& Identity()",              asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("MTX33", "const VEC3 &GetRow(int) const",        asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX33", "VEC3 GetColumn(int) const",            asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX33", "MTX33 &SetIdentity()",                 asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX33", "bool opEquals(const MTX33 &in) const", asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX33", "bool IsIdentity() const",              asFUNCTION(0), asCALL_THISCALL);
}

void RegisterMathMTX34(asIScriptEngine *engine)
{
	engine->RegisterObjectProperty("MTX34", "f32 _00", 0);
	engine->RegisterObjectProperty("MTX34", "f32 _01", 4);
	engine->RegisterObjectProperty("MTX34", "f32 _02", 8);
	engine->RegisterObjectProperty("MTX34", "f32 _03", 12);
	engine->RegisterObjectProperty("MTX34", "f32 _10", 16);
	engine->RegisterObjectProperty("MTX34", "f32 _11", 20);
	engine->RegisterObjectProperty("MTX34", "f32 _12", 24);
	engine->RegisterObjectProperty("MTX34", "f32 _13", 28);
	engine->RegisterObjectProperty("MTX34", "f32 _20", 32);
	engine->RegisterObjectProperty("MTX34", "f32 _21", 36);
	engine->RegisterObjectProperty("MTX34", "f32 _22", 40);
	engine->RegisterObjectProperty("MTX34", "f32 _23", 44);

	engine->RegisterObjectBehaviour("MTX34", asBEHAVE_CONSTRUCT, "void f()",                                                           asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX34", asBEHAVE_CONSTRUCT, "void f(const MTX33 &in)",                                            asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX34", asBEHAVE_CONSTRUCT, "void f(f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX34", asBEHAVE_CONSTRUCT, "void f(const MTX34 &in)",                                            asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX34", asBEHAVE_DESTRUCT,  "void f()",                                                           asFUNCTION(0), asCALL_CDECL_OBJLAST);

	engine->RegisterObjectMethod("MTX34", "const MTX34& Identity()",                                                         asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("MTX34", "MTX34 &opAssign(const MTX34 &in)",                                                asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &opAddAssign(const MTX34 &in)",                                             asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &opSubAssign(const MTX34 &in)",                                             asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &opMulAssign(f32)",                                                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &opDivAssign(f32)",                                                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &opNeg() const",                                                            asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 opAdd(const MTX34 &in) const",                                              asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX34", "MTX34 opSub(const MTX34 &in) const",                                              asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX34", "MTX34 opMul(f32) const",                                                          asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX34", "MTX34 opMul_r(f32) const",                                                        asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX34", "MTX34 opDiv(f32) const",                                                          asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX34", "const VEC4 &GetRow(int) const",                                                   asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "VEC3 GetColumn(int) const",                                                       asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX34", "MTX34 &SetIdentity()",                                                            asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &SetScale(const VEC3 &in)",                                                 asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &SetTranslate(const VEC3 &in)",                                             asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &SetRotateXyz(const VEC3 &in)",                                             asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &SetRotate(const VEC3 &in, f32)",                                           asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &SetRotate(const QUAT &in)",                                                asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &SetLookAt(const VEC3 &in, const VEC3 &in, const VEC3 &in)",                asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &SetLookAt(const VEC3 &in, f32, const VEC3 &in)",                           asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &SetCameraRotate(const VEC3 &in, const VEC3 &in)",                          asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &SetTextureProjectionFrustum(f32, f32, f32, f32, f32, f32, f32, f32, f32)", asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &SetTextureProjectionPerspective(f32, f32, f32, f32, f32, f32)",            asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "MTX34 &SetTextureProjectionOrtho(f32, f32, f32, f32, f32, f32, f32, f32)",        asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "bool opEquals(const MTX34 &in) const",                                            asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX34", "bool IsIdentity() const",                                                         asFUNCTION(0), asCALL_THISCALL);
}

void RegisterMathMTX43(asIScriptEngine *engine)
{
	engine->RegisterObjectProperty("MTX43", "f32 _00", 0);
	engine->RegisterObjectProperty("MTX43", "f32 _01", 4);
	engine->RegisterObjectProperty("MTX43", "f32 _02", 8);
	engine->RegisterObjectProperty("MTX43", "f32 _10", 12);
	engine->RegisterObjectProperty("MTX43", "f32 _11", 16);
	engine->RegisterObjectProperty("MTX43", "f32 _12", 20);
	engine->RegisterObjectProperty("MTX43", "f32 _20", 24);
	engine->RegisterObjectProperty("MTX43", "f32 _21", 28);
	engine->RegisterObjectProperty("MTX43", "f32 _22", 32);
	engine->RegisterObjectProperty("MTX43", "f32 _30", 36);
	engine->RegisterObjectProperty("MTX43", "f32 _31", 40);
	engine->RegisterObjectProperty("MTX43", "f32 _32", 44);

	engine->RegisterObjectBehaviour("MTX43", asBEHAVE_CONSTRUCT, "void f()",                                                           asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX43", asBEHAVE_CONSTRUCT, "void f(f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX43", asBEHAVE_CONSTRUCT, "void f(const MTX43 &in)",                                            asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX43", asBEHAVE_DESTRUCT,  "void f()",                                                           asFUNCTION(0), asCALL_CDECL_OBJLAST);

	engine->RegisterObjectMethod("MTX43", "const MTX43& Identity()",             asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("MTX43", "MTX43 &opAssign(const MTX43 &in)",    asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX43", "MTX43 &opAddAssign(const MTX43 &in)", asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX43", "MTX43 &opSubAssign(const MTX43 &in)", asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX43", "MTX43 &opMulAssign(const MTX43 &in)", asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX43", "MTX43 &opMulAssign(f32)",             asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX43", "MTX43 &opDivAssign(f32)",             asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX43", "MTX43 &opNeg() const",                asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX43", "MTX43 opAdd(const MTX43 &in) const",  asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX43", "MTX43 opSub(const MTX43 &in) const",  asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX43", "MTX43 opMul(f32) const",              asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX43", "MTX43 opDiv(f32) const",              asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX43", "const VEC3 &GetRow(int) const",       asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX43", "VEC4 GetColumn(int) const",           asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX43", "MTX43 &SetIdentity()",                asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX43", "bool opEquals(const MTX43 &in) const",asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX43", "bool IsIdentity() const",             asFUNCTION(0), asCALL_THISCALL);
}

void RegisterMathMTX44(asIScriptEngine *engine)
{
	engine->RegisterEnum("PivotDirection");
	engine->RegisterEnumValue("PivotDirection", "PIVOT_NONE",             0);
	engine->RegisterEnumValue("PivotDirection", "PIVOT_UPSIDE_TO_TOP",    1);
	engine->RegisterEnumValue("PivotDirection", "PIVOT_UPSIDE_TO_RIGHT",  2);
	engine->RegisterEnumValue("PivotDirection", "PIVOT_UPSIDE_TO_BOTTOM", 3);
	engine->RegisterEnumValue("PivotDirection", "PIVOT_UPSIDE_TO_LEFT",   4);
	engine->RegisterEnumValue("PivotDirection", "PIVOT_NUM",              5);

	engine->RegisterObjectProperty("MTX44", "f32 _00", 0);
	engine->RegisterObjectProperty("MTX44", "f32 _01", 4);
	engine->RegisterObjectProperty("MTX44", "f32 _02", 8);
	engine->RegisterObjectProperty("MTX44", "f32 _03", 12);
	engine->RegisterObjectProperty("MTX44", "f32 _10", 16);
	engine->RegisterObjectProperty("MTX44", "f32 _11", 20);
	engine->RegisterObjectProperty("MTX44", "f32 _12", 24);
	engine->RegisterObjectProperty("MTX44", "f32 _13", 28);
	engine->RegisterObjectProperty("MTX44", "f32 _20", 32);
	engine->RegisterObjectProperty("MTX44", "f32 _21", 36);
	engine->RegisterObjectProperty("MTX44", "f32 _22", 40);
	engine->RegisterObjectProperty("MTX44", "f32 _23", 44);
	engine->RegisterObjectProperty("MTX44", "f32 _30", 48);
	engine->RegisterObjectProperty("MTX44", "f32 _31", 52);
	engine->RegisterObjectProperty("MTX44", "f32 _32", 56);
	engine->RegisterObjectProperty("MTX44", "f32 _33", 60);
	
	engine->RegisterObjectBehaviour("MTX44", asBEHAVE_CONSTRUCT, "void f()",                                                                               asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX44", asBEHAVE_CONSTRUCT, "void f(const MTX34 &in)",                                                                asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX44", asBEHAVE_CONSTRUCT, "void f(f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX44", asBEHAVE_CONSTRUCT, "void f(const MTX44 &in)",                                                                asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("MTX44", asBEHAVE_DESTRUCT,  "void f()",                                                                               asFUNCTION(0), asCALL_CDECL_OBJLAST);
	
	engine->RegisterObjectMethod("MTX44", "const MTX44& Identity()",                                         asFUNCTION(0), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("MTX44", "MTX44 &opAssign(const MTX44 &in)",                                asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &opAddAssign(const MTX44 &in)",                             asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &opSubAssign(const MTX44 &in)",                             asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &opMulAssign(const MTX44 &in)",                             asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &opMulAssign(f32)",                                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &opDivAssign(f32)",                                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &opNeg() const",                                            asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 opAdd(const MTX44 &in) const",                              asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX44", "MTX44 opSub(const MTX44 &in) const",                              asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX44", "MTX44 opMul(f32) const",                                          asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX44", "MTX44 opDiv(f32) const",                                          asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX44", "const VEC4 &GetRow(int) const",                                   asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "VEC4 GetColumn(int) const",                                       asFUNCTION(0), asCALL_GENERIC);
	engine->RegisterObjectMethod("MTX44", "MTX44 &Transpose()",                                              asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &SetIdentity()",                                            asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &SetScale(const VEC3 &in)",                                 asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &SetTranslate(const VEC3 &in)",                             asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &SetRotateXyz(const VEC3 &in)",                             asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &SetRotate(const VEC3 &in, f32)",                           asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &SetFrustum(f32, f32, f32, f32, f32, f32, PivotDirection)", asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &SetOrtho(f32, f32, f32, f32, f32, f32, PivotDirection)",   asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "MTX44 &SetPerspective(f32, f32, f32, f32, PivotDirection)",       asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "bool opEquals(const MTX44 &in) const",                            asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("MTX44", "bool IsIdentity() const",                                         asFUNCTION(0), asCALL_THISCALL);

}

void RegisterMathQUAT(asIScriptEngine *engine)
{
	engine->RegisterObjectProperty("QUAT", "f32 x", 0);
	engine->RegisterObjectProperty("QUAT", "f32 y", 4);
	engine->RegisterObjectProperty("QUAT", "f32 z", 8);
	engine->RegisterObjectProperty("QUAT", "f32 w", 12);

	engine->RegisterObjectBehaviour("QUAT", asBEHAVE_CONSTRUCT, "void f()",                   asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("QUAT", asBEHAVE_CONSTRUCT, "void f(const QUAT &in)",     asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("QUAT", asBEHAVE_CONSTRUCT, "void f(f32, f32, f32, f32)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("QUAT", asBEHAVE_DESTRUCT,  "void f()",                   asFUNCTION(0), asCALL_CDECL_OBJLAST);

	engine->RegisterObjectMethod("QUAT", "QUAT &opAssign(const QUAT &in)",                            asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("QUAT", "QUAT &opAddAssign(const QUAT &in)",                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("QUAT", "QUAT &opSubAssign(const QUAT &in)",                         asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("QUAT", "QUAT &opMulAssign(f32)",                                    asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("QUAT", "QUAT &opDivAssign(f32)",                                    asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("QUAT", "QUAT &opNeg() const",                                       asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("QUAT", "QUAT opAdd(const QUAT &in) const",                          asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("QUAT", "QUAT opSub(const QUAT &in) const",                          asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("QUAT", "QUAT opMul(f32) const",                                     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("QUAT", "QUAT opDiv(f32) const",                                     asFUNCTION(0), asCALL_THISCALL);
	engine->RegisterObjectMethod("QUAT", "bool opEquals(const QUAT &in) const",                       asFUNCTION(0), asCALL_THISCALL);

	engine->RegisterGlobalFunction("QUAT& QUATAdd(QUAT &out, const QUAT &in, const QUAT &in )",                                       asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATSub(QUAT &out, const QUAT &in, const QUAT &in )",                                       asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATDivide(QUAT &out, const QUAT &in, const QUAT &in )",                                    asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATMult(QUAT &out, const QUAT &in, const QUAT &in )",                                      asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("f32 QUATDot(const QUAT &in, const QUAT &in )",                                                    asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATInverse(QUAT &out, const QUAT &in )",                                                   asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATScale(QUAT&out, const QUAT&in , f32 )",                                                 asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATNormalize(QUAT&out, const QUAT&in )",                                                   asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATExp(QUAT&out, const QUAT&in )",                                                         asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATLogN(QUAT&out, const QUAT&in )",                                                        asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATLerp(QUAT&out, const QUAT&in , const QUAT&in , f32 )",                                  asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATSlerp(QUAT&out, const QUAT&in , const QUAT&in , f32 )",                                 asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& MTX34ToQUAT(QUAT&out, const MTX34&in )",                                                    asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATSquad(QUAT&out, const QUAT&in , const QUAT&in , const QUAT&in , const QUAT&in , f32 )", asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATMakeClosest(QUAT&out, const QUAT&in , const QUAT&in  )",                                asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATRotAxisRad(QUAT&out, const VEC3&in , f32  )",                                           asFUNCTION(0), asCALL_CDECL);
	engine->RegisterGlobalFunction("QUAT& QUATMakeVectorRotation(QUAT&out, const VEC3&in , const VEC3&in  )",                         asFUNCTION(0), asCALL_CDECL);
}

void RegisterScriptMathTypeDefinitions(asIScriptEngine *engine)
{
	engine->SetDefaultNamespace("nw::math");
	RegisterMathVEC2(engine);
	RegisterMathVEC3(engine);
	RegisterMathVEC4(engine);
	RegisterMathMTX22(engine);
	RegisterMathMTX23(engine);
	RegisterMathMTX33(engine);
	RegisterMathMTX34(engine);
	RegisterMathMTX43(engine);
	RegisterMathMTX44(engine);
	RegisterMathQUAT(engine);
	engine->SetDefaultNamespace("");
}

void RegisterScriptMathTypes(asIScriptEngine *engine)
{
	RegisterScriptMathTypeDeclarations(engine);
	RegisterScriptMathTypeDefinitions(engine);
}

void RegisterScriptPreregNamespaces(asIScriptEngine *engine, const nlohmann::json &config)
{
	for (const auto &registration : config["namespaces"])
	{
		engine->SetDefaultNamespace(registration["namespace"].get<std::string>().c_str());
		RegisterScriptCafeTypedefs(engine);
		engine->SetDefaultNamespace("");
	}
}

void RegisterScriptPreregEnumsAndValues(asIScriptEngine *engine, const nlohmann::json &config)
{
	// Unused in PMCS
}

void RegisterScriptPreregObjectTypes(asIScriptEngine *engine, const nlohmann::json &config)
{
	for (const auto &registration : config["object_types"])
	{
		engine->SetDefaultNamespace(registration["namespace"].get<std::string>().c_str());
		engine->RegisterObjectType(registration["object_name"].get<std::string>().c_str(),
								   registration["size"].get<int>(),
								   registration["flags"].get<asDWORD>());
		engine->SetDefaultNamespace("");
	}
}

void RegisterScriptPreregObjectProperties(asIScriptEngine *engine, const nlohmann::json &config)
{
	for (const auto &registration : config["object_properties"])
	{
		engine->SetDefaultNamespace(registration["namespace"].get<std::string>().c_str());
		engine->RegisterObjectProperty(registration["object_name"].get<std::string>().c_str(),
									   registration["declaration"].get<std::string>().c_str(),
									   registration["offset"].get<int>());
		engine->SetDefaultNamespace("");
	}
}

void RegisterScriptPreregObjectBehaviours(asIScriptEngine *engine, const nlohmann::json &config)
{
	for (const auto &registration : config["object_behaviours"])
	{
		engine->SetDefaultNamespace(registration["namespace"].get<std::string>().c_str());
		engine->RegisterObjectBehaviour(registration["object_name"].get<std::string>().c_str(),
										registration["behaviour"].get<asEBehaviours>(),
										registration["declaration"].get<std::string>().c_str(),
										asFUNCTION(0),
										registration["calling_convention"].get<asECallConvTypes>());
		engine->SetDefaultNamespace("");
	}
}

void RegisterScriptPreregObjectMethods(asIScriptEngine *engine, const nlohmann::json &config)
{
	for (const auto &registration : config["object_methods"])
	{
		engine->SetDefaultNamespace(registration["namespace"].get<std::string>().c_str());
		engine->RegisterObjectMethod(registration["object_name"].get<std::string>().c_str(),
									 registration["declaration"].get<std::string>().c_str(),
									 asFUNCTION(0),
									 registration["global"].get<int>() ? asCALL_THISCALL_ASGLOBAL : asCALL_THISCALL);
		engine->SetDefaultNamespace("");
	}
}

void RegisterScriptPreregGlobalFunctions(asIScriptEngine *engine, const nlohmann::json &config)
{
	for (const auto &registration : config["global_functions"])
	{
		engine->SetDefaultNamespace(registration["namespace"].get<std::string>().c_str());
		engine->RegisterGlobalFunction(registration["declaration"].get<std::string>().c_str(),
									   asFUNCTION(0),
									   asCALL_CDECL);
		engine->SetDefaultNamespace("");
	}
}

void RegisterScriptPreregGlobalProperties(asIScriptEngine *engine, const nlohmann::json &config)
{

	for (const auto &registration : config["global_properties"])
	{
		engine->SetDefaultNamespace(registration["namespace"].get<std::string>().c_str());
		engine->RegisterGlobalProperty(registration["declaration"].get<std::string>().c_str(),
									   reinterpret_cast<void *>(static_cast<size_t>(engine->GetGlobalPropertyCount() + 1)));
		engine->SetDefaultNamespace("");
	}
}

void ConfigureEngine(asIScriptEngine *engine, const nlohmann::json &config)
{
	// Replicate the PMCS scripting environment

	// Init PMCS global properties
	// PMCS has this at true, but we're building, so we have to diverge here
	engine->SetEngineProperty(asEP_INIT_GLOBAL_VARS_AFTER_BUILD, 0);
	engine->SetEngineProperty(asEP_OPTIMIZE_BYTECODE, 1);
	engine->SetEngineProperty(asEP_SCRIPT_SCANNER, 1);
	engine->SetEngineProperty(asEP_STRING_ENCODING, 0);
	engine->SetEngineProperty(asEP_ALLOW_UNSAFE_REFERENCES, 1);
	engine->SetEngineProperty(asEP_AUTO_GARBAGE_COLLECT, 1);

	// Register script extensions
	RegisterStdString(engine);
	engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT, "void f(const int)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT, "void f(const uint)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT, "void f(const bool)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT, "void f(const float)", asFUNCTION(0), asCALL_CDECL_OBJLAST);
	RegisterScriptArray(engine, false);
	RegisterStdStringUtils(engine);
	RegisterScriptAny(engine);
	RegisterScriptWeakRef(engine);

	// PMCS-specific extensions
	RegisterScriptCafeTypedefs(engine);
	RegisterScriptMathTypes(engine);

	// The rest of this comes from the data through a linked list built through
	// global constructors; we parse this as a JSON
	RegisterScriptPreregNamespaces(engine, config);
	RegisterScriptPreregEnumsAndValues(engine, config);
	RegisterScriptPreregObjectTypes(engine, config);
	RegisterScriptPreregObjectProperties(engine, config);
	RegisterScriptPreregObjectBehaviours(engine, config);
	RegisterScriptPreregObjectMethods(engine, config);
	RegisterScriptPreregGlobalFunctions(engine, config);
	RegisterScriptPreregGlobalProperties(engine, config);
}

std::string dumpBytecode(asIScriptFunction *func)
{
	std::string dump;
	asCScriptFunction *function = static_cast<asCScriptFunction *>(func);

	asCScriptEngine *engine = static_cast<asCScriptEngine *>(function->GetEngine());
	
	asUINT length;
	asDWORD *code = function->GetByteCode(&length);
	for (uint32_t position = 0; position < length; )
	{
		std::string disas = "UNK";

		// Read opcode
		asDWORD *inst = code + position;
		asEBCInstr op = static_cast<asEBCInstr>(*reinterpret_cast<asBYTE *>(inst));
		const char *mnem = asBCInfo[op].name;
		switch (asBCInfo[op].type)
		{
		case asBCTYPE_NO_ARG: // no args
			{
				disas = fmtString("%-8s", mnem);
			}
			break;
		case asBCTYPE_W_ARG: // word imm
			{
				asWORD arg0 = asBC_WORDARG0(inst);
				switch (op)
				{
				case asBC_STR:
					{
						const auto &str = engine->GetConstantString(arg0);
						disas = fmtString("%-8s %d (%d:\"%s\")", mnem, arg0, str.GetLength(), str.AddressOf());
					}
					break;
				default:
					disas = fmtString("%-8s %d", mnem, arg0);
					break;
				}
			}
			break;
		case asBCTYPE_wW_ARG: // word var dst
		case asBCTYPE_rW_ARG: // word var src
			{
				uint16_t arg0 = asBC_WORDARG0(inst);
				disas = fmtString("%-8s v%d", mnem,
								  reinterpret_cast<int16_t &>(arg0));
			}
			break;
		case asBCTYPE_wW_rW_ARG: // word dst, word src
		case asBCTYPE_rW_rW_ARG: // word src, word src
			{
				uint16_t arg0 = asBC_WORDARG0(inst);
				uint16_t arg1 = asBC_WORDARG1(inst);
				disas = fmtString("%-8s v%d, v%d", mnem,
								  reinterpret_cast<int16_t &>(arg0),
								  reinterpret_cast<int16_t &>(arg1));
			}
			break;
		case asBCTYPE_wW_W_ARG: // word dst, word imm
			{
				uint16_t arg0 = asBC_WORDARG0(inst);
				uint16_t arg1 = asBC_WORDARG1(inst);
				disas = fmtString("%-8s v%d, %d", mnem,
								  reinterpret_cast<int16_t &>(arg0),
								  reinterpret_cast<int16_t &>(arg1));
			}
			break;
		case asBCTYPE_wW_rW_DW_ARG: // word dst, word src, dword imm
		case asBCTYPE_rW_W_DW_ARG:  // word src, word imm, dword imm
			{
				uint16_t arg0 = asBC_WORDARG0(inst);
				uint16_t arg1 = asBC_WORDARG1(inst);
				uint32_t arg2 = asBC_DWORDARG(inst);
				switch (op)
				{
				case asBC_ADDIf:
				case asBC_SUBIf:
				case asBC_MULIf:
					disas = fmtString("%-8s v%d, v%d, %f", mnem,
									  reinterpret_cast<int16_t &>(arg0),
									  reinterpret_cast<int16_t &>(arg1),
									  reinterpret_cast<float &>(arg2));
					break;
				default:
					disas = fmtString("%-8s v%d, v%d, %d", mnem,
									  reinterpret_cast<int16_t &>(arg0),
									  reinterpret_cast<int16_t &>(arg1),
									  reinterpret_cast<int16_t &>(arg2));
					break;
				}
			}
			break;
		case asBCTYPE_DW_ARG: // dword imm
			{
				uint32_t arg0 = asBC_DWORDARG(inst);
				switch (op)
				{
				case asBC_OBJTYPE:
					{
						asIObjectType *type = reinterpret_cast<asIObjectType *>(arg0);
						disas = fmtString("%-8s 0x%x (type:%s)", mnem, arg0, type->GetName());
					}
					break;
				case asBC_FuncPtr:
					{
						asIScriptFunction *func = reinterpret_cast<asIScriptFunction *>(arg0);
						disas = fmtString("%-8s 0x%x (func:%s)", mnem, arg0, func->GetDeclaration());
					}
					break;
				case asBC_PshC4:
				case asBC_Cast:
					disas = fmtString("%-8s 0x%x (i:%d, f:%g)", mnem,
									  arg0, reinterpret_cast<int &>(arg0), reinterpret_cast<float &>(arg0));
					break;
				case asBC_TYPEID:
					disas = fmtString("%-8s 0x%x (decl:%s)", mnem,
									  arg0, engine->GetTypeDeclaration(reinterpret_cast<int &>(arg0)));
					break;
				case asBC_PGA: // global vars
				case asBC_PshGPtr:
				case asBC_LDG:
				case asBC_PshG4:
					{
						asCGlobalProperty *prop = function->GetPropertyByGlobalVarPtr(reinterpret_cast<void *>(arg0));
						disas = fmtString("%-8s 0x%x (%d:%s)", mnem,
										  arg0,
										  prop->id,
										  prop->name.AddressOf());
					}
					break;
				case asBC_CALL:
				case asBC_CALLSYS:
				case asBC_CALLBND:
				case asBC_CALLINTF:
				case asBC_Thiscall1:
					{
						asIScriptFunction *func;
						if (arg0 & 0x40000000)
						{
							func = engine->importedFunctions[reinterpret_cast<int &>(arg0) & ~0x40000000]->importedFunctionSignature;
						}
						else
						{
							func = engine->GetFunctionById(reinterpret_cast<int &>(arg0));
						}

						disas = fmtString("%-8s %d (%s)", mnem,
										  arg0,
										  func->GetDeclaration(true, true, true));
					}
					break;
				case asBC_REFCPY:
					disas = fmtString("%-8s 0x%x", mnem, arg0);
					break;
				case asBC_JMP:
				case asBC_JZ:
				case asBC_JLowZ:
				case asBC_JS:
				case asBC_JP:
				case asBC_JNZ:
				case asBC_JLowNZ:
				case asBC_JNS:
				case asBC_JNP:
					disas = fmtString("%-8s %+d (d:%x)", mnem,
									  reinterpret_cast<int &>(arg0),
									  position + reinterpret_cast<int &>(arg0));
					break;
				default:
					disas = fmtString("%-8s %d", mnem, reinterpret_cast<int &>(arg0));
					break;
				}
			}
			break;
		case asBCTYPE_QW_ARG: // qword imm
			{
				uint64_t arg0 = asBC_QWORDARG(inst);
				// #todo-csasm: Potentially add 64-bit pointer opcode handling
				disas = fmtString("%-8s 0x%llx (i:%lld, f:%g)", mnem,
								  arg0, reinterpret_cast<int64_t &>(arg0), reinterpret_cast<double &>(arg0));
			}
			break;
		case asBCTYPE_wW_QW_ARG: // word dst, qword imm
		case asBCTYPE_rW_QW_ARG: // word src, qword imm
			{
				uint16_t arg0 = asBC_WORDARG0(inst);
				uint64_t arg1 = asBC_QWORDARG(inst);
				switch (op)
				{
				case asBC_RefCpyV:
				case asBC_FREE:
					{
						asIObjectType *type = reinterpret_cast<asIObjectType *>(arg1);
						disas = fmtString("%-8s v%d, 0x%x (type:%s)", mnem,
										  reinterpret_cast<int16_t &>(arg0),
										  static_cast<uint32_t>(arg1),
										  type->GetName());
					}
					break;
				default:
					disas = fmtString("%-8s v%d, 0x%llx (i:%lld, f:%g)", mnem,
									  reinterpret_cast<int16_t &>(arg0),
									  arg1,
									  reinterpret_cast<int64_t &>(arg1),
									  reinterpret_cast<double &>(arg1));
					break;
				}
			}
			break;
		case asBCTYPE_DW_DW_ARG: // dword imm, dword imm
			{
				// #todo-csasm: Investigate DW_DW layout further
				#define asBC_DWORDARG1(x)  (*(((asDWORD*)x)+2))
				uint32_t arg0 = asBC_DWORDARG(inst);
				uint32_t arg1 = asBC_DWORDARG1(inst);
				switch (op)
				{
				case asBC_ALLOC:
					{
						asIObjectType *type = reinterpret_cast<asIObjectType *>(arg0);
						asIScriptFunction *func = engine->GetFunctionById(asBC_WORDARG0(inst));
						disas = fmtString("%-8s 0x%x, %d (type:%s, %s)", mnem,
										  arg0,
										  reinterpret_cast<int &>(arg1),
										  type->GetName(),
										  func ? func->GetDeclaration() : "{no func}");
					}
					break;
				case asBC_SetG4:
					{
						asCGlobalProperty *prop = function->GetPropertyByGlobalVarPtr(reinterpret_cast<void *>(arg0));
						disas = fmtString("%-8s 0x%x, %d (%d:%s)", mnem,
										  arg0,
										  reinterpret_cast<int &>(arg1),
										  prop->id,
										  prop->name.AddressOf());
					}
					break;
				default:
					disas = fmtString("%-8s %u, %d", mnem,
									  arg0,
									  reinterpret_cast<int &>(arg1));
					break;
				}
			}
			break;
		case asBCTYPE_rW_DW_DW_ARG: // word src, dword imm, dword imm
			{
				uint16_t arg0 = asBC_WORDARG0(inst);
				uint32_t arg1 = asBC_DWORDARG(inst);
				uint32_t arg2 = asBC_DWORDARG1(inst);

				disas = fmtString("%-8s v%d, %u, %u", mnem,
								  reinterpret_cast<int16_t &>(arg0), arg1, arg2);
			}
			break;
		case asBCTYPE_QW_DW_ARG: // qword imm, dword imm
			{
				// #todo-csasm: Consider adding support for 64-bit asBC_ALLOC
				uint64_t arg0 = asBC_QWORDARG(inst);
				uint32_t arg1 = asBC_DWORDARG(inst);

				disas = fmtString("%-8s %llu, %d", mnem,
								  reinterpret_cast<int64_t &>(arg0), reinterpret_cast<int &>(arg1));
			}
			break;
		case asBCTYPE_INFO: // word imm
			{
				uint16_t arg0 = asBC_WORDARG0(inst);
				switch (op)
				{
				case asBC_LABEL:
					disas = fmtString("%d:", arg0);
					break;
				case asBC_LINE:
					disas = fmtString("%-8s", mnem);
					break;
				case asBC_Block:
					// #todo-csasm: Add block indenting
					disas = fmtString("%c", arg0 ? '{' : '}');
					break;
				}
			}
			break;
		case asBCTYPE_rW_DW_ARG: // word src, dword imm
		case asBCTYPE_wW_DW_ARG: // word dst, dword imm
		case asBCTYPE_W_DW_ARG: // word imm, dword imm
			{
				uint16_t arg0 = asBC_WORDARG0(inst);
				uint32_t arg1 = asBC_DWORDARG(inst);
				switch (op)
				{
				case asBC_SetV1:
					disas = fmtString("%-8s v%d, 0x%x", mnem,
									  reinterpret_cast<int16_t &>(arg0),
									  static_cast<uint8_t>(arg1));
					break;
				case asBC_SetV2:
					disas = fmtString("%-8s v%d, 0x%x", mnem,
									  reinterpret_cast<int16_t &>(arg0),
									  static_cast<uint16_t>(arg1));
					break;
				case asBC_SetV4:
					disas = fmtString("%-8s v%d, 0x%x (i:%d, f:%g)", mnem,
									  reinterpret_cast<int16_t &>(arg0),
									  arg1,
									  reinterpret_cast<int &>(arg1),
									  reinterpret_cast<float &>(arg1));
					break;
				case asBC_CMPIf:
					disas = fmtString("%-8s v%d, %f", mnem,
									  reinterpret_cast<int16_t &>(arg0),
									  reinterpret_cast<float &>(arg1));
					break;
				case asBC_LdGRdR4: // global vars
				case asBC_CpyGtoV4:
				case asBC_CpyVtoV4:
					{
						asCGlobalProperty *prop = function->GetPropertyByGlobalVarPtr(reinterpret_cast<void *>(arg1));
						disas = fmtString("%-8s v%d, 0x%x (%d:%s)", mnem,
										  reinterpret_cast<int16_t &>(arg0),
										  reinterpret_cast<int &>(arg1),
										  prop->id,
										  prop->name.AddressOf());
					}
					break;
				default:
					disas = fmtString("%-8s v%d, %d", mnem,
									  reinterpret_cast<int16_t &>(arg0),
									  reinterpret_cast<int &>(arg1));
					break;
				}
			}
			break;
		case asBCTYPE_wW_rW_rW_ARG: // dst word, src word, src word
			{
				// #todo-csasm: Investigate wW_rW_rW layout further
#define asBC_WORDARG2(x)  (*(((asWORD*)x)+3))
				uint16_t arg0 = asBC_WORDARG0(inst);
				uint16_t arg1 = asBC_WORDARG1(inst);
				uint16_t arg2 = asBC_WORDARG2(inst);
				disas = fmtString("%-8s v%d, v%d, v%d", mnem,
								  reinterpret_cast<int16_t &>(arg0),
								  reinterpret_cast<int16_t &>(arg1),
								  reinterpret_cast<int16_t &>(arg2));
			}
			break;
		default:
			__debugbreak();
			break;
		}
		dump.append(fmtString("\t\t%04x: %s\n", position, disas.c_str()));

		position += asBCTypeSize[asBCInfo[op].type];
	}
	return dump;
}

std::string DumpModule(asIScriptModule *module)
{
	// Dump all information in the module
	std::string dump = "";

	// Enums
	dump.append(fmtString("enums: %u\n", module->GetEnumCount()));
	for (unsigned int i = 0; i < module->GetEnumCount(); ++i)
	{
		int typeId;
		const char *nameSpace;
		const char *enumName = module->GetEnumByIndex(i, &typeId, &nameSpace);

		std::string prefix = "";
		if (std::string(nameSpace) != "")
		{
			prefix = nameSpace;
			prefix += "::";
		}
		dump.append(fmtString("\t%s%s",
							  prefix.c_str(),
							  enumName));

		asIObjectType *type = module->GetEngine()->GetObjectTypeById(typeId);
		if (type)
		{
			dump.append(fmtString(" : %s", type->GetName()));
		}
		dump.append("\n");

		// Values
		for (int j = 0; j < module->GetEnumValueCount(typeId); ++j)
		{
			int value;
			const char *valueName = module->GetEnumValueByIndex(typeId, j, &value);
			dump.append(fmtString("\t\t%s = %d,\n", valueName, value));
		}
	}
	
	// Typedefs
	dump.append(fmtString("typedefs: %u\n", module->GetTypedefCount()));
	for (unsigned int i = 0; i < module->GetTypedefCount(); ++i)
	{
		int typeId;
		const char *typedefName = module->GetTypedefByIndex(i, &typeId);
		dump.append(fmtString("\t%s %d\n", typedefName, typeId));
	}

	// Object types
	dump.append(fmtString("object_types: %u\n", module->GetObjectTypeCount()));
	for (unsigned int i = 0; i < module->GetObjectTypeCount(); ++i)
	{
		asIObjectType *type = module->GetObjectTypeByIndex(i);
		dump.append(fmtString("\t%s %d %08x\n", type->GetName(), type->GetSize(), type->GetFlags()));
		// #todo-csasm: Advanced object type dumping
		dump.append(fmtString("\t\tproperties: %u\n", type->GetPropertyCount()));
		for (unsigned int j = 0; j < type->GetPropertyCount(); ++j)
		{
			dump.append(fmtString("\t\t\t%s\n", type->GetPropertyDeclaration(j)));
		}
	}

	// Global variables
	dump.append(fmtString("global_variables: %u\n", module->GetGlobalVarCount()));
	for (unsigned int i = 0; i < module->GetGlobalVarCount(); ++i)
	{
		// #todo-csasm: Dump global variables
		dump.append(fmtString("\t%s\n", module->GetGlobalVarDeclaration(i, true)));
	}

	// Imported functions
	dump.append(fmtString("imported_functions: %u\n", module->GetImportedFunctionCount()));
	for (unsigned int i = 0; i < module->GetImportedFunctionCount(); ++i)
	{
		// #todo-csasm: Dump imported functions
		dump.append(fmtString("\t%s %s\n",
							  module->GetImportedFunctionDeclaration(i),
							  module->GetImportedFunctionSourceModule(i)));
	}

	// Functions
	dump.append(fmtString("functions: %u\n", module->GetFunctionCount()));
	for (unsigned int i = 0; i < module->GetFunctionCount(); ++i)
	{
		asIScriptFunction *func = module->GetFunctionByIndex(i);
		// #todo-csasm: Dump functions
		dump.append(fmtString("\t%s\n",
							  func->GetDeclaration(true, true, true)));
		dump.append(dumpBytecode(func));
	}

	return dump;
}

int main(int argc, char **argv)
{
	// ConIO for UTF8 characters
	setupConsoleCodePage();

	std::cout << fmtString("csasm by PistonMiner, built on %s\n\n", __TIMESTAMP__);

	// Create engine
	asIScriptEngine *engine = asCreateScriptEngine();
	if (!engine)
	{
		return -1;
	}

	engine->SetMessageCallback(asFUNCTION(AngelScriptMessageCallback), 0, asCALL_CDECL);

	// We must replicate the scripting environment that PMCS registers in order to parse its scripts
	std::ifstream configStream(argv[2]);
	nlohmann::json config = nlohmann::json::parse(configStream);
	ConfigureEngine(engine, config);

	AsfModuleTracker tracker(engine, argv[1]);
	AsfModule *mainModule = tracker.getModule(argv[3]);
	std::cout << DumpModule(mainModule->getScriptModule());

	resetConsoleCodePage();
}