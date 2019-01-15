/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-1-26
 * Update: 2019-1-8
 * Author: scott.cgi
 */

 
#ifndef JSON_H
#define  JSON_H


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
    float        (*GetFloat)        (JsonObject* object, const char* key, float  defaultValue);
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
    bool        (*GetBool)      (JsonArray* array, int index);
    int         (*GetInt)       (JsonArray* array, int index);
    float        (*GetFloat)     (JsonArray* array, int index);
    JsonType    (*GetType)      (JsonArray* array, int index);

    /**
     * When JsonValue released the string value will free.
     */
    char*       (*GetString)    (JsonArray* array, int index);
    JsonObject* (*GetObject)    (JsonArray* array, int index);
    JsonArray*  (*GetArray)     (JsonArray* array, int index);

};


extern struct AJsonArray AJsonArray[1];


/**
 * Parse Json string or file.
 */
struct AJson
{
    /**
     * Parse the Json string, return root JsonValue.
     */
    JsonValue* (*Parse)      (const char* jsonString);

    /**
     * Parse the Json file from jsonResourceFilePath, return root JsonValue.
     *
     * jsonResourceFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    JsonValue* (*ParseFile)  (const char* jsonResourceFilePath);

    /**
     * Release JsonValue member memory space and free itself,
     * if release root JsonValue will free all memory space.
     */
    void       (*Delete)     (JsonValue* jsonValue);
};


extern struct AJson AJson[1];


#endif
