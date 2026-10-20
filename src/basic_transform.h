#ifndef BASIC_TRANSFORM_H
#define BASIC_TRANSFORM_H

#include <cassert>

#include "vector_util.h"
#include <SFML/System/Vector2.hpp>

struct BasicTransform {
    // TODO: Determine if possible in general, else provide counterexample:
    // compose(f, g) is equivalent to first applying g, then applying f.
    static BasicTransform compose(const BasicTransform& f, const BasicTransform& g) {
        return {
            // it is possible, if g(x) = x * s1 + t1, then f(g(x)) = (x * s1 + t1) * s2 + t2
            // if you distribute the s2 multiplication, you get x * s1 * s2 + t1 * s2 + t2
            // so from the original formula, we can equate h(x) = x * s + t where s = (s1 * s2) and t = (t1 * s2 + t2)
            // to x * (s1 * s2) + (t1 * s2 + t2)
            g.scale * f.scale, g.translation * f.scale + f.translation
        };
    };

    // TODO: Determine if possible in general, else provide counterexample:
    static BasicTransform inverse(const BasicTransform& f) {
            // it is possible as long as all scale components are non-zero
            // if y = f(x) = x * s1 + t, then the formula for f-1(x) would have to first subtract the translation,
            // then divide by the scale. The formula for f-1(x) becomes f-1(y) = (y - t1) / s1
            // expand this into y / s1 - t1 /s1, and we have our equation for all non-zeroes
            // f-1(y) = y * (1/s1) - (t1/s1) so s-1 = (1/s1) and t-1 = -(t1/s1)
            sf::Vector2f inverseScale {1.f / f.scale.x, 1.f / f.scale.y};
        return {
            inverseScale, -f.translation / f.scale
        };
    };

    constexpr BasicTransform(const sf::Vector2f& scale = {1.f, 1.f},
                             const sf::Vector2f& translation = {0.f, 0.f})
        : scale{scale}, translation{translation} {}

    constexpr sf::Vector2f applyPoint(const sf::Vector2f& p) const {
        // first scale, then translate -- if you do the reverse, the translation gets messed up.
        return {p.x * scale.x + translation.x, p.y * scale.y + translation.y};
    }

    constexpr sf::Vector2f applyVector(const sf::Vector2f& p) const {
        // vectors are not translated by transformations
        return {p.x * scale.x, p.y * scale.y};
    }

    constexpr bool operator==(const BasicTransform& other) const {
        return scale == other.scale && translation == other.translation;
    }

    sf::Vector2f scale;
    sf::Vector2f translation;
};

#endif