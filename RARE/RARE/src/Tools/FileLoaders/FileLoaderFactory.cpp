#include "FileLoaderFactory.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
namespace Rare {

	unsigned char* FileLoaderFactory::loadImage(const char name[], int* width, int* height, int* numOfColChannels, int req_comp) {
		stbi_set_flip_vertically_on_load(true);
		return stbi_load(name, width, height, numOfColChannels, req_comp);
	}

	void FileLoaderFactory::free(void* data) { stbi_image_free(data); }
}