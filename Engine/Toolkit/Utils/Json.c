/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2013-5-29
 * Author: scott.cgi
 */

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "Engine/Toolkit/Utils/Json.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/FileTool.h"
#include "ArrayStrMap.h"


/**
 * The JsonValue which
 * if json_array  free each items and recursive
 * if json_object free each K-V   and recursive
 */
static void Release(JsonValue* value)
{
	// JsonValue hold the whole memory
	// so free JsonValue will be release JsonValue's memory

	switch (value->type)
	{
		case json_array:
		{
			ArrayList* list = value->array->arrayList;
			for (int i = 0; i < list->size; i++)
			{
				Release(AArrayList_Get(list, i, JsonValue*));
			}

			AArrayList->Release(list);

            break;
		}

		case json_object:
		{
			ArrayStrMap* map = value->object->arrayStrMap;
			for (int i = 0; i < map->elementList->size; i++)
			{
				Release(AArrayStrMap_GetAt(map, i, JsonValue*));
			}

			AArrayStrMap->Release(map);

            break;
		}
	}

	free(value);
}



static inline JsonValue* CreateJsonValue(void* data, size_t valueSize, JsonType type)
{
	JsonValue* value = (JsonValue*) malloc(sizeof(JsonValue) + valueSize);

	switch (type)
	{
		case json_float:
			break;

	    case json_string:
	    	value->stringValue = memcpy
                                 (
                                     (char*) value + sizeof(JsonValue),
                                     data,
                                     valueSize
                                 );
            break;

	    case json_array:
	    	value->array = (JsonArray*) ((char*) value + sizeof(JsonValue));
	    	AArrayList->Init(sizeof(JsonValue*), value->array->arrayList);
	    	break;

	    case json_object:
	    	value->object = (JsonObject*) ((char*) value + sizeof(JsonValue));
	    	AArrayStrMap->Init(sizeof(JsonValue*), value->object->arrayStrMap);
	    	break;

	    default:
	    	ALog_A(false, "CreateJsonValue unknown JsonType = %d", type);
	}

	value->type = type;

	return value;
}


//--------------------------------------------------------------------------------------------------


static bool ObjectGetBool(JsonObject* object, char* key, bool defaultValue)
{
	JsonValue* jsonValue = AArrayStrMap_Get(object->arrayStrMap, key, JsonValue*);
	return jsonValue != NULL ? strcmp(jsonValue->stringValue, "true") == 0 : defaultValue;
}


static int ObjectGetInt(JsonObject* object, char* key, int defaultValue)
{
	JsonValue* jsonValue = AArrayStrMap_Get(object->arrayStrMap, key, JsonValue*);
	
	if (jsonValue != NULL)
	{
		return jsonValue->floatValue;
	}
	
	return defaultValue;
}


static float ObjectGetFloat(JsonObject* object, char* key, float defaultValue)
{
	JsonValue* jsonValue = AArrayStrMap_Get(object->arrayStrMap, key, JsonValue*);
	
	if (jsonValue != NULL)
	{
		return jsonValue->floatValue;
	}
	
	return defaultValue;
}


static char* ObjectGetString(JsonObject* object, char* key, char* defaultValue)
{
	JsonValue* jsonValue = AArrayStrMap_Get(object->arrayStrMap, key, JsonValue*);
	return jsonValue != NULL ? jsonValue->stringValue : defaultValue;
}


static JsonObject* ObjectGetObject(JsonObject* object, char* key)
{
	JsonValue* jsonValue = AArrayStrMap_Get(object->arrayStrMap, key, JsonValue*);
	return jsonValue != NULL ? jsonValue->object : NULL;
}


static JsonArray* ObjectGetArray(JsonObject* object, char* key)
{
	JsonValue* jsonValue = AArrayStrMap_Get(object->arrayStrMap, key, JsonValue*);
	return jsonValue != NULL ? jsonValue->array : NULL;
}


static JsonType ObjectGetType(JsonObject* object, char* key)
{
	JsonValue* jsonValue = AArrayStrMap_Get(object->arrayStrMap, key, JsonValue*);

	if (jsonValue == NULL)
	{
		return json_null;
	}

	return jsonValue->type;
}


static char* ObjectGetKey(JsonObject* object, int index)
{
	return AArrayStrMap->GetKey(object->arrayStrMap, index);
}


static JsonObject* ObjectGetObjectByIndex(JsonObject* object, int index)
{
	return AArrayStrMap_GetAt(object->arrayStrMap, index, JsonValue*)->object;
}


static JsonArray* ObjectGetArrayByIndex(JsonObject* object, int index)
{
	return AArrayStrMap_GetAt(object->arrayStrMap, index, JsonValue*)->array;
}


