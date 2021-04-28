#include <iostream>

#include "fptree.h"

using namespace nvindex;

bool load_binary_data(double data[], int length, const std::string& file_path) {
  std::ifstream is(file_path.c_str(), std::ios::binary | std::ios::in);
  if (!is.is_open()) {
    return false;
  }
  is.read(reinterpret_cast<char*>(data), std::streamsize(length * sizeof(double)));
  is.close();
  return true;
}


int main(int argc, char **argv){
    init_pmem_pool();
    Index<double, double, 64>* btree;
    btree = new FP_tree::Btree<double, double, 64> ();

     // Parsing arguments
    int num_data = 0;
    int load_data = 0;
    float selection_ratio = 0.0f;
    std::string data_set = "/DATA/clx/longitudes-200M.bin.data";

    int c;
    while ((c = getopt(argc, argv, "n:w:l:s:")) != -1) {
        switch (c) {
        case 'n':
        num_data = atoi(optarg);
        break;
        case 'l':
        load_data = atoi(optarg);
        break;
        case 's':
        selection_ratio = atof(optarg);
        default:
        break;
        }
    }
    int selected = num_data * selection_ratio;

      // Reading data
    double *keys = new double[num_data];
    load_binary_data(keys, num_data, data_set);

    struct timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC,&t1);
    for(int i = 0; i < load_data; ++i) {
      btree->insert(keys[i], keys[i]); 
    }

    clock_gettime(CLOCK_MONOTONIC,&t2);
    double sec=t2.tv_sec - t1.tv_sec;
    double nsec=t2.tv_nsec - t1.tv_nsec;
    printf("load totaltime:%ld s %ld ns \n",t2.tv_sec - t1.tv_sec, t2.tv_nsec - t1.tv_nsec);
    printf("load_latency o: %lf us \n",((sec*1000000000+nsec)/load_data)/1000);
    printf("load_throughput: %lf Mops/s \n",(double)load_data/(sec+nsec/1000000000));

    clock_gettime(CLOCK_MONOTONIC,&t1);
    for(int i = load_data; i < num_data; ++i) {
       btree->insert(keys[i], keys[i]); 
    }

    clock_gettime(CLOCK_MONOTONIC,&t2);

    sec=t2.tv_sec - t1.tv_sec;
    nsec=t2.tv_nsec - t1.tv_nsec;
    printf("test totaltime:%ld s %ld ns \n",t2.tv_sec - t1.tv_sec, t2.tv_nsec - t1.tv_nsec);
    printf("test_latency o: %lf us \n",((sec*1000000000+nsec)/(num_data - load_data))/1000);
    printf("test_throughput: %lf Mops/s \n",(double)(num_data - load_data)/(sec+nsec/1000000000));

    delete[] keys;
    return 0;
}