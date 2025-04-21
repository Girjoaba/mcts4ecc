#include "test_conditions.h"

#define PATH_duttaN22K16_numbers "tst/matrices/numbers/duttaN22K16.in"
#define PATH_duttaN39K32_numbers "tst/matrices/numbers/duttaN39K32.in"
#define PATH_duttaN72K64_numbers "tst/matrices/numbers/duttaN72K64.in"
#define PATH_hsiaoN22K16_numbers "tst/matrices/numbers/hsiaoN22K16.in"
#define PATH_liN22K16_numbers "tst/matrices/numbers/liN22K16.in"
#define PATH_li_big_numbers "tst/matrices/results/li_big_numbers.in"
#define PATH_not_distinctN6K3_numbers "tst/matrices/numbers/not_distinctN6K3.in"
#define PATH_zeroN6K3_numbers "tst/matrices/numbers/zeroN6K3.in"
#define PATH_3cycles_numbers "tst/matrices/numbers/3cycles.in"
#define PATH_myBigLi_numbers "tst/matrices/numbers/myLiN72K64.in"

uint8_t *N22K16, *N39K32, *N72K64, *hsiaoN22K16, *liN22K16, *liN72K64results, *myLiBig;
Config config22, config39, config72, config22hsiao;

void load_resource(const char *file_path, uint8_t **matrix, Config *config) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Could not open the file!\n");
        exit(EXIT_FAILURE);
    }
    int N, K;
    read_N_K(file, &N, &K);
    *matrix = read_number_matrix(file, N);
    config->N = N;
    fclose(file);
}

void setup_shared_resources() {
    load_resource(PATH_duttaN22K16_numbers, &N22K16, &config22);
    load_resource(PATH_duttaN39K32_numbers, &N39K32, &config39);
    load_resource(PATH_duttaN72K64_numbers, &N72K64, &config72);
    load_resource(PATH_hsiaoN22K16_numbers, &hsiaoN22K16, &config22hsiao);
    load_resource(PATH_liN22K16_numbers, &liN22K16, &config22);
    load_resource(PATH_li_big_numbers, &liN72K64results, &config72);
    load_resource(PATH_myBigLi_numbers, &myLiBig, &config72);
}

void teardown_shared_resources() {
    free(N22K16);
    free(N39K32);
    free(N72K64);
    free(hsiaoN22K16);
    free(liN22K16);
    free(liN72K64results);
    free(myLiBig);
}

void test_check_0_cols() {
    // Setup
    uint8_t *zeroMat;
    Config config0;
    load_resource(PATH_zeroN6K3_numbers, &zeroMat, &config0);
    // True Positive
    ASSERT_EQUAL(true, check_0_cols(&config0, zeroMat));
    // False Negative
    ASSERT_EQUAL(false, check_0_cols(&config22, N22K16));
    ASSERT_EQUAL(false, check_0_cols(&config22, liN22K16));
    ASSERT_EQUAL(false, check_0_cols(&config39, N39K32));
    ASSERT_EQUAL(false, check_0_cols(&config72, N72K64));
    ASSERT_EQUAL(false, check_0_cols(&config72, myLiBig));
    // Teardown
    free(zeroMat);
}

void test_check_no_2_cycles() {
    // Setup
    uint8_t *not_distinctMat;
    Config configNot;
    load_resource(PATH_not_distinctN6K3_numbers, &not_distinctMat, &configNot);
    // True Postive
    ASSERT_EQUAL(true, check_no_2_cycles(&config22, N22K16));
    ASSERT_EQUAL(true, check_no_2_cycles(&config22, liN22K16));
    ASSERT_EQUAL(true, check_no_2_cycles(&config39, N39K32));
    ASSERT_EQUAL(true, check_no_2_cycles(&config72, N72K64));
    ASSERT_EQUAL(true, check_no_2_cycles(&config72, myLiBig));
    // False Negative
    ASSERT_EQUAL(false, check_no_2_cycles(&configNot, not_distinctMat));
    // Teardown
    free(not_distinctMat);
}

