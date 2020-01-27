#include <vilga/detail/backend.hpp>
#include <utility>
#include <new>
#include <type_traits>
#include <vilga_impl/backend.hpp>
#include <vilga/detail/data.hpp>

namespace vilga_detail {

// backend has to behave as singleton... but it's really
// tricky to ensure it's life - in theory vilga
// should be also used from static objects
// because it semantics should reflect std::cout.
// Lucky I've found that a trick behind that is
// so called "nifty/Schwarz counter".
static int nifty_counter;
static typename std::aligned_storage<sizeof(backend), alignof(backend)>::type backend_memory;
backend& backend_instance = reinterpret_cast<backend&>(backend_memory);

backend::backend() {
  impl_ = std::make_unique<backend::impl>();
}

backend::~backend() = default;

void backend::consume(data&& data) {
  impl_->consume(std::move(data));
}

vilga_backend_initializer::vilga_backend_initializer() noexcept {
  if (nifty_counter++ == 0) new (&backend_instance) backend();
}

vilga_backend_initializer::~vilga_backend_initializer() noexcept {
  if (--nifty_counter == 0) (&backend_instance)->~backend();
}

}  // namespace vilga_detail
