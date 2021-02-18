/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2013-5-29
 * Update   : 2019-1-8
 * Author   : scott.cgi
 */


#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Engine/Toolkit/Utils/Json.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/FileTool.h"


//----------------------------------------------------------------------------------------------------------------------


/**
 * If the JsonValue is JsonType_Array,  then free each items and do recursively.
 * if the JsonValue is JsonType_Object, then free each k-v   and do recursively.
 */
static void Destroy(JsonValue* value)
{
    // JsonValue hold the whole memory
    // so free JsonValue will be release JsonValue's memory

    switch (value->type)
    {
        case JsonType_Array:
        {
            ArrayList* list = value->jsonArray->valueList;
            for (int i = 0; i < list->size; ++i)
            {
                Destroy(AArrayList_Get(list, i, JsonValue*));
            }

            AArrayList->Release(list);
            break;
        }

        case JsonType_Object:
        {
            ArrayStrMap* map = value->jsonObject->valueMap;
            for (int i = 0; i < map->elementList->size; ++i)
            {
                Destroy(AArrayStrMap_GetAt(map, i, JsonValue*));
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


static JsonValue* CreateJsonValue(void* data, size_t valueSize, JsonType type)
{
    JsonValue* value = malloc(sizeof(JsonValue) + valueSize);

    switch (type)
    {
        case JsonType_Float:
            break;

        case JsonType_String:
            value->jsonString = memcpy((char*) value + sizeof(JsonValue), data, valueSize);
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


static bool ObjectGetBool(JsonObject* object, const char* key, bool defaultValue)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);
    return jsonValue != NULL ? strcmp(jsonValue->jsonString, "true") == 0 : defaultValue;
}


static int ObjectGetInt(JsonObject* object, const char* key, int defaultValue)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);
    
    if (jsonValue != NULL)
    {
        return (int) jsonValue->jsonFloat;
    }
    
    return defaultValue;
}


static float ObjectGetFloat(JsonObject* object, const char* key, float defaultValue)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);
    
    if (jsonValue != NULL)
    {
        return jsonValue->jsonFloat;
    }
    
    return defaultValue;
}


static char* ObjectGetString(JsonObject* object, const char* key, const char* defaultValue)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);
    return jsonValue != NULL ? jsonValue->jsonString : (char*) defaultValue;
}


static JsonObject* ObjectGetObject(JsonObject* object, const char* key)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);
    return jsonValue != NULL ? jsonValue->jsonObject : NULL;
}


static JsonArray* ObjectGetArray(JsonObject* object, const char* key)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);
    return jsonValue != NULL ? jsonValue->jsonArray : NULL;
}


static JsonType ObjectGetType(JsonObject* object, const char* key)
{
    JsonValue* jsonValue = AArrayStrMap_Get(object->valueMap, key, JsonValue*);

    if (jsonValue == NULL)
    {
        return JsonType_Null;
    }

    return jsonValue->type;
}


static const char* ObjectGetKey(JsonObject* object, int index)
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
{{
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
}};


//----------------------------------------------------------------------------------------------------------------------


static bool ArrayGetBool(JsonArray* array, int index)
{
    return strcmp(AArrayList_Get(array->valueList, index, JsonValue*)->jsonString, "true") == 0;
}


