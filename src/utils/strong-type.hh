#pragma once

namespace kraken::utils {

/// A very simple strong type wrapper.
template <typename T, typename Tag>
struct StrongType {
    explicit StrongType(T value) : value_(value) {}

    explicit operator T() const {
        return value_;
    }

private:
    T value_;
};

} // namespace kraken::utils