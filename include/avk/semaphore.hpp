#pragma once
#include <avk/avk.hpp>

namespace avk
{
	// Forward declaration:
	class queue;

	/** A synchronization object which allows GPU->GPU synchronization */
	class semaphore_t
	{
		friend class root;
		
	public:
		semaphore_t();
		semaphore_t(semaphore_t&&) noexcept = default;
		semaphore_t(const semaphore_t&) = delete;
		semaphore_t& operator=(semaphore_t&&) noexcept = default;
		semaphore_t& operator=(const semaphore_t&) = delete;
		~semaphore_t();

		/** Set a custom deleter function.
		 *	This is often used for resource cleanup, e.g. a buffer which can be deleted when this semaphore is destroyed.
		 */
		template <typename F>
		semaphore_t& set_custom_deleter(F&& aDeleter) noexcept
		{
			if (mCustomDeleter.has_value()) {
				// There is already a custom deleter! Make sure that this stays alive as well.
				mCustomDeleter = [
					existingDeleter = std::move(mCustomDeleter.value()),
					additionalDeleter = std::forward<F>(aDeleter)
				]() {
					additionalDeleter();
					existingDeleter();
				};
			}
			else {
				mCustomDeleter = std::forward<F>(aDeleter);
			}
			return *this;
		}

		semaphore_t& handle_lifetime_of(any_owning_resource_t aResource);

		const auto& create_info() const	{ return mCreateInfo; }
		auto& create_info()				{ return mCreateInfo; }
		const auto& handle() const { return mSemaphore.get(); }
		const auto* handle_addr() const { return &mSemaphore.get(); }

	private:
		// The semaphore config struct:
		vk::SemaphoreCreateInfo mCreateInfo;
		// The semaphore handle:
		vk::UniqueHandle<vk::Semaphore, DISPATCH_LOADER_CORE_TYPE> mSemaphore;

		/** A custom deleter function called upon destruction of this semaphore */
		std::optional<avk::unique_function<void()>> mCustomDeleter;

		std::vector<any_owning_resource_t> mLifetimeHandledResources;
	};

	// Typedef for a variable representing an owner of a semaphore
	using semaphore = avk::owning_resource<semaphore_t>;
}
