#include <check.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "map.h"

SSMap* ssmap;

void header_setup(void) {
    ssmap = (SSMap*)malloc(sizeof(SSMap));
    SSMapInit(ssmap);
}

void header_teardown(void) {
    SSMapFree(ssmap);
    free(ssmap);
}

START_TEST(test_header_map_create) {
    SSMap temp;
    ck_assert_msg(SSMapInit(&temp) != NULL, "Failed to initialize an SSMap");
    SSMapFree(&temp);
}
END_TEST

START_TEST(test_header_map_add_new) {
    ck_assert_msg(SSMapAdd(ssmap, "hello", "world"), "Failed to add a value to SSMap");
    ck_assert_msg(strcmp(SSMapGet(ssmap, "hello"), "world") == 0, "SSMap added value incorrectly");
    ck_assert_msg(ssmap->count == 1, "SSMap failed to increment count");
}
END_TEST

START_TEST(test_header_map_add_duplicate) {
    ck_assert(SSMapAdd(ssmap, "pig", "oink"));
    ck_assert(SSMapAdd(ssmap, "pig", "moo") == false);
}
END_TEST

START_TEST(test_header_map_set_new) {
    uint32_t count = ssmap->count;
    SSMapSet(ssmap, "Alexander", "Hamilton");
    ck_assert(count != ssmap->count);
    ck_assert(strcmp(SSMapGet(ssmap, "Alexander"), "Hamilton") == 0);
}
END_TEST

START_TEST(test_header_map_set_old) {
    SSMapAdd(ssmap, "cow", "moo");
    uint32_t count = ssmap->count;
    SSMapSet(ssmap, "cow", "mooo");
    ck_assert(count == ssmap->count);
    ck_assert(strcmp(SSMapGet(ssmap, "cow"), "mooo") == 0);
}
END_TEST

START_TEST(test_header_map_get_invalid) {
    ck_assert(SSMapGet(ssmap, "Aaron") == NULL);
}
END_TEST

START_TEST(test_header_map_remove_valid) {
    SSMapSet(ssmap, "chicken", "bock");
    uint32_t count = ssmap->count;
    ck_assert(SSMapRemove(ssmap, "chicken"));
    ck_assert(ssmap->count == count - 1);
}
END_TEST

START_TEST(test_header_map_remove_invalid) {
    ck_assert(SSMapRemove(ssmap, "rooster") == false);
}
END_TEST

START_TEST(test_header_map_resize) {
    SSMap temp;
    ck_assert(SSMapInit(&temp));
    SSMapAdd(&temp, "a", "a");
    SSMapAdd(&temp, "b", "b");
    SSMapAdd(&temp, "c", "c");
    SSMapAdd(&temp, "d", "d");
    ck_assert(temp.loadFactor == 4);
    ck_assert(temp.capacity == 8);
    SSMapAdd(&temp, "e", "e");
    ck_assert(temp.loadFactor == 8);
    ck_assert(temp.capacity == 16);
    ck_assert(SSMapGet(&temp, "a") != NULL);
    ck_assert(SSMapGet(&temp, "b") != NULL);
    SSMapFree(&temp);
}
END_TEST

Suite* MapSuite(void) {
    Suite* s;
    TCase* tc_header_map;

    s = suite_create("Generic Maps");
    tc_header_map = tcase_create("Header Map");

    tcase_add_checked_fixture(tc_header_map, header_setup, header_teardown);
    tcase_add_test(tc_header_map, test_header_map_create);
    tcase_add_test(tc_header_map, test_header_map_add_new);
    tcase_add_test(tc_header_map, test_header_map_add_duplicate);
    tcase_add_test(tc_header_map, test_header_map_set_new);
    tcase_add_test(tc_header_map, test_header_map_set_old);
    tcase_add_test(tc_header_map, test_header_map_get_invalid);
    tcase_add_test(tc_header_map, test_header_map_remove_valid);
    tcase_add_test(tc_header_map, test_header_map_remove_invalid);
    tcase_add_test(tc_header_map, test_header_map_resize);
    

    suite_add_tcase(s, tc_header_map);

    return s;
}

int main(void) {
    int number_failed;
    Suite* s;
    SRunner* sr;

    s = MapSuite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}