#pragma once
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <tools/Logging/Logger.h>

#include <shaderc/shaderc.hpp>
#define RARE_INTERNAL(x) "resources/" x
#define RARE_INTERNAL_SHADER(x) RARE_INTERNAL("shaders/") x

namespace Rare {
	namespace ShaderCompilation {
		enum RARE_SHADER_TYPE {
			VERTEX,
			FRAGMENT
		};
		
		static std::vector<uint32_t> compile(const std::string& srcName, shaderc_shader_kind kind, const std::vector<char>& src, bool optimize = false) {
			static shaderc::Compiler shader_compiler;
			static shaderc::CompileOptions shader_compiler_options;

			if (optimize) {
				shader_compiler_options.SetOptimizationLevel(shaderc_optimization_level_performance);
			}
			//shader_compiler_options.AddMacroDefinition("version", "450");
			//shader_compiler_options.SetGenerateDebugInfo();
			shader_compiler_options.SetOptimizationLevel(shaderc_optimization_level_zero);
			shaderc::SpvCompilationResult module = shader_compiler.CompileGlslToSpv(src.data(), kind, srcName.c_str(), shader_compiler_options);
			if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
				RARE_FATAL("Could not compile shader: {} : {}", srcName.c_str(), module.GetCompilationStatus());
				return std::vector<uint32_t>();
			}
			std::vector<uint32_t> result = { module.begin(), module.end() };
			result.resize(result.size() * (sizeof(uint32_t) / sizeof(char)), 0);
			return result;
		}

		static std::vector<uint32_t> ReadShaderSPV(const std::string& filename) {

			std::ifstream file(filename, std::ios::ate | std::ios::binary);
			if (!file.is_open()) {
				RARE_FATAL("Failed to open file: \t{}", filename);
			}
			size_t fileSize = (size_t)file.tellg();
			std::vector<uint32_t> buffer(fileSize);
			file.seekg(0);
			file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
			file.close();

			return buffer;
		}

		static std::vector<uint32_t> CompileShaderSource(const std::string& filename, RARE_SHADER_TYPE type) {

			std::ifstream file(filename, std::ios::ate);
			if (!file.is_open()) {
				RARE_FATAL("Failed to open file: \t{}", filename);
			}
			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);
			file.seekg(0);
			file.read(buffer.data(), fileSize);
			file.close();
			
			switch (type) {
				case VERTEX:
					return compile(filename, shaderc_glsl_vertex_shader, buffer);
				case FRAGMENT:
					return compile(filename, shaderc_glsl_fragment_shader, buffer);
			}
			return compile(filename, shaderc_glsl_infer_from_source, buffer);
		}

	}

}