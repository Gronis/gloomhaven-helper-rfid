
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