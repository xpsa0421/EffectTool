#pragma once
#include "Std.h"

class JsonHelper
{
public:
	static bool LoadJSON(const W_STR& fileName, rapidjson::Document& outDoc);

	static bool GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
	static bool GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
	static bool GetWString(const rapidjson::Value& inObject, const char* inProperty, W_STR& outStr);
	static bool GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
	static bool GetFloat2(const rapidjson::Value& inObject, const char* inProperty, XMFLOAT2& outFloat2);
	static bool GetFloat3(const rapidjson::Value& inObject, const char* inProperty, XMFLOAT3& outFloat3);
	static bool GetFloat4(const rapidjson::Value& inObject, const char* inProperty, XMFLOAT4& outFloat4);

	// Setter functions
	static void AddInt(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, int value);
	static void AddFloat(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, float value);
	static void AddWString(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, const W_STR& value);
	static void AddBool(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, bool value);
	static void AddFloat2(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, const XMFLOAT2& value);
	static void AddFloat3(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, const XMFLOAT3& value);
	static void AddFloat4(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, const XMFLOAT4& value);
};

