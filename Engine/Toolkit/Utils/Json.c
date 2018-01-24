/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-5-29
 * Author: scott.cgi
 */


#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Engine/Toolkit/Utils/Json.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/FileTool.h"


/**
 * The JsonValue which
 * if JsonType_Array  free each items and recursive
 * if JsonType_Object free each K-V   and recursive
 */
static void Release(JsonValue* value)
{
    // JsonValue hold the whole memory
    // so free JsonValue will be release JsonValue's memory

    switch (value->type)
    {
        case JsonType_Array:
        {
            ArrayList* list = value->jsonArray->valueList;
            for (int i = 0; i < list->size; i++)
            {
                Release(AArrayList_Get(list, i, JsonValue*));
            }

            AArrayList->Release(list);

            break;
        }

        case JsonType_Object:
        {
            ArrayStrMap* map = value->jsonObject->valueMap;
            for (int i = 0; i < map->elementList->size; i++)
            {
                Release(AArrayStrMap_GetAt(map, i, JsonValue*));
            }

            AArrayStrMap->Release(map);

            break;
        }
            
        case JsonType_Float:
            break;
            
        case JsonType_String:
            break;
            
        case JsonType_Null:
            break;
    }

    free(value);
}


static inline JsonValue* CreateJsonValue(void* data, size_t valueSize, JsonType type)
{
    JsonValue* value = (JsonValue*) malloc(sizeof(JsonValue) + valueSize);

    switch (type)
    {
        case JsonType_Float:
            break;

        case JsonType_String:
            value->jsonString = memcpy
                                (
                                    (char*) value + sizeof(JsonValue),
                                    data,
                                    valueSize
                                );
            break;

        case JsonType_Array:
            value->jsonArray = (JsonArray*) ((char*) value + sizeof(JsonValue));
            AArrayList->Init(sizeof(JsonValue*), value->jsonArray->valueList);
            break;

        case JsonType_Object:
            value->jsonObject = (JsonObject*) ((char*) value + sizeof(JsonValue));
            AArrayStrMap->Init(sizeof(JsonValue*), value->jsonObject->valueMap);
            break;

        default:
            ALog_A(false, "CreateJsonValue unknown JsonType = %d", type);
    }

    value->type = type;

    return value;
}


//----------------------------------------------------------------------------------------------------------------------


static bool ObjectGetBool(JsonObject* object, char* key, bool defaultValue)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);
    return jsonValue != NULL ? strcmp(jsonValue->jsonString, "true") == 0 : defaultValue;
}


static int ObjectGetInt(JsonObject* object, char* key, int defaultValue)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);
    
    if (jsonValue != NULL)
    {
        return jsonValue->jsonFloat;
    }
    
    return defaultValue;
}


static float ObjectGetFloat(JsonObject* object, char* key, float defaultValue)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);
    
    if (jsonValue != NULL)
    {
        return jsonValue->jsonFloat;
    }
    
    return defaultValue;
}


static char* ObjectGetString(JsonObject* object, char* key, char* defaultValue)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);
    return jsonValue != NULL ? jsonValue->jsonString : defaultValue;
}


static JsonObject* ObjectGetObject(JsonObject* object, char* key)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);
    return jsonValue != NULL ? jsonValue->jsonObject : NULL;
}


static JsonArray* ObjectGetArray(JsonObject* object, char* key)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);
    return jsonValue != NULL ? jsonValue->jsonArray : NULL;
}


static JsonType ObjectGetType(JsonObject* object, char* key)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);

    if (jsonValue == NULL)
    {
        return JsonType_Null;
    }

    return jsonValue->type;
}


static char* ObjectGetKey(JsonObject* object, int index)
{
    return AArrayStrMap->GetKey(object->valueMap, index);
}


static JsonObject* ObjectGetObjectByIndex(JsonObject* object, int index)
{
    return AArrayStrMap_GetAt(object->valueMap, index, JsonValue*)->jsonObject;
}