struct AJsonObject AJsonObject[1] =
{
	ObjectGetBool,
	ObjectGetInt,
	ObjectGetFloat,
	ObjectGetType,
	ObjectGetString,
	ObjectGetObject,
	ObjectGetArray,
	ObjectGetKey,
	ObjectGetObjectByIndex,
	ObjectGetArrayByIndex,
};


//--------------------------------------------------------------------------------------------------


static bool ArrayGetBool(JsonArray* array, int index)
{
	return strcmp(AArrayList_Get(array->arrayList, index, JsonValue*)->stringValue, "true") == 0;
}


static int ArrayGetInt(JsonArray* array, int index)
{
	return AArrayList_Get(array->arrayList, index, JsonValue*)->floatValue;
}

static float ArrayGetFloat(JsonArray* array, int index)
{
	return AArrayList_Get(array->arrayList, index, JsonValue*)->floatValue;
}


static char* ArrayGetString(JsonArray* array, int index)
{
	return AArrayList_Get(array->arrayList, index, JsonValue*)->stringValue;
}


static JsonObject* ArrayGetObject(JsonArray* array, int index)
{
	return AArrayList_Get(array->arrayList, index, JsonValue*)->object;
}


static JsonArray* ArrayGetArray(JsonArray* array, int index)
{
	return AArrayList_Get(array->arrayList, index, JsonValue*)->array;
}


static JsonType ArrayGetType(JsonArray* array, int index)
{
	if (index < 0 || index >= array->arrayList->size)
	{
		return json_null;
	}
	
	return AArrayList_Get(array->arrayList, index, JsonValue*)->type;
}


struct AJsonArray AJsonArray[1] =
{
	ArrayGetBool,
	ArrayGetInt,
	ArrayGetFloat,
	ArrayGetType,
	ArrayGetString,
	ArrayGetObject,
	ArrayGetArray,
};


//--------------------------------------------------------------------------------------------------


// skip whitespace and CR/LF
static inline void SkipWhiteSpace(char** jsonPtr)
{
	char* json = *jsonPtr;

	while (*json == ' ' || *json == '\t' || *json == '\n' || *json == '\r')
	{
		json++;
	}

	ALog_A(json != NULL, "The Json parse error on NULL, json is incomplete");
	
	*jsonPtr = json;
}


#define parse_number_validate_by_strtof
#ifdef  parse_number_validate_by_strtof


static inline void* ParseNumber(char** jsonPtr)
{
	char* endPtr;

	JsonValue* value  = CreateJsonValue(NULL, 0, json_float);
	value->floatValue = strtof(*jsonPtr, &endPtr);

	// record char after number
	char c  = *endPtr;

	// insert number string end
	*endPtr = '\0';

	ALog_D("Json number = %s", *jsonPtr);

	// restore char after number
	*endPtr  = c;
	*jsonPtr = endPtr;

	return value;
}


#else


static inline void* ParseNumber(char** jsonPtr)
{
    char* json = (char*) *jsonPtr;
	char  c;

	do
	{
		c = *(++json);
	}
	while (isdigit(c) || c == '-' || c == '.' || c == 'e' || c == 'E');

	// insert number string end
	*json = '\0';
	ALog_D("Json number = %s", *jsonPtr);

	JsonValue* value  = CreateJsonValue(NULL, 0, json_float);
	value->floatValue = atof(*jsonPtr);

	// restore char after number
	*json             = c;
	*jsonPtr          = json;

	return value;
}


#endif
#undef parse_number_validate_by_strtof


static inline int SkipString(char** jsonPtr)
{
	// skip '\"'
	(*jsonPtr)++;

	char c     = **jsonPtr;
	int  count = 0;

	while (c != '\"')
	{
		// skip escaped quotes
		if (c == '\\')
		{
			do
			{
				count += 2;
				c      = *(*jsonPtr + count);
			}
			while (c == '\\');
		}
		else
		{
			count++;
			c = *(*jsonPtr + count);
		}
	}

	// how many char skip
	return count;
}


static inline JsonValue* ParseString(char** jsonPtr)
{
	int        count          = SkipString(jsonPtr);
	JsonValue* value          = CreateJsonValue((void*) (*jsonPtr), (count + 1) * sizeof(char), json_string);
	
	value->stringValue[count] = '\0';

	// skip whole string include the other '\"'
	*jsonPtr                 += count + 1;
	
	ALog_D("Json string = %s", value->stringValue);
	
	return value;
}


// predefine
static inline JsonValue* ParseValue(char** jsonPtr);


