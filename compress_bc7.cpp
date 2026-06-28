#define BASISD_SUPPORT_KTX2  0
#define BASISD_SUPPORT_KTX2_ZSTD 0
#define BASISD_SUPPORT_DXT1 0
#define BASISD_SUPPORT_DXT5A 0
#define BASISD_SUPPORT_PVRTC1 0
#define BASISD_SUPPORT_ETC2_EAC_A8 0
#define BASISD_SUPPORT_ASTC 0
#define BASISD_SUPPORT_ATC 0
#define BASISD_SUPPORT_ETC2_EAC_RG11 0
#define BASISD_SUPPORT_FXT1 0
#define BASISD_SUPPORT_PVRTC2 0
#define BASISD_SUPPORT_ETC2_EAC_RG110 0

#include "transcoder/basisu_transcoder.cpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <jni.h>

extern "C" {

	JNIEXPORT void JNICALL Java_com_github_knokko_compressor_Bc7Compressor_initNative(
		JNIEnv* env, jclass thisClass
	) {
		basist::bc7f::init();
		basist::uastc_init();
	}

	JNIEXPORT void JNICALL Java_com_github_knokko_compressor_Bc7Compressor_compressNative(
		JNIEnv* env, jclass thisClass, jint bc7fFlags,
		jlong sourceAddress, jint width, jint height, jlong destinationAddress
	) {
		basist::color_rgba *allPixels = (basist::color_rgba*) sourceAddress;
		uint8_t *allBlockData = (uint8_t*) destinationAddress;

		basist::color_rgba localPixels[16];
		uint8_t localBlockData[16];

		int numBlocksX = width / 4;
		if (4 * numBlocksX != width) numBlocksX += 1;
		int numBlocksY = height / 4;
		if (4 * numBlocksY != height) numBlocksY += 1;

		for (int blockY = 0; blockY < numBlocksY; blockY++) {
			int startY = 4 * blockY;
			for (int blockX = 0; blockX < numBlocksX; blockX++) {
				int startX = 4 * blockX;

				// Copy next pixel block into localPixels
				for (int localY = 0; localY < 4; localY++) {
					int globalY = std::min(height - 1, startY + localY);
					for (int localX = 0; localX < 4; localX++) {
						int globalX = std::min(startX + localX, width - 1);
						localPixels[localX + 4 * localY] = allPixels[globalX + width * globalY];
					}
				}

				// Compress localPixels into localBlockData
				basist::bc7f::fast_pack_bc7_auto_rgba(localBlockData, localPixels, bc7fFlags);

				// Copy localBlockData into the next section of allBlockData
				int blockIndex = blockX + numBlocksX * blockY;
				int blockDataIndex = 16 * blockIndex;
				memcpy(&allBlockData[blockDataIndex], localBlockData, 16);
			}
		}
	}
}