static int ArrayGetInt(JsonArray* array, int index)
{
    return (int) AArrayList_Get(array->valueList, index, JsonValue*)->jsonFloat;
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
{{
    ArrayGetBool,
    ArrayGetInt,
    ArrayGetFloat,
    ArrayGetType,
    ArrayGetString,
    ArrayGetObject,
    ArrayGetArray,
}};


//----------------------------------------------------------------------------------------------------------------------


static inline void SkipWhiteSpace(const char** jsonPtr)
{
    const char* json = *jsonPtr;

    while (true)
    {
        switch (*json)
        {
            case ' ' :
            case '\t':
            case '\n':
            case '\r':
                ++json;
                continue;
            default:
                break;
        }
        break;
    }

    ALog_A(json != NULL, "The Json parse error on NULL, json is incomplete.");
    
    *jsonPtr = json;
}


static void* ParseNumber(const char** jsonPtr)
{
    char* endPtr;

    JsonValue* value = CreateJsonValue(NULL, 0, JsonType_Float);
    value->jsonFloat = strtof(*jsonPtr, &endPtr);

    ALog_D("Json number = %.*s", (int) (endPtr - *jsonPtr), *jsonPtr);
    
    *jsonPtr = endPtr;

    return value;
}


static int SkipString(const char** jsonPtr, const char** outStrStart)
{
    // skip '"'
    const char* json  = ++(*jsonPtr);
    int         count = 0;
    char        c;

    // check end '"'
    while ((c = json[count++]) != '"')
    {
        if (c == '\\')
        {
            // skip escaped quotes
            // the escape char may be '\"'，which will break while
            ++count;
        }
    }

    *outStrStart = json;
    
    // already skipped the string end '"'
    *jsonPtr    += count;

    // how many char skipped
    // count contains the string end '"', so -1
    return count - 1;
}


static JsonValue* ParseString(const char** jsonPtr)
{
    const char* strStart;
    int         length        = SkipString(jsonPtr, &strStart);
    JsonValue*  value         = CreateJsonValue((void*) strStart, (length + 1) * sizeof(char), JsonType_String);
    value->jsonString[length] = '\0';

    ALog_D("Json string = %s", value->jsonString);

    return value;
}


// predefine
static JsonValue* ParseValue(const char** jsonPtr);


static JsonValue* ParseArray(const char** jsonPtr)
{
    JsonValue* jsonValue = CreateJsonValue(NULL, sizeof(JsonArray), JsonType_Array);
    ArrayList* list      = jsonValue->jsonArray->valueList;

    ALog_D("Json Array: [");
    
    // skip '['
    ++(*jsonPtr);

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
            ++(*jsonPtr);
        }
        else
        {
            ALog_A(**jsonPtr == ']', "Json Array not has ']', error char = %c ", **jsonPtr);
            break;
        }
    }
    while (true);

    // skip ']'
    ++(*jsonPtr);
    ALog_D("] JsonArray element count = %d", list->size);
    
    return jsonValue;
}


static JsonValue* ParseObject(const char** jsonPtr)
{
    JsonValue*   jsonValue = CreateJsonValue(NULL, sizeof(JsonObject), JsonType_Object);
    ArrayStrMap* map       = jsonValue->jsonObject->valueMap;

    ALog_D("Json Object: {");
    
    // skip '{'
    ++(*jsonPtr);

    do
    {
        SkipWhiteSpace(jsonPtr);

        if (**jsonPtr == '}')
        {
            break;
        }
        
        ALog_A(**jsonPtr == '"', "Json object parse error, char = %c, should be '\"' ", **jsonPtr);

        const char* strStart;
        int         keyLen = SkipString(jsonPtr, &strStart);
        char        key[keyLen + 1];
        // make string end
        key[keyLen] = '\0';

        memcpy(key, strStart, (size_t) keyLen);
        ALog_D("Json key = %s", key);

        SkipWhiteSpace(jsonPtr);
        ALog_A((**jsonPtr) == ':', "Json object parse error, char = %c, should be ':' ", **jsonPtr);

        // skip ':'
        ++(*jsonPtr);
        JsonValue* value = ParseValue(jsonPtr);
        
        // set object element
        AArrayStrMap_TryPut(map, key, value);

        SkipWhiteSpace(jsonPtr);

        if (**jsonPtr == ',')
        {
            ++(*jsonPtr);
        }
        else
        {
            ALog_A(**jsonPtr == '}', "Json Object not has '}', error char = %c ", **jsonPtr);
            break;
        }
    }
    while (true);

    // skip '}'
    ++(*jsonPtr);
    ALog_D("} JsonObject elements count = %d", map->elementList->size);

    return jsonValue;
}


/**
 * ParseValue changed the *jsonPtr, so if *jsonPtr is direct malloc will cause error
 */
static JsonValue* ParseValue(const char** jsonPtr)
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
            const char* json = *jsonPtr;

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
            const char* json = *jsonPtr;

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
            const char* json = *jsonPtr;

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


static JsonValue* Parse(const char* jsonString)
{
    return ParseValue(&jsonString);
}


static JsonValue* ParseFile(const char* jsonFilePath)
{
    char*        jsonString = AFileTool->CreateStringFromResource(jsonFilePath);
    JsonValue*   value      = Parse(jsonString);
    free(jsonString);

    return value;
}


struct AJson AJson[1] =
{{
    Parse,
    ParseFile,
    Destroy,
}};