static JsonArray* ObjectGetArrayByIndex(JsonObject* object, int index)
{
    return AArrayStrMap_GetAt(object->valueMap, index, JsonValue*)->jsonArray;
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


//----------------------------------------------------------------------------------------------------------------------


static bool ArrayGetBool(JsonArray* array, int index)
{
    return strcmp(AArrayList_Get(array->valueList, index, JsonValue*)->jsonString, "true") == 0;
}


static int ArrayGetInt(JsonArray* array, int index)
{
    return AArrayList_Get(array->valueList, index, JsonValue*)->jsonFloat;
}

static float ArrayGetFloat(JsonArray* array, int index)
{
    return AArrayList_Get(array->valueList, index, JsonValue*)->jsonFloat;
}


static char* ArrayGetString(JsonArray* array, int index)
{
    return AArrayList_Get(array->valueList, index, JsonValue*)->jsonString;
}


static JsonObject* ArrayGetObject(JsonArray* array, int index)
{
    return AArrayList_Get(array->valueList, index, JsonValue*)->jsonObject;
}


static JsonArray* ArrayGetArray(JsonArray* array, int index)
{
    return AArrayList_Get(array->valueList, index, JsonValue*)->jsonArray;
}


static JsonType ArrayGetType(JsonArray* array, int index)
{
    if (index < 0 || index >= array->valueList->size)
    {
        return JsonType_Null;
    }
    
    return AArrayList_Get(array->valueList, index, JsonValue*)->type;
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


//----------------------------------------------------------------------------------------------------------------------


static inline void SkipWhiteSpace(char** jsonPtr)
{
    char* json  = *jsonPtr;

    while (true)
    {
        switch (*json)
        {
            case ' ' :
            case '\t':
            case '\n':
            case '\r':
                json++;
                continue;
        }

        break;
    }

    ALog_A(json != NULL, "The Json parse error on NULL, json is incomplete");
    
    *jsonPtr = json;
}


#define PARSE_NUMBER_VALIDATE_BY_STRTOF
#ifdef  PARSE_NUMBER_VALIDATE_BY_STRTOF


static inline void* ParseNumber(char** jsonPtr)
{
    char* endPtr;

    JsonValue* value = CreateJsonValue(NULL, 0, JsonType_Float);
    value->jsonFloat = strtof(*jsonPtr, &endPtr);

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
    char* json = *jsonPtr;

    while (true)
    {
        switch (*(++json))
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '-':
            case '+':
            case '.':
            case 'e':
            case 'E':
                continue;
        }

        break;
    }

    char c = *json;
    // insert number string end
    *json  = '\0';
    ALog_D("Json number = %s", *jsonPtr);

    JsonValue* value = CreateJsonValue(NULL, 0, JsonType_Float);
    value->jsonFloat = atof(*jsonPtr);

    // restore char after number
    *json            = c;
    *jsonPtr         = json;

    return value;
}


#endif
#undef PARSE_NUMBER_VALIDATE_BY_STRTOF


static inline int SkipString(char **jsonPtr)
{
    // skip '"'
    (*jsonPtr)++;

    char c     = **jsonPtr;
    int  count = 0;

    // check end '"'
    while (c != '"')
    {
        if (c != '\\')
        {
            count++;
        }
        else
        {
            // skip escaped quotes
            count += 2;
        }

        c = *(*jsonPtr + count);
    }

    // skip whole string include the end '"'
    *jsonPtr += count + 1;

    // how many char skipped
    return count;
}


static inline JsonValue* ParseString(char** jsonPtr)
{
    int        length         = SkipString(jsonPtr);
    char*      start          = *jsonPtr - length - 1;
    JsonValue* value          = CreateJsonValue((void*) start, (length + 1) * sizeof(char), JsonType_String);
    
    value->jsonString[length] = '\0';

    ALog_D("Json string = %s", value->stringValue);
    
    return value;
}


// predefine
static inline JsonValue* ParseValue(char** jsonPtr);


static inline JsonValue* ParseArray(char** jsonPtr)
{
    JsonValue* jsonValue = CreateJsonValue(NULL, sizeof(JsonArray), JsonType_Array);
    ArrayList* list      = jsonValue->jsonArray->valueList;

    ALog_D("Json Array: [");
    
    // skip '['
    (*jsonPtr)++;

    do
    {
        SkipWhiteSpace(jsonPtr);

        if (**jsonPtr == ']')
        {
            break;
        }

        JsonValue* value = ParseValue(jsonPtr);
        // add Array element
        AArrayList_Add(list, value);
         
        SkipWhiteSpace(jsonPtr);

        if (**jsonPtr == ',')
        {
            (*jsonPtr)++;
        }
        else
        {
            ALog_A(**jsonPtr == ']', "Json Array not has ']', error char = %c ", **jsonPtr);
            break;
        }
    }
    while (true);

    // skip ']'
    (*jsonPtr)++;
    ALog_D("] JsonArray element count = %d", list->size);
    
    return jsonValue;
}


static inline JsonValue* ParseObject(char** jsonPtr)
{
    JsonValue*   jsonValue = CreateJsonValue(NULL, sizeof(JsonObject), JsonType_Object);
    ArrayStrMap* map       = jsonValue->jsonObject->valueMap;

    ALog_D("Json Object: {");
    
    // skip '{'
    (*jsonPtr)++;

    do
    {
        SkipWhiteSpace(jsonPtr);

        if (**jsonPtr == '}')
        {
            break;
        }
        
        ALog_A(**jsonPtr == '"', "Json object parse error, char = %c, should be '\"' ", **jsonPtr);

        int   keyLen = SkipString(jsonPtr);
        char* key    = *jsonPtr - keyLen - 1;

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
            ALog_A(**jsonPtr == '}', "Json Object not has '}', error char = %c ", **jsonPtr);
            break;
        }
    }
    while (true);

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
        case '{':
            return ParseObject(jsonPtr);

        case '[':
            return ParseArray(jsonPtr);

        case '"':
            return ParseString(jsonPtr);

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '-':
            return ParseNumber(jsonPtr);

        case 'f':
        {
            char* json = *jsonPtr;

            if
            (
                json[1] == 'a' &&
                json[2] == 'l' &&
                json[3] == 's' &&
                json[4] == 'e'
            )
            {
                ALog_D("Json false");

                (*jsonPtr) += 5;

                // copy with '\0'
                return CreateJsonValue("false", 6, JsonType_String);
            }
            break;
        }

        case 't':
        {
            char* json = *jsonPtr;

            if
            (
                json[1] == 'r' &&
                json[2] == 'u' &&
                json[3] == 'e'
            )
            {
                ALog_D("Json true");

                (*jsonPtr) += 4;

                // copy with '\0'
                return CreateJsonValue("true", 5, JsonType_String);
            }
            break;
        }

        case 'n':
        {
            char* json = *jsonPtr;

            if
            (
                json[1] == 'u' &&
                json[2] == 'l' &&
                json[3] == 'l'
            )
            {
                ALog_D("Json null");

                (*jsonPtr) += 4;

                // copy with '\0'
                return CreateJsonValue("null", 5, JsonType_String);
            }
            break;
        }

        default:
            break;
    }

    ALog_A(false, "Invalid json value type, error char = %c", c);

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
