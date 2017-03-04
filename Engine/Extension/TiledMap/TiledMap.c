/*
 *
 *
 *  Created on: 2013-3-1
 *      Author: scott.cgi
 */


#include <string.h>

#include "Engine/Extension/TiledMap/TiledMap.h"

#include "Engine/Toolkit/Define/StructFrom.h"
#include "Engine/Toolkit/Utils/Json.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Graphics/OpenGL/Sprite.h"
#include "Engine/Toolkit/Platform/File.h"
#include "Engine/Graphics/OpenGL/Mesh.h"


static inline void ParseMapInfo(TiledMap* map, JsonObject* root)
{
	int mapTileWidth      = AJsonObject->GetInt(root, "tilewidth", 0);
	int mapTileHeight     = AJsonObject->GetInt(root, "tileheight",0);
	int mapWidth          = AJsonObject->GetInt(root, "width",  0) * mapTileWidth;
	int mapHeight         = AJsonObject->GetInt(root, "height", 0) * mapTileHeight;

	ALogD
	(
		"map width = %d, height = %d, tileWidth = %d, tileHeight = %d",
		mapWidth, mapHeight, mapTileWidth, mapTileHeight
	);

	map->drawable->width  = (float) mapWidth;
	map->drawable->height = (float) mapHeight;
}

static inline void ParseTilesets(TiledMap* map, JsonObject* root, const char* filePath)
{
	JsonArray* tilesets        = AJsonObject->GetArray(root, "tilesets");
	int        tilesetsLen     = tilesets->arrayList->size;

	ALogA(tilesetsLen > 0, "tileset length must more than 0");

	ALogD("tilesets length = %d", tilesetsLen);

	map->tilesetArr = AArray->Create(sizeof(Mesh), tilesetsLen);
	Mesh* meshs     = AArrayGetData(map->tilesetArr, Mesh);

	int mapTileWidth  = AJsonObject->GetInt(root, "tilewidth",  0);
	int mapTileHeight = AJsonObject->GetInt(root, "tileheight", 0);

    for (int i = 0; i < tilesetsLen; i++)
    {
    	JsonObject* tileset = AJsonArray->GetObject(tilesets, i);

		ALogA
		(
			AJsonObject->GetInt(tileset,  "spacing", 0) == 0,
			"tileset not support spcing %d > 0", AJsonObject->GetInt(tileset,  "spacing", 0)
		);

		ALogA
		(
			AJsonObject->GetInt(tileset,  "margin",  0) == 0,
			"tileset not support margin %d > 0", AJsonObject->GetInt(tileset,  "margin",  0)
		);

		int tilewidth    = AJsonObject->GetInt(tileset,  "tilewidth",  0);
		int tileheight   = AJsonObject->GetInt(tileset,  "tileheight", 0 );

		const char* name = AJsonObject->GetString(tileset, "name", NULL);

		ALogA(mapTileWidth  == tilewidth,  "tileset %s tilewidth  %d  must as same as map tilewidth  %d",  name, tilewidth, mapTileWidth);
		ALogA(mapTileHeight == tileheight, "tileset %s tileheight %d  must as same as map tileheight %d ", name, tileheight, mapTileHeight);

    	// layer used texture
    	Texture* texture;

    	const char* image = AJsonObject->GetString(tileset, "image", NULL);
        int   fileDirLen  = AFile->GetDirLength(filePath);

    	if (fileDirLen == 0)
    	{
    		texture = ATexture->Get(image);
    	}
    	else
    	{
    		char path[fileDirLen + strlen(image) + 1];

    		memcpy(path, filePath, fileDirLen);
    		path[fileDirLen] = '\0';
    		strcat(path, image);

    		texture = ATexture->Get(path);
    	}

    	AMesh->Init(texture, &meshs[i]);
    }
}


/**
 *  One block region make up of tiles
 */
typedef struct
{
	int minOffsetX;
	int minOffsetY;

	int minOffsetTextureX;
	int minOffsetTextureY;

//--------------------------------------------------------------------------------------------------
// mark tile in region when foreach tiles
//--------------------------------------------------------------------------------------------------

	int x;
	int y;
}
TileRegion;


