#include "../utils/base.h"
//#include "base_func.h"
#include "vanila_model.h"
#include "vanl.id.h"

static int g_face_width = 60;
static int g_face_height = 60;
static AlignParam param_model = { 0, 0, 0, 0, 0, 0 };

FaceModelExtractor::FaceModelExtractor(int input_width, int input_height, int num_thread) {

    m_input_channel = 3;
    m_input_width = input_width;
    m_input_height = input_height;
    m_model_net.opt.use_vulkan_compute = true;
    int ret = m_model_net.load_param("./weights/face_modeling/landmark.param");
    if (ret != 0) {
        printf("\nFace Modeling load param failed. %d", ret);
        return;
    }

    ret = m_model_net.load_model("./weights/face_modeling/landmark.bin");
    if (ret != 0) {
        printf("\nFace Modeling model failed. %d", ret);
        return;
    }
    printf("\nFace Modeling init success.");
}

void FaceModelExtractor::extract(float* data_ptr, float* out_ptr) {

    ncnn::Extractor ex = m_model_net.create_extractor();
    ex.set_light_mode(true);
    ex.set_num_threads(4);

    ncnn::Mat in(g_face_width, g_face_height, 1, data_ptr);
    ex.input(vanl_param_id::BLOB_data, in);

    ncnn::Mat out;
    ex.extract(vanl_param_id::BLOB_Dense3, out);

    for (int i = 0; i < 136; i++)
        out_ptr[i] = out[i];
}