#pragma once

#include <type_traits>

struct OperandReduction {};



template <typename T>
struct EnableFeature : public std::false_type {};

template <> struct EnableFeature<OperandReduction> : public std::true_type {};
