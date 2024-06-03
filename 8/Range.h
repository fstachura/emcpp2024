#ifndef LAB6_RANGE_H
#define LAB6_RANGE_H

#include <cstddef>
#include <iterator>

template<typename T>
class Range;

template<typename T>
class RangeIterator {
    const Range<T> r;
    T state;

public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = T;
    using value_type = T;
    using pointer_type = T;
    using reference_type = T;

    RangeIterator(Range<T> r): r(r), state(r.start) {
    }

    RangeIterator(Range<T> r, T state): r(r), state(state) {
    }

    reference_type operator*() {
        return state;
    }

    pointer_type operator->() {
        return &state;
    }

    RangeIterator<T>& operator++() {
        if(state+r.step < r.stop) {
            state += r.step;
        } else {
            state = r.stop;
        }
        return *this;
    }

    RangeIterator<T> operator++(int) {
        auto tmp = *this;
        if(state+r.step < r.stop) {
            state += r.step;
        } else {
            state = r.stop;
        }
        return tmp;
    }

    friend bool operator==(const RangeIterator<T>& a, const RangeIterator<T>& b) {
        return a.r == b.r && (a.state == b.state || ((a.state+a.step) >= a.stop && (b.state+b.step) >= b.stop));
    }

    friend bool operator!=(const RangeIterator<T>& a, const RangeIterator<T>& b) {
        return a.state != b.state || !(a.r == b.r);
    }
};

template <typename T>
struct Range {
    T start, stop, step;

    Range(T stop): start(0), stop(stop), step(1) {
    }

    Range(T start, T stop): start(start), stop(stop), step(1) {
    }

    Range(T start, T stop, T step): start(start), stop(stop), step(step) {
    }

    RangeIterator<T> begin() {
        return RangeIterator<T>(*this);
    }

    RangeIterator<T> end() {
        return RangeIterator<T>(*this, stop);
    }

    friend bool operator==(const Range<T>& a, const Range<T>& b) {
        return a.start == b.start &&
            a.stop == b.stop &&
            a.step == b.step;
    }
};

template<typename... T>
auto make_range(T... t) {
    return Range(t...);
}

#endif //LAB6_RANGE_H
