#pragma once
#include <macros>


/// @note A basic implementation of atomic operations in the Bora SDK.
namespace bora::stl::atomic {
IMPORT_ATTR("bora::stl::atomic", "create")
extFunc u64 create(u64 initial_value);

IMPORT_ATTR("bora::stl::atomic", "load")
extFunc u64 load(u64 id);

IMPORT_ATTR("bora::stl::atomic", "store")
extFunc void store(u64 id, u64 value);

IMPORT_ATTR("bora::stl::atomic", "compare_exchange")
extFunc bool compare_exchange(u64 id, u64 expected, u64 desired);
}