static inline void ParseTileLayer(TiledMap* map, JsonArray* tilesets, JsonObject* layer, const char* name, ArrayList* tileRegions)
{
	float mapCenterX = AGLToolToGLX(map->drawable->width  / 2);
	float mapCenterY = AGLToolToGLY(map->drawable->height / 2);

	int tilesetsLen  = tilesets->arrayList->size;
	int layerWidth   = AJsonObject->GetInt(layer, "width", 0);
	int tilesetIndex = -1;

	int textureWidthTileNum;
	int firstTileIndex;

	int tileWidth;
	int tileHeight;

	// tileSet index in tileSets
	JsonArray* data    = AJsonObject->GetArray(layer, "data");
	int        dataLen = data->arrayList->size;

	for (int j = 0; j < dataLen; j++)
	{
		int index = AJsonArray->GetInt(data, j);

		if (index < 1)
		{
			// this index no tileSet
			continue;
		}

		JsonObject* tileset;
		for (int k = tilesetsLen - 1; k > -1; k--)
		{
			tileset        = AJsonArray->GetObject(tilesets, k);
			firstTileIndex = AJsonObject->GetInt   (tileset,  "firstgid", 0);

			if (index >= firstTileIndex)
			{
				if (tilesetIndex == -1)
				{
					tilesetIndex        = k;
					tileWidth           = AJsonObject->GetInt(tileset,  "tilewidth",  0);
					tileHeight          = AJsonObject->GetInt(tileset,  "tileheight", 0);
					textureWidthTileNum = AJsonObject->GetInt(tileset,  "imagewidth", 0)  / tileWidth;
				}

				ALogA
				(
					 tilesetIndex == k,
					 "One layer %s 's tiles must in one texture, old index %d in tileset, new index %d in tileset",
					 name, tilesetIndex, k
				 );

				break;
			}
		}

		ALogA
		(
			tilesetIndex != -1, "layer name %s data index %d not found in tilesets",
			name, index
		 );

		int         offsetX       = (j % layerWidth) * tileWidth;
		int         offsetY       = (j / layerWidth) * tileHeight;
		bool        isInRegion    = false;
		TileRegion* tileRegion;


		for (int k = 0; k < tileRegions->size; k++)
		{
			tileRegion = AArrayListGetPtr(tileRegions, k, TileRegion);

			// new tile under the region marked tile
			// this priority higher than tile adjacent situation
			// so belong this tileRegion
			if (tileRegion->y + tileHeight == offsetY)
			{
				isInRegion = true;
				break;
			}

			// new tile next to the region marked tile
			// maybe other region have tile up down situation
			if (tileRegion->x + tileWidth == offsetX)
			{
				isInRegion = true;
			}

		}

		if (isInRegion)
		{
			tileRegion->x                 = offsetX;
			tileRegion->y                 = offsetY;
		}
		else
		{
			// this is new region first tile
			tileRegion                    = AArrayListGetPtrAdd(tileRegions, TileRegion);

			tileRegion->x                 = offsetX;
			tileRegion->y                 = offsetY;

			tileRegion->minOffsetX        = offsetX;
			tileRegion->minOffsetY        = offsetY;

			int indexInTexture            = index - firstTileIndex;
			tileRegion->minOffsetTextureX = (indexInTexture % textureWidthTileNum) * tileWidth;
			tileRegion->minOffsetTextureY = (indexInTexture / textureWidthTileNum) * tileHeight;
		}
	}

	Array* subMeshArr = AArray->Create(sizeof(SubMesh*), tileRegions->size);
	AArrayStrMapPut(map->layerMap, name, subMeshArr);

	// make tile region to SubMesh
	for (int k = 0; k < tileRegions->size; k++)
	{
		TileRegion* tileRegion     = AArrayListGetPtr(tileRegions, k, TileRegion);

		// tile offset x y is top left
		// we add width and height to bottom right
		int         maxOffsetX     = tileRegion->x + tileWidth;
		int         maxOffsetY     = tileRegion->y + tileHeight;


		Quad quad[1];
		AQuad->Init
		(
			AGLToolToGLWidth (maxOffsetX - tileRegion->minOffsetX),
			AGLToolToGLHeight(maxOffsetY - tileRegion->minOffsetY),
			quad
		);

		// because texture width and height is openGL coordinate so convert this to openGL coordinate
		quad->offsetTextureX = AGLToolToGLWidth (tileRegion->minOffsetTextureX);
		quad->offsetTextureY = AGLToolToGLHeight(tileRegion->minOffsetTextureY);

		// minOffsetX and minOffsetY is relative map center
		// so we need move offset relative screen center
		quad->offsetCenterX  = AGLToolToGLX(tileRegion->minOffsetX) - mapCenterX;
		quad->offsetCenterY  = AGLToolToGLY(tileRegion->minOffsetY) - mapCenterY;

		AArrayGet(subMeshArr, k, SubMesh*) = AMesh->AddChildWithQuad
											  (
												AArrayGetPtr(map->tilesetArr, tilesetIndex, Mesh),
												quad
											  );


	}

	// clear for next layer
	AArrayList->Clear(tileRegions);
}


