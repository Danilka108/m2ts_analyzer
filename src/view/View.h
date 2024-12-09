#ifndef MPEGTS_ANALYZER_VIEW_H
#define MPEGTS_ANALYZER_VIEW_H

#include <cstddef>
#include <cstring>
#include <memory>
#include <type_traits>
#include <vector>

template <class View>
struct ViewTraits {
  static constexpr size_t MIN_SIZE = 0;

  static size_t size(const View *view) {
    static_assert(true, "ViewTraits::full_size must be specialized.");
    return 0;
  }
};

template <typename TargetView, typename SourceView,
          typename std::enable_if<!std::is_const<SourceView>::value ||
                                      std::is_const<SourceView>::value &&
                                          std::is_const<TargetView>::value,
                                  int>::type = 0>
TargetView *cast_view_to_view(SourceView *src_view) {
  if (ViewTraits<SourceView>::size(src_view) <
      ViewTraits<TargetView>::MIN_SIZE) {
    return nullptr;
  }

  TargetView *target_view = reinterpret_cast<TargetView *>(src_view);
  if (ViewTraits<TargetView>::size(target_view) >
      ViewTraits<SourceView>::size(src_view)) {
    return nullptr;
  }

  return target_view;
}

template <typename V>
V *copy_view(const V *src_view) {
  void *copy = ::operator new(ViewTraits<V>::size(src_view));
  std::memcpy(copy, (void *)src_view, ViewTraits<V>::size(src_view));
  return reinterpret_cast<V *>(copy);
}

template <typename V>
std::shared_ptr<typename std::remove_const<V>::type> make_shared_from_view(
    V *view) {
  return std::shared_ptr<V>(copy_view(view));
}

template <typename V>
const V *cast_buf_to_view(const uint8_t *buf, size_t buf_size) {
  if (buf_size < ViewTraits<V>::MIN_SIZE) {
    return nullptr;
  }

  const V *view = reinterpret_cast<const V *>(buf);
  if (buf_size < ViewTraits<V>::size(view)) {
    return nullptr;
  }

  return view;
}

template <typename View>
bool parse_views(std::vector<std::shared_ptr<View>> &dest, const uint8_t *buf,
                 size_t buf_size) {
  std::vector<std::shared_ptr<View>> views;

  const uint8_t *start = buf;
  const uint8_t *end = buf + buf_size;
  while (start < buf + buf_size) {
    const View *view = cast_buf_to_view<View>(start, end - start);
    if (view == nullptr) {
      return false;
    }

    views.push_back(std::shared_ptr<View>(copy_view<View>(view)));
    start += ViewTraits<View>::size(view);
  }

  if (start != end) {
    return false;
  }

  dest.insert(dest.end(), views.begin(), views.end());
  return true;
}

#endif
