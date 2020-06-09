#pragma once

namespace Rare {
#define RARE_INTERNAL(x) "resources/" x
#define RARE_INTERNAL_SHADER(x) RARE_INTERNAL("shaders/") x
#define RARE_INTERNAL_TEXTURE(x) RARE_INTERNAL("textures/") x
	class FileLoaderFactory {

	public:
		FileLoaderFactory() = delete;

		//rgba default composition
		static unsigned char* loadImage(const char name[], int* width, int* height, int* numOfColChannels, int req_comp = 4/*STBI_rgb_alpha = 4*/);
		static void free(void* data);

	};

}