#pragma once

namespace kraken::utils {

/// A very simple strong type wrapper.
template <typename T, typename Tag>
struct StrongType {
    explicit StrongType(T value) : value_(value) {}

    StrongType(StrongType const&) = default;
    StrongType& operator=(StrongType const&) = default;

    explicit operator T() const {
        return value_;
    }

    auto operator<=>(StrongType<T, Tag> const&) const = default;

private:
    T value_;
};

} // namespace kraken::utils
