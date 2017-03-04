/*
 *
 *
 *  Created on: 2013-5-29
 *      Author: scott-cgi
 */

#ifndef json_h
#define json_h

#include "Mojoc/Toolkit/Utils/ArrayStrMap.h"
#include "Mojoc/Toolkit/Utils/ArrayList.h"



typedef enum
{
	json_object,
	json_array,
	json_string,
	json_float,
	json_null,
}
JsonType;



typedef struct
{
   ArrayStrMap(objectKey, JsonValue*) arrayStrMap[1];
}
JsonObject;


typedef struct
{
   ArrayList(JsonValue*) arrayList[1];
}
JsonArray;



typedef struct
{
	JsonType type;

	union
	{
		/** json_string */
		char*       stringValue;
		/** json_object */
		JsonObject* object;
		/** json_array */
		JsonArray*  array;
		/** json_float */
		float       floatValue;
	};
}
JsonValue;



struct AJsonObject
{
	bool        (*GetBool)         (JsonObject* object, const char* key, bool  defaultValue);
	int         (*GetInt)          (JsonObject* object, const char* key, int   defaultValue);
	float       (*GetFloat)        (JsonObject* object, const char* key, float defaultValue);
	JsonType    (*GetType)         (JsonObject* object, const char* key);

	/**
	 * When JsonValue released string value will free
	 */
	char*       (*GetString)       (JsonObject* object, const char* key, char* defaultValue);

	/** Not found return NULL */
	JsonObject* (*GetObject)       (JsonObject* object, const char* key);

	/** Not found return NULL */
	JsonArray*  (*GetArray)        (JsonObject* object, const char* key);

	const char* (*GetKey)          (JsonObject* object, int   index);

	/** Get JsonObject in index of JsonObject map */
	JsonObject* (*GetObjectByIndex)(JsonObject* object, int   index);

	/** Get JsonArray in index of JsonObject map */
	JsonArray*  (*GetArrayByIndex) (JsonObject* object, int   index);
};

extern struct AJsonObject AJsonObject[1];



struct AJsonArray
{
	bool        (*GetBool)      (JsonArray* array, int index);
	int         (*GetInt)       (JsonArray* array, int index);
	float       (*GetFloat)     (JsonArray* array, int index);

	JsonType    (*GetType)      (JsonArray* array, int index);

	/** When JsonValue released string value will free */
	char*       (*GetString)    (JsonArray* array, int index);

	JsonObject* (*GetObject)    (JsonArray* array, int index);
	JsonArray*  (*GetArray)     (JsonArray* array, int index);

};

extern struct AJsonArray AJsonArray[1];



struct AJson
{

	/**
	 * Parse with Json string, return root JsonValue
	 */
	JsonValue* (*Parse)         (const char* jsonString);

	/**
	 * Parse with Json file, return root JsonValue
	 */
	JsonValue* (*ParseWithFile) (const char* jsonPath);


	/**
	 * Release JsonValue member memory space and free itself
	 * if release root JsonValue will free all memory space
	 */
	void       (*Release)       (JsonValue*  jsonValue);
};

extern struct AJson AJson[1];


#endif
