/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-1-26
 * Author : scott.cgi
 * Version: 0.0.0
 */


#ifndef JSON_H
#define JSON_H


#include "Engine/Toolkit/Utils/ArrayStrMap.h"
#include "Engine/Toolkit/Utils/ArrayList.h"


typedef enum
{
    JsonType_Object,
    JsonType_Array,
    JsonType_String,
    JsonType_Float,
    JsonType_Null,
}
JsonType;


typedef struct
{
   ArrayStrMap(objectKey, JsonValue*) valueMap[1];
}
JsonObject;


typedef struct
{
   ArrayList(JsonValue*) valueList[1];
}
JsonArray;


typedef struct
{
    JsonType type;

    union
    {
        /**
         * JsonType_String
         */
        char*       jsonString;

        /**
         * JsonType_Object
         */
        JsonObject* jsonObject;

        /**
         * JsonType_Array
         */
        JsonArray*  jsonArray;

        /**
         * JsonType_Float
         */
        float       jsonFloat;
    };
}
JsonValue;


struct AJsonObject
{
    bool        (*GetBool)         (JsonObject* object, char* key, bool  defaultValue);
    int         (*GetInt)          (JsonObject* object, char* key, int   defaultValue);
    float       (*GetFloat)        (JsonObject* object, char* key, float defaultValue);
    JsonType    (*GetType)         (JsonObject* object, char* key);

    /**
     * When JsonValue released string value will free
     */
    char*       (*GetString)       (JsonObject* object, char* key, char* defaultValue);

    /**
     * Not found return NULL
     */
    JsonObject* (*GetObject)       (JsonObject* object, char* key);

    /**
     * Not found return NULL
     */
    JsonArray*  (*GetArray)        (JsonObject* object, char* key);

    /**
     * Get JsonObject key
     */
    char*       (*GetKey)          (JsonObject* object, int   index);

    /**
     * Get JsonObject in index of JsonObject map
     */
    JsonObject* (*GetObjectByIndex)(JsonObject* object, int   index);

    /**
     * Get JsonArray in index of JsonObject map
     */
    JsonArray*  (*GetArrayByIndex) (JsonObject* object, int   index);
};


extern struct AJsonObject AJsonObject[1];


struct AJsonArray
{
    bool        (*GetBool)      (JsonArray* array, int index);
    int         (*GetInt)       (JsonArray* array, int index);
    float       (*GetFloat)     (JsonArray* array, int index);
    JsonType    (*GetType)      (JsonArray* array, int index);

    /**
     * When JsonValue released string value will free
     */
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
    JsonValue* (*Parse)         (char* jsonString);

    /**
     * Parse with Json file, return root JsonValue
     */
    JsonValue* (*ParseWithFile) (char* jsonPath);

    /**
     * Release JsonValue member memory space and free itself
     * if release root JsonValue will free all memory space
     */
    void       (*Release)       (JsonValue*  jsonValue);
};


extern struct AJson AJson[1];


#endif
