void LPF_1_db(float hz, float time, float in, float *out)
{
    *out += (1 / (1 + 1 / (hz * 6.28f * time))) * (in - *out);
}