static inline void ParseLayer(TiledMap* map, JsonObject*  root)
{
	JsonArray* layers = AJsonObject->GetArray(root, "layers");
	ALogA(layers, "layers can no null");

	int layerLen = layers->arrayList->size;
	ALogD("layers length = %d", layerLen);

	AArrayStrMap->InitWithCapacity(sizeof(Array*), layerLen, map->layerMap);

	// collect tile regions in one layer
	ArrayList tileRegions[1];
	AArrayList->InitWithCapacity(sizeof(TileRegion), 3, tileRegions);
	tileRegions->increase = 5;

	JsonArray* tilesets = AJsonObject->GetArray(root, "tilesets");

	for (int i = 0; i < layerLen; i++)
	{
		JsonObject* layer = AJsonArray->GetObject(layers, i);
		const char* name  = AJsonObject->GetString(layer, "name", NULL);
		const char* type  = AJsonObject->GetString(layer, "type", NULL);

		ALogA(strcmp("imagelayer", type), "Can not support imagelayer, layer name %s", name);

		if (strcmp("tilelayer", type) == 0)
		{
			ParseTileLayer(map, tilesets, layer, name, tileRegions);
		}
		else if (strcmp("objectgroup", type) == 0)
		{

		}
	}

	AArrayList->Release(tileRegions);
}

static inline void Parse(TiledMap* map, const char* filePath)
{
	JsonValue*   value = AJson->ParseWithFile(filePath);
	JsonObject*  root  = value->object;

	ParseMapInfo(map, root);
	ParseTilesets(map, root, filePath);
	ParseLayer(map, root);

	AJson->Release(value);

	map->drawable->width  = AGLToolToGLWidth (map->drawable->width);
	map->drawable->height = AGLToolToGLHeight(map->drawable->height);

    Mesh* tilesets = AArrayGetData(map->tilesetArr, Mesh);
    for (int i = 0; i < map->tilesetArr->length; i++)
    {
    	AArrayList->Shrink(tilesets[i].children);
    	AMesh->GenerateBuffer(&tilesets[i]);

    	Drawable* drawable = tilesets[i].drawable;
    	// use map matrix
    	ADrawableSetParent(drawable, map->drawable);

    	// use VBO
    	// drawable->useVBO(drawable);
    }
}

static void Release(TiledMap* map)
{
    for (int i = 0; i < map->tilesetArr->length; i++)
    {
    	AMesh->Release( AArrayGetPtr(map->tilesetArr, i, Mesh));
    }

    for (int i = 0; i < map->layerMap->arrayList->size; i++)
    {
    	free(AArrayStrMapGetAt(map->layerMap, i, Array*));
    }

    free(map->tilesetArr);
    map->tilesetArr = NULL;

	AArrayStrMap->Release(map->layerMap);
}


static void Render(Drawable* drawable)
{
	TiledMap* tiledMap = StructFrom2(drawable, TiledMap);
    for (int i = 0; i < tiledMap->tilesetArr->length; i++)
    {
    	ADrawable->Draw(AArrayGet(tiledMap->tilesetArr, i, Mesh).drawable);
    }
}


static void InitTiledMap(TiledMap* map, const char* filePath)
{
	ADrawable->Init(map->drawable);
	map->drawable->Render = Render;
	Parse(map, filePath);
}

static TiledMap* Create(const char* filePath)
{
	TiledMap* map = (TiledMap*) malloc(sizeof(TiledMap));
	InitTiledMap(map, filePath);

	return map;
}

struct ATiledMap ATiledMap[1] =
{
	Create,
	Release,
	Render,
};
