//
// Created by lwj on 2019/10/11.
//
#include <glog/logging.h>
#include "fec_encode.h"

int test() {
    FecEncode fec_encode(10, 5);
    int i;
    char arr[15][100] =
        {
            "origin 01", "origin 02", "origin 03", "origin 04", "origin 05",
            "origin 06", "origin 07", "origin 08", "origin 09", "origin 10",
            "origin 11", "origin 12", "origin 13", "origin 14", "origin 15",
        };
    printf("--------origin data start------------\n");
    for (i = 0; i < 15; i++) {
        printf("<%s>\n", arr[i]);
    }
    printf("--------origin data end------------\n");

    int ret = 0;
    for (int index = 0; index < 15; ++index) {
        ret = fec_encode.Input(arr[index], 9);
        if (ret == 1) {
            LOG(INFO) << "fec_encode input ret success index:" << index;
            break;
        }
    }
    std::vector<char *> res;
    std::vector<int32_t> res_length;
    ret = fec_encode.Output(res, res_length);
    if (ret != 0) {
        LOG(ERROR) << "failed to get fec_encoded data";
        return -1;
    }
//    rs_encode2(10, 15, data, 9);
    printf("++++++++encoded data start++++++++++++\n");
    for (i = 0; i < 15; i++) {
        ///加7是为了跳过fec所加的包头
        printf("<%s>\n", res[i] + 7);
    }
    printf("++++++++encoded data end++++++++++++\n");

    char *data[15];
    for (i = 0; i < 15; ++i) {
        data[i] = (char *) malloc((res_length[i] + 1) * sizeof(char));
        bzero(data[i], (res_length[i] + 1) * sizeof(char));
        memcpy(data[i], res[i], res_length[i]);
    }
    printf("@@@@@@@@trans data start@@@@@@@@@@@@\n");
    free(data[0]);
    data[0] = nullptr;
    free(data[1]);
    data[1] = nullptr;
    free(data[2]);
    data[2] = nullptr;
    free(data[10]);
    data[10] = nullptr;
    free(data[12]);
    data[12] = nullptr;
    for (i = 0; i < 15; i++) {
        if (data[i] == nullptr)
            continue;
        ///加7是为了跳过fec所加的包头
        printf("<%s>\n", data[i] + 7);
    }
    printf("@@@@@@@@trans data end@@@@@@@@@@@@@\n");

    ///注意这里的size必须是16,因为数据包大小是9而fec的包头大小是7
    ret = rs_decode2(10, 15, data, 16);
    printf("ret:%d\n", ret);

    printf("########decoder data start##########\n");
    for (i = 0; i < 15; i++) {
        if (data[i] == nullptr)
            continue;
        ///加7是为了跳过fec所加的包头
        printf("<%s>\n", data[i] + 7);
    }
    printf("########decoder data end##########\n");
    printf("########res data start##########\n");
    for (i = 0; i < 15; i++) {
        ///加7是为了跳过fec所加的包头
        printf("<%s>\n", res[i] + 7);
    }
    printf("########res data end##########\n");
    return 0;
}

int main(int argc, const char *argv[]) {
    google::InitGoogleLogging("INFO");
    FLAGS_logtostderr = true;
    test();
    return 0;
}
