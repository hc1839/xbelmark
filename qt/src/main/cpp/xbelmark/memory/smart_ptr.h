#ifndef XBELMARK_MEMORY_SMART_PTR_H
#define XBELMARK_MEMORY_SMART_PTR_H

#include <functional>
#include <memory>

namespace xbelmark {
namespace memory {

/**
 *  Unique pointer with a user-defined deleter.
 */
template <typename T>
using UniquePtr = std::unique_ptr<T, std::function<void(T *)>>;

} // namespace memory
} // namespace xbelmark

#endif