void test_check_no_3_cycles() {
    // Setup
    uint8_t *three_cyclesMat;
    Config config3;
    load_resource(PATH_3cycles_numbers, &three_cyclesMat, &config3);
    // True Postive
    ASSERT_EQUAL(true, check_no_3_cycles(&config22, N22K16));
    ASSERT_EQUAL(true, check_no_3_cycles(&config39, N39K32));
    ASSERT_EQUAL(true, check_no_3_cycles(&config72, N72K64));
    // False Negative
    ASSERT_EQUAL(false, check_no_3_cycles(&config3, three_cyclesMat));
    // Teardown
    free(three_cyclesMat);
}

void test_check_no_adj_single_syndromes() {
    // True Postive
    // ASSERT_EQUAL(true, check_no_adj_single_syndromes(&config22, N22K16));
    // ASSERT_EQUAL(true, check_no_adj_single_syndromes(&config22, liN22K16));
    // ASSERT_EQUAL(true, check_no_adj_single_syndromes(&config39, N39K32));
    // ASSERT_EQUAL(true, check_no_adj_single_syndromes(&config72, N72K64));
    // ASSERT_EQUAL(true, check_no_adj_single_syndromes(&config72, liN72K64results));
    ASSERT_EQUAL(true, check_no_adj_single_syndromes(&config72, myLiBig));
}

void test_check_forbidden_4_cycles() {
    // True Postive
    ASSERT_EQUAL(true, check_no_forbidden_4_cycles(&config22, N22K16));
    ASSERT_EQUAL(true, check_no_forbidden_4_cycles(&config22, liN22K16));
    ASSERT_EQUAL(true, check_no_forbidden_4_cycles(&config39, N39K32));
    ASSERT_EQUAL(true, check_no_forbidden_4_cycles(&config72, N72K64));
    ASSERT_EQUAL(true, check_no_forbidden_4_cycles(&config72, myLiBig));
    ASSERT_EQUAL(0, get_forbidden_4_cycles(&config22, N22K16));
    ASSERT_EQUAL(0, get_forbidden_4_cycles(&config22, liN22K16));
    ASSERT_EQUAL(0, get_forbidden_4_cycles(&config22, myLiBig));
    ASSERT_EQUAL(0, get_forbidden_4_cycles(&config39, N39K32));
    ASSERT_EQUAL(0, get_forbidden_4_cycles(&config72, N72K64));
    // False Negative
    ASSERT_EQUAL(false, check_no_forbidden_4_cycles(&config22hsiao, hsiaoN22K16));
    ASSERT_EQUAL(8, get_forbidden_4_cycles(&config22hsiao, hsiaoN22K16));
}

void test_get_bad_4_cycles() {
    ASSERT_EQUAL(118, get_bad_4_cycles(&config22, N22K16));
    ASSERT_EQUAL(379, get_bad_4_cycles(&config39, N39K32));
    ASSERT_EQUAL(1316, get_bad_4_cycles(&config72, N72K64));
    ASSERT_EQUAL(120, get_bad_4_cycles(&config22hsiao, hsiaoN22K16));
}

void test_get_total_4_cycles() {
    ASSERT_EQUAL(251, get_total_4_cycles(&config22, N22K16));
    ASSERT_EQUAL(1363, get_total_4_cycles(&config39, N39K32));
    ASSERT_EQUAL(8289, get_total_4_cycles(&config72, N72K64));
    ASSERT_EQUAL(252, get_total_4_cycles(&config22hsiao, hsiaoN22K16));
}

void test_package_conditions() {
    setup_shared_resources();

    RUN_TEST(test_check_0_cols);
    RUN_TEST(test_check_no_2_cycles);
    RUN_TEST(test_check_no_3_cycles);
    RUN_TEST(test_check_forbidden_4_cycles);
    RUN_TEST(test_check_no_adj_single_syndromes);
    RUN_TEST(test_get_bad_4_cycles);
    RUN_TEST(test_get_total_4_cycles);

    teardown_shared_resources();

    printf("All condition tests successfully passed!\n");
}