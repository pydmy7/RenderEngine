#pragma once

#include <vector>
#include <bit>

template<class T, class Cmp = std::less<T>>
struct RMQ {
    using u64 = std::uint64_t;
    static constexpr unsigned B = 64;

    const Cmp cmp = Cmp();
    int n{};
    std::vector<std::vector<T>> a;
    std::vector<T> pre, suf, ini;
    std::vector<u64> stk;

    RMQ() = default;
    explicit RMQ(const std::vector<T>& v) {
        init(v);
    }

    void init(const std::vector<T>& v) {
        n = static_cast<int>(v.size());
        pre = suf = ini = v;
        stk.resize(n);

        if (n == 0) {
            return;
        }

        const int M = (n - 1) / B + 1;
        const int lg = std::bit_width(static_cast<unsigned>(M)) - 1;
        a.assign(lg + 1, std::vector<T>(M));

        for (int i = 0; i < M; ++i) {
            a[0][i] = v[i * B];
            for (int j = 1; j < B && i * B + j < n; ++j) {
                a[0][i] = std::min(a[0][i], v[i * B + j], cmp);
            }
        }

        for (int i = 1; i < n; ++i) {
            if (i % B) {
                pre[i] = std::min(pre[i], pre[i - 1], cmp);
            }
        }

        for (int i = n - 2; i >= 0; --i) {
            if (i % B != B - 1) {
                suf[i] = std::min(suf[i], suf[i + 1], cmp);
            }
        }

        for (int j = 0; j < lg; ++j) {
            for (int i = 0; i + (2 << j) <= M; ++i) {
                a[j + 1][i] = std::min(a[j][i], a[j][i + (1 << j)], cmp);
            }
        }

        for (int i = 0; i < M; ++i) {
            const int l = i * B;
            const int r = std::min(static_cast<unsigned>(n), l + B);
            u64 s = 0;
            for (int j = l; j < static_cast<int>(r); ++j) {
                while (s && cmp(v[j], v[std::bit_width(s) - 1 + l])) {
                    s ^= static_cast<u64>(1) << (std::bit_width(s) - 1);
                }
                s |= static_cast<u64>(1) << (j - l);
                stk[j] = s;
            }
        }
    }

    T operator()(int l, int r) const {
        if (l / B != (r - 1) / B) {
            T ans = std::min(suf[l], pre[r - 1], cmp);
            int lb = l / B + 1;
            int rb = r / B;
            if (lb < rb) {
                int k = std::bit_width(static_cast<unsigned>(rb - lb)) - 1;
                ans = std::min({ans, a[k][lb], a[k][rb - (1 << k)]}, cmp);
            }
            return ans;
        } else {
            int x = B * (l / B);
            u64 mask = stk[r - 1] >> (l - x);
            return ini[std::countr_zero(mask) + l];
        }
    }
};
