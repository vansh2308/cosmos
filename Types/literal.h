#ifndef COSMOS_LITERAL_H
#define COSMOS_LITERAL_H
#pragma once

#include <optional>
#include <string>
#include <variant>

namespace cosmos::Types{
    using Literal = std::variant<std::string, double>;
    using OptionalLiteral = std::optional<Literal>;

    auto get_literal_string(const Literal& value) -> std::string;
    auto make_optional_literal(double d_val) -> OptionalLiteral;
    auto make_optional_literal(const std::string& lexeme) -> OptionalLiteral;
}

#endif

