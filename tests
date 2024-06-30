#include <stdio.h>
#include <stdlib.h>
#include "../main.c"

// Helper function to check if a file exists
static int file_exists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1; // File exists
    }
    return 0; // File does not exist
}

// Test function for creating an archive
static void test_create_archive(void **state)
{
    (void) state; // Unused parameter
    int result = create_archive("test.zip", "file1.txt file2.txt", 5);
    assert(result == 0); // Assert that archive creation was successful
    assert(file_exists("test.zip")); // Check that the archive file was created
}

// Test function for extracting files from an archive
static void test_extract_archive(void **state)
{
    (void) state; // Unused parameter
    int result = extract_archive("test.zip", "./output_directory");
    assert(result == 0); // Assert that extraction was successful
    assert(file_exists("./output_directory/file1.txt")); // Check that file1.txt was extracted
    assert(file_exists("./output_directory/file2.txt")); // Check that file2.txt was extracted
}

// Test function for listing files in an archive
static void test_list_archive(void **state)
{
    (void) state; // Unused parameter
    int result = list_archive("test.zip");
    assert(result == 0); // Assert that listing was successful
    // No specific output validation in this test, just checking operation success
}

// Test function for adding a file to an archive
static void test_add_to_archive(void **state)
{
    (void) state; // Unused parameter
    int result = add_to_archive("test.zip", "file3.txt");
    assert(result == 0); // Assert that adding file3.txt was successful
}

// Test function for deleting a file from an archive
static void test_delete_from_archive(void **state)
{
    (void) state; // Unused parameter
    int result = delete_from_archive("test.zip", "file1.txt");
    assert(result == 0); // Assert that deletion of file1.txt was successful
}

// Test function for creating an archive in verbose mode
static void test_verbose_mode(void **state)
{
    (void) state; // Unused parameter
    int result = create_archive_verbose("test.zip", "file1.txt file2.txt", 5);
    assert(result == 0); // Assert that verbose archive creation was successful
    // Additional checks related to verbose mode can be added here if needed
}

// Test function for creating an archive with nonexistent files
static void test_create_archive_nonexistent_files(void **state)
{
    (void) state; // Unused parameter
    int result = create_archive("nonexistent.zip", "nonexistent1.txt nonexistent2.txt", 5);
    assert(result != 0); // Expecting operation to fail due to nonexistent files
    // Cannot check if archive file was not created without sys/stat.h
}

// Test function for extracting an archive to a nonexistent directory
static void test_extract_archive_invalid_directory(void **state)
{
    (void) state; // Unused parameter
    int result = extract_archive("test.zip", "./invalid_directory");
    assert(result != 0); // Expecting operation to fail due to invalid directory
    // Cannot check extraction success without cmocka.h
}

int main(void)
{
    // Define array of unit tests
    const struct CMUnitTest tests[] =
            {
                    cmocka_unit_test(test_create_archive),
                    cmocka_unit_test(test_extract_archive),
                    cmocka_unit_test(test_list_archive),
                    cmocka_unit_test(test_add_to_archive),
                    cmocka_unit_test(test_delete_from_archive),
                    cmocka_unit_test(test_verbose_mode),
                    cmocka_unit_test(test_create_archive_nonexistent_files),
                    cmocka_unit_test(test_extract_archive_invalid_directory),
            };

    // Run the group of unit tests
    return cmocka_run_group_tests(tests, NULL, NULL);
}
