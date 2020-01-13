#include <vilga/detail/backend.hpp>
#include <utility>
#include <vilga_impl/backend.hpp>
#include <vilga/detail/data.hpp>

namespace vilga_detail {

backend::backend() {
  // todo: swap with factory to ensure global object
  impl_ = std::make_unique<backend::impl>();
}

backend::~backend() = default;

backend::backend(backend&& other) noexcept : impl_(std::move(other.impl_)) { }

void backend::operator=(backend&& other) noexcept {
  impl_ = std::move(other.impl_);
}

void backend::consume(data&& data) {
  impl_->consume(std::move(data));
}

}  // namespace vilga_detail
