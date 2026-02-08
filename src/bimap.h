#include <algorithm>
#include <unordered_map>
#include <vector>

namespace BabaIsYou {

template <typename A, typename B>
class BiMap {
  public:
    void Add(const A& a, const B& b) {
        auto& va = m_AToB[a];
        if (std::find(va.begin(), va.end(), b) == va.end()) {
            va.push_back(b);
        }

        auto& vb = m_BToA[b];
        if (std::find(vb.begin(), vb.end(), a) == vb.end()) {
            vb.push_back(a);
        }
    }

    void Remove(const A& a, const B& b) {
        if (auto it = m_AToB.find(a); it != m_AToB.end()) {
            auto& va = it->second;
            va.erase(std::remove(va.begin(), va.end(), b), va.end());
            if (va.empty()) {
                m_AToB.erase(it);
            }
        }

        if (auto it = m_BToA.find(b); it != m_BToA.end()) {
            auto& vb = it->second;
            vb.erase(std::remove(vb.begin(), vb.end(), a), vb.end());
            if (vb.empty()) {
                m_BToA.erase(it);
            }
        }
    }

    const std::vector<B>& Get(const A& a) const {
        static const std::vector<B> empty;
        auto it = m_AToB.find(a);
        return it != m_AToB.end() ? it->second : empty;
    }

    const std::vector<A>& Get(const B& b) const {
        static const std::vector<A> empty;
        auto it = m_BToA.find(b);
        return it != m_BToA.end() ? it->second : empty;
    }

    void Clear() {
        m_AToB.clear();
        m_BToA.clear();
    }

  private:
    std::unordered_map<A, std::vector<B>> m_AToB;
    std::unordered_map<B, std::vector<A>> m_BToA;
};

} // namespace BabaIsYou
