#ifndef XBELMARK_ENUMERATION_NAME_H
#define XBELMARK_ENUMERATION_NAME_H

#include <string>

namespace xbelmark {
namespace enumeration {

/**
 *  @brief Name of enumerator as declared in the enumeration.
 *
 *  @tparam T
 *    Enumeration type.
 */
template <typename T>
std::string EnumNameOf(T enumerator);

/**
 *  @brief Enumerator from its name as declared in the enumeration.
 *
 *  @tparam T
 *    Enumeration type.
 */
template <typename T>
T EnumValueOf(const std::string &name);

} // namespace enumeration
} // namespace xbelmark

#endif