static inline JsonValue* ParseArray(char** jsonPtr)
{
	JsonValue* jsonValue = CreateJsonValue(NULL, sizeof(JsonArray), json_array);
	ArrayList* list      = jsonValue->array->arrayList;

	ALog_D("Json Array: [");
	
	// skip '['
	(*jsonPtr)++;

	SkipWhiteSpace(jsonPtr);
	
	// empty array
	if (**jsonPtr == ']')
	{
		goto label_json_array_end;
	}
	
	do
	{
		JsonValue* value = ParseValue(jsonPtr);
		// add Array element
		AArrayList_Add(list, value);
		 
		 SkipWhiteSpace(jsonPtr);

		 if (**jsonPtr == ',')
		 {
			 (*jsonPtr)++;

			 SkipWhiteSpace(jsonPtr);

			 if (**jsonPtr == ']')
			 {
				 goto label_json_array_end;
			 }
		 }
		 else
		 {
			 break;
		 }
	}
	while (true);
	
	SkipWhiteSpace(jsonPtr);
	ALog_A(**jsonPtr == ']', "Json Array not has ']', error char = %c ", **jsonPtr);
	
	label_json_array_end:
	// skip ']'
	(*jsonPtr)++;
	ALog_D("] JsonArray element count = %d", list->size);
	
	return jsonValue;
}


static inline JsonValue* ParseObject(char** jsonPtr)
{
	JsonValue*   jsonValue = CreateJsonValue(NULL, sizeof(JsonObject), json_object);
	ArrayStrMap* map       = jsonValue->object->arrayStrMap;

	ALog_D("Json Object: {");
	
	// skip '{'
	(*jsonPtr)++;

	SkipWhiteSpace(jsonPtr);
	
	// empty object
	if (**jsonPtr == '}')
	{
		goto label_json_object_end;
	}
	
	do
	{
		SkipWhiteSpace(jsonPtr);

		if (**jsonPtr == '}')
		{
			goto label_json_object_end;
		}
		
		ALog_A(**jsonPtr == '\"', "Json object parse error, char = %c, should be '\"' ", **jsonPtr);

		int   keyLen = SkipString(jsonPtr);
		char* key    = (char*) *jsonPtr;

		// skip whole string include the other '\"'
		*jsonPtr += keyLen + 1;

		SkipWhiteSpace(jsonPtr);
		ALog_A((**jsonPtr) == ':', "Json object parse error, char = %c, should be ':' ", **jsonPtr);

		// skip ':'
		(*jsonPtr)++;
		JsonValue*  value = ParseValue(jsonPtr);
		char        c     = key[keyLen];

		// make string end in json string
		key[keyLen]       = '\0';

		ALog_D("Json key = %s", key);

		// set object element
		AArrayStrMap_TryPut(map, key, value);

		// restore char after key string
		key[keyLen]       = c;

		SkipWhiteSpace(jsonPtr);

		if (**jsonPtr == ',')
		{
			(*jsonPtr)++;
		}
		else
		{
			break;
		}

	}
	while (true);

	SkipWhiteSpace(jsonPtr);
	ALog_A(**jsonPtr == '}', "Json Object not has '}', error char = %c ", **jsonPtr);
	
	label_json_object_end:
	// skip '}'
	(*jsonPtr)++;
	ALog_D("} JsonObject elements count = %d", map->elementList->size);

	return jsonValue;
}


/**
 * ParseValue changed the *jsonPtr, so if *jsonPtr is direct malloc will cause error
 */
static inline JsonValue* ParseValue(char** jsonPtr)
{
	SkipWhiteSpace(jsonPtr);

	char c = **jsonPtr;

	switch (c)
	{
		case '[':
			return ParseArray(jsonPtr);

		case '{':
			return ParseObject(jsonPtr);

		case '\"':
			return ParseString(jsonPtr);

		default:
			if (isdigit(c) || c == '-')
			{
				return ParseNumber(jsonPtr);
			}
			else if (strncmp(*jsonPtr, "null", 4) == 0)
			{
				ALog_D("Json null");

				(*jsonPtr) += 4;

				// copy with '\0'
				return CreateJsonValue("null", 5, json_string);

			}
			else if (strncmp(*jsonPtr, "false", 5) == 0)
			{
				ALog_D("Json false");

				(*jsonPtr) += 5;

				// copy with '\0'
				return CreateJsonValue("false", 6, json_string);

			}
			else if (strncmp(*jsonPtr, "true", 4) == 0)
			{
				ALog_D("Json true");

				(*jsonPtr) += 4;

				// copy with '\0'
				return CreateJsonValue("true", 5, json_string);
			}
			else
			{
				ALog_A(false, "Invalid json value type, error char = %c", c);
			}

		    break;
	}

	return NULL;
}


static JsonValue* Parse(char* jsonString)
{
	return ParseValue(&jsonString);
}


static JsonValue* ParseWithFile(char* jsonPath)
{
	char*        jsonString = AFileTool->CreateStringFromRes(jsonPath);
	JsonValue*   value      = Parse(jsonString);
	free(jsonString);

	return value;
}


struct AJson AJson[1] =
{
	Parse,
	ParseWithFile,
	Release,
};