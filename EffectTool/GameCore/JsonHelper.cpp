//#include "JsonHelper.h"
//
//bool JsonHelper::LoadJSON(const W_STR& fileName, rapidjson::Document& outDoc)
//{
//	std::ifstream ifs(wtm(fileName).c_str());
//	if (!ifs.is_open())
//	{
//		#ifdef _DEBUG
//		printf("[JsonHelper] Failed to find file %s\n", wtm(fileName).c_str());
//		#endif // _DEBUG
//		return false;
//	}
//	rapidjson::IStreamWrapper isw(ifs);
//
//	outDoc.ParseStream(isw);
//	if (!outDoc.IsObject()) 
//	{
//		#ifdef _DEBUG
//		printf("[JsonHelper] File %s is not a valid JSON\n", wtm(fileName).c_str());
//		#endif // _DEBUG	
//		return false;
//	}
//	
//	return true;
//}
//
//bool JsonHelper::GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt)
//{
//	// Check if this property exists
//	auto itr = inObject.FindMember(inProperty);
//	if (itr == inObject.MemberEnd())
//	{
//		return false;
//	}
//
//	// Get the value type, and check it's an integer
//	auto& property = itr->value;
//	if (!property.IsInt())
//	{
//		return false;
//	}
//
//	// We have the property
//	outInt = property.GetInt();
//	return true;
//}
//
//bool JsonHelper::GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat)
//{
//	auto itr = inObject.FindMember(inProperty);
//	if (itr == inObject.MemberEnd())
//	{
//		return false;
//	}
//
//	auto& property = itr->value;
//	if (!property.IsDouble())
//	{
//		return false;
//	}
//
//	outFloat = property.GetDouble();
//	return true;
//}
//
//bool JsonHelper::GetWString(const rapidjson::Value& inObject, const char* inProperty, W_STR& outStr)
//{
//	auto itr = inObject.FindMember(inProperty);
//	if (itr == inObject.MemberEnd())
//	{
//		return false;
//	}
//
//	auto& property = itr->value;
//	if (!property.IsString())
//	{
//		return false;
//	}
//
//	outStr = mtw(property.GetString());
//	return true;
//}
//
//bool JsonHelper::GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool)
//{
//	auto itr = inObject.FindMember(inProperty);
//	if (itr == inObject.MemberEnd())
//	{
//		return false;
//	}
//
//	auto& property = itr->value;
//	if (!property.IsBool())
//	{
//		return false;
//	}
//
//	outBool = property.GetBool();
//	return true;
//}
//
//bool JsonHelper::GetFloat2(const rapidjson::Value& inObject, const char* inProperty, XMFLOAT2& outFloat2)
//{
//	auto itr = inObject.FindMember(inProperty);
//	if (itr == inObject.MemberEnd())
//	{
//		return false;
//	}
//
//	auto& property = itr->value;
//	if (!property.IsArray() || property.Size() != 2)
//	{
//		return false;
//	}
//
//	for (rapidjson::SizeType i = 0; i < 2; i++)
//	{
//		if (!property[i].IsNumber())
//		{
//			return false;
//		}
//	}
//
//	outFloat2.x = property[0].GetDouble();
//	outFloat2.y = property[1].GetDouble();
//
//	return true;
//}
//
//bool JsonHelper::GetFloat3(const rapidjson::Value& inObject, const char* inProperty, XMFLOAT3& outFloat3)
//{
//	auto itr = inObject.FindMember(inProperty);
//	if (itr == inObject.MemberEnd())
//	{
//		return false;
//	}
//
//	auto& property = itr->value;
//	if (!property.IsArray() || property.Size() != 3)
//	{
//		return false;
//	}
//
//	for (rapidjson::SizeType i = 0; i < 3; i++)
//	{
//		if (!property[i].IsNumber())
//		{
//			return false;
//		}
//	}
//	
//	outFloat3.x = property[0].GetDouble();
//	outFloat3.y = property[1].GetDouble();
//	outFloat3.z = property[2].GetDouble();
//
//	return true;
//}
//
//bool JsonHelper::GetFloat4(const rapidjson::Value& inObject, const char* inProperty, XMFLOAT4& outFloat4)
//{
//	auto itr = inObject.FindMember(inProperty);
//	if (itr == inObject.MemberEnd())
//	{
//		return false;
//	}
//
//	auto& property = itr->value;
//	if (!property.IsArray() || property.Size() != 4)
//	{
//		return false;
//	}
//
//	for (rapidjson::SizeType i = 0; i < 4; i++)
//	{
//		if (!property[i].IsNumber())
//		{
//			return false;
//		}
//	}
//
//	outFloat4.x = property[0].GetDouble();
//	outFloat4.y = property[1].GetDouble();
//	outFloat4.z = property[2].GetDouble();
//	outFloat4.w = property[3].GetDouble();
//
//	return true;
//}
//
//void JsonHelper::AddInt(rapidjson::Document::AllocatorType& alloc,
//	rapidjson::Value& inObject, const char* name, int value)
//{
//	rapidjson::Value v(value);
//	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
//}
//
//void JsonHelper::AddFloat(rapidjson::Document::AllocatorType& alloc,
//	rapidjson::Value& inObject, const char* name, float value)
//{
//	rapidjson::Value v(value);
//	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
//}
//
//void JsonHelper::AddWString(rapidjson::Document::AllocatorType& alloc,
//	rapidjson::Value& inObject, const char* name, const W_STR& value)
//{
//	rapidjson::Value v;
//	v.SetString(wtm(value).c_str(), static_cast<rapidjson::SizeType>(value.length()),
//		alloc);
//	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
//}
//
//void JsonHelper::AddBool(rapidjson::Document::AllocatorType& alloc,
//	rapidjson::Value& inObject, const char* name, bool value)
//{
//	rapidjson::Value v(value);
//	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
//}
//
//void JsonHelper::AddFloat2(rapidjson::Document::AllocatorType& alloc,
//	rapidjson::Value& inObject, const char* name, const XMFLOAT2& value)
//{
//	// Create an array
//	rapidjson::Value v(rapidjson::kArrayType);
//	// Push back elements
//	v.PushBack(rapidjson::Value(value.x).Move(), alloc);
//	v.PushBack(rapidjson::Value(value.y).Move(), alloc);
//
//	// Add array to inObject
//	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
//}
//
//void JsonHelper::AddFloat3(rapidjson::Document::AllocatorType& alloc,
//	rapidjson::Value& inObject, const char* name, const XMFLOAT3& value)
//{
//	// Create an array
//	rapidjson::Value v(rapidjson::kArrayType);
//	// Push back elements
//	v.PushBack(rapidjson::Value(value.x).Move(), alloc);
//	v.PushBack(rapidjson::Value(value.y).Move(), alloc);
//	v.PushBack(rapidjson::Value(value.z).Move(), alloc);
//
//	// Add array to inObject
//	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
//}
//
//void JsonHelper::AddFloat4(rapidjson::Document::AllocatorType& alloc,
//	rapidjson::Value& inObject, const char* name, const XMFLOAT4& value)
//{
//	// Create an array
//	rapidjson::Value v(rapidjson::kArrayType);
//	// Push back elements
//	v.PushBack(rapidjson::Value(value.x).Move(), alloc);
//	v.PushBack(rapidjson::Value(value.y).Move(), alloc);
//	v.PushBack(rapidjson::Value(value.z).Move(), alloc);
//	v.PushBack(rapidjson::Value(value.w).Move(), alloc);
//
//	// Add array to inObject
//	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
//}