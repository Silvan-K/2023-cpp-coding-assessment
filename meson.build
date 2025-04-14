project('swe-cpp-assessment', 'cpp',
        version : '0.1',
        default_options : ['cpp_std=c++17',
                           'buildtype=release'])

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')

lib = shared_library('lib', ['src' / 'al_uint.cc',
                             'src' / 'prime_field_element.cc'])

lib_dep = declare_dependency(link_with: [lib])

test_al_uint = executable('test_al_uint', 'tests' / 'test_al_uint.cc',
                      dependencies : [lib_dep, gtest_dep, gtest_main_dep],
                      install : false)
test_prime_field_element = executable('test_prime_field_element', 'tests' / 'test_prime_field_element.cc',
                              dependencies : [lib_dep, gtest_dep, gtest_main_dep],
                              install : false)
performance_tests = executable('performance_tests', 'tests' / 'performance_tests.cc',
                               dependencies : [lib_dep, gtest_dep, gtest_main_dep],
                               install : false)

test('test_al_uint', test_al_uint)
test('test_prime_field_element', test_prime_field_element)
test('performance_tests', performance_tests, timeout: 0)
