//
// Created by Matias on 13/09/2019.
//

#include "for_compressor.h"

namespace {
    unsigned int count_bits( uint32_t n) {
        unsigned int count = 0;
        while (n) {
            n = n >> 1;
            count++;
        }
        return count;
    }

    unsigned int find_bits_to_represent_n(std::list<uint32_t> &nums) {
        unsigned int max = *std::max_element(std::begin(nums), std::end(nums));
        return count_bits(max);
    }

    int sub_smallest(std::list<uint32_t> &nums) {
        int min = *std::min_element(std::begin(nums), std::end(nums));
        std::for_each(nums.begin(), nums.end(), [min](uint32_t &n){n -= min;});
        return min;
    }

    void fill_int_list(std::list<uint32_t> &list, std::vector<char> &source){
        unsigned int i;
        printf("swaperino");
        for (i = 0; i < source.size(); i += BYTES_PER_NUMBER) {
            //int* ptr = (int*)(source + i);
            //int v = (int)ntohl(*ptr);
            int v = (int)(source[i + 3]);
            list.push_back(v);
        }
    }

    std::vector<unsigned char> pack(std::list<uint32_t> &nums, size_t &bit_size){
        const int size = ceil(bit_size * nums.size() / BITS_IN_BYTE);
        std::vector<unsigned char> bytes_v(size, 0);
        unsigned int free_bits = BITS_IN_BYTE;
        int j = 0;

        std::for_each(nums.begin(), nums.end(), [&](uint32_t &n){
                          unsigned int bits_to_mov = bit_size;
                          auto nc = (unsigned char)n;
                          while (bits_to_mov > 0) {
                              unsigned int bits_movd;
                              bits_movd = std::min(free_bits, bits_to_mov);
                              nc = (nc << (free_bits - bits_movd));
                              bytes_v[j] = bytes_v[j] | nc;

                              free_bits -= bits_movd;
                              bits_to_mov -= bits_movd;
                              if (free_bits == 0) {
                                  free_bits = BITS_IN_BYTE;
                                  j++;
                              }
                          }
                      }
        );
        return bytes_v;
    }

}

void FoRCompressor::compress(CompressResult &r, std::vector<char> &to_compress,\
                            size_t block_size){
    //if (to_compress->length() < (block_size * BYTES_PER_NUMBER)) {
        //fix_size(to_compress, block_size);//fill with zeroes
    //}
    std::list<uint32_t> nums;
    fill_int_list(nums, to_compress);
    uint32_t reference = sub_smallest(nums);
    size_t bit_size = find_bits_to_represent_n(nums);
    std::vector<unsigned char> packed_bytes = pack(nums, bit_size);
    r.set(reference, bit_size, packed_bytes);
}