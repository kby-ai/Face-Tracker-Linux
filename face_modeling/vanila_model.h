#ifndef VANILA_MODEL_H
#define VANILA_MODEL_H

#include <net.h>
#include <string>

class FaceModelExtractor {
public:
    FaceModelExtractor(int input_width, int input_height, int num_thread = 4);

    void extract(float* data_ptr, float* out_ptr);

public:
    int         m_input_width;
    int         m_input_height;
    int         m_input_channel;
private:
    ncnn::Net                   m_model_net;
    std::vector<unsigned char>  m_model_bin;
    std::vector<unsigned char>  m_model_param;
};
#endif //VANILA_MODEL_H
