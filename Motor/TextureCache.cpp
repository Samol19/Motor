#include "TextureCache.h"
#include "ImageLoader.h"
TextureCache::TextureCache()
{
}

TextureCache::~TextureCache()
{
}

GLTexture TextureCache::getTexture(string texturePath)
{
	//opcion 1
	//map<string, GLTexture>::iterator mit = textureMap.find(texturePath);

	//opcion 2
	auto mit=textureMap.find(texturePath);

	if (mit == textureMap.end()) {

		GLTexture texture = ImageLoader::loadPng(texturePath);

		textureMap[texturePath]=texture;
		//pair<string, GLTexture> newpair(texturePath, texture);
		//textureMap.insert(newpair);

		return texture;

	}

	return mit->second;
}
