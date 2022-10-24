#pragma once
// since we can't use RTTI on the rp2040, time for some stinky workaround
using type_id_t = void(*)();

template<typename>
void type_id() {}