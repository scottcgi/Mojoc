/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The Mojoc game engine is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2013-1-26
 * Update: 2019-1-8
 * Author: scott.cgi
 */

 
#ifndef JSON_H
#define JSON_H


#include "Engine/Toolkit/Utils/ArrayStrMap.h"
#include "Engine/Toolkit/Utils/ArrayList.h"


/**
 * Json value type.
 */
typedef enum
{
    JsonType_Object,
    JsonType_Array,
    JsonType_String,
    JsonType_Float,
    JsonType_Null,
}
JsonType;


/**
 * For json object that contains a set of k-v pairs.
 */
typedef struct
{
   ArrayStrMap(objectKey, JsonValue*) valueMap[1];
}
JsonObject;


/**
 * For json array that contains a list of json value.
 */
typedef struct
{
   ArrayList(JsonValue*) valueList[1];
}
JsonArray;


/**
 * One json value.
 */
typedef struct
{
    JsonType type;

    union
    {
        /**
         * For JsonType_String.
         */
        char*       jsonString;

        /**
         * For JsonType_Object.
         */
        JsonObject* jsonObject;

        /**
         * For JsonType_Array.
         */
        JsonArray*  jsonArray;

        /**
         * For JsonType_Float and int value.
         */
        float       jsonFloat;
    };
}
JsonValue;


/**
 * Get kinds of value from JsonObject.
 */
struct AJsonObject
{
    bool        (*GetBool)         (JsonObject* object, const char* key, bool  defaultValue);
    int         (*GetInt)          (JsonObject* object, const char* key, int   defaultValue);
    float       (*GetFloat)        (JsonObject* object, const char* key, float defaultValue);
    JsonType    (*GetType)         (JsonObject* object, const char* key);

    /**
     * When JsonValue released the string value will free.
     */
    char*       (*GetString)       (JsonObject* object, const char* key, const char* defaultValue);

    /**
     * If not found return NULL.
     */
    JsonObject* (*GetObject)       (JsonObject* object, const char* key);

    /**
     * If not found return NULL.
     */
    JsonArray*  (*GetArray)        (JsonObject* object, const char* key);

    /**
     * Get JsonObject's key.
     */
    const char* (*GetKey)          (JsonObject* object, int index);

    /**
     * Get index of JsonObject in JsonObject map.
     */
    JsonObject* (*GetObjectByIndex)(JsonObject* object, int index);

    /**
     * Get index of JsonObject in JsonArray map.
     */
    JsonArray*  (*GetArrayByIndex) (JsonObject* object, int index);
};


extern struct AJsonObject AJsonObject[1];


/**
 * Get kinds of value from JsonArray.
 */
struct AJsonArray
{
    bool        (*GetBool)  (JsonArray* array, int index);
    int         (*GetInt)   (JsonArray* array, int index);
    float       (*GetFloat) (JsonArray* array, int index);
    JsonType    (*GetType)  (JsonArray* array, int index);

    /**
     * When JsonValue released the string value will free.
     */
    char*       (*GetString)(JsonArray* array, int index);
    JsonObject* (*GetObject)(JsonArray* array, int index);
    JsonArray*  (*GetArray) (JsonArray* array, int index);

};


extern struct AJsonArray AJsonArray[1];


/**
 * Control Json data.
 */
struct AJson
{
    /**
     * Parse the Json string, return root JsonValue.
     */
    JsonValue* (*Parse)    (const char* jsonString);

    /**
     * Parse the Json file from jsonFilePath, return root JsonValue.
     *
     * jsonFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    JsonValue* (*ParseFile)(const char* jsonFilePath);

    /**
     * Destroy JsonValue member memory space and free itself,
     * if Destroy root JsonValue will free all memory space.
     *
     * important: after Destroy the jsonValue will be invalidated.
     */
    void       (*Destroy)  (JsonValue* jsonValue);
};


extern struct AJson AJson[1];


#endif
