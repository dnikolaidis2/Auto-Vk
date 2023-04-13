#pragma once

#include <avk/avk.hpp>

namespace avk
{
#if VK_HEADER_VERSION >= 231
	class micromap_t
	{
		friend class root;
	public:
		micromap_t() = default;
		micromap_t(micromap_t&&) noexcept = default;
		micromap_t(const micromap_t&) = delete;
		micromap_t& operator=(micromap_t&&) noexcept = default;
		micromap_t& operator=(const micromap_t&) = delete;
		~micromap_t() = default;
	private:
		vk::UniqueHandle<vk::MicromapEXT, DISPATCH_LOADER_EXT_TYPE> mMicromap;
		const root* mRoot = nullptr;
	};

	using opacity_micromap = avk::owning_resource<micromap_t>;
#endif
}
