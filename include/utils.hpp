#ifndef __UTILS_H__
#define __UTILS_H__

namespace ghr
{

// Copy bytes from src to dest until count or till buffer is full.
std::size_t copyBytes(
    const uint8_t *src, uint8_t *dest, std::size_t count, const std::size_t bufferSize)
{
    if(count >= bufferSize)
    {
        count = bufferSize;
    }
    std::memcpy(reinterpret_cast<void*>(dest),reinterpret_cast<const void*>(src),count);
    return count;
}

template <typename T>
void reverse(std::size_t count, T *data)
{
    std::size_t i = 0, ii = count - 1;
    uint8_t tmp;
    while (i < ii)
    {
        tmp = data[i];
        data[i] = data[ii];
        data[ii] = tmp;
        i++;
        ii--;
    }
}

// trim from start (in place)
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return !std::isspace(ch);
            }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
                return !std::isspace(ch);
            })
                .base(),
            s.end());
}

// trim from both ends (in place)
static inline std::string trim(std::string s)
{
    ltrim(s);
    rtrim(s);
    return s;
}

} // namespace ghr
#endif // __UTILS_H__