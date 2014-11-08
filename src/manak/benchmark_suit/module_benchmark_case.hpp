#define MANAK_BENCHMARK_CASE_ITD(Name, Library, Function, Iter, Tolerance, Desc)     \
( new manak::BenchmarkCase(#Name,                                             \
                           #Library,                                          \
                           Function,                                          \
                           Iter,                                              \
                           Tolerance,                                         \
                           Desc) )

#define MANAK_BENCHMARK_CASE(Name, Library, Function)                         \
MANAK_BENCHMARK_CASE_ITD(Name, Library, Function, MANAK_DEFAULT_ITERATIONS, MANAK_DEFAULT_TOLERANCE, "")

#define MANAK_BENCHMARK_CASE_T(Name, Library, Function, Tolerance)            \
MANAK_BENCHMARK_CASE_ITD(NAme, Library, Function, MANAK_DEFAULT_ITERATIONS, Tolerance, "")

#define MANAK_BENCHMARK_CASE_I(Name, Function, Iter)                          \
MANAK_BENCHMARK_CASE_ITD(Name, Function, Iter, MANAK_DEFAULT_TOLERANCE, "")




#define MANAK_AUTO_BENCHMARK_CASE_ITD(Name, Library, Iter, Tolerance, Desc)   \
struct Name ## _ ## Library ##_## Benchmark                                   \
{                                                                             \
  static void Run();                                                          \
  static manak::BenchmarkCase* static_temp;                                   \
};                                                                            \
manak::BenchmarkCase* Name ## _ ## Library ##_## Benchmark::static_temp =     \
manak::BenchmarkSuite::GetMasterSuite()->GetCurrentSuite()->                  \
AddCase(MANAK_BENCHMARK_CASE_ITD(Name,                                        \
                                 Library,                                     \
                                 Name ## _ ## Library ##_## Benchmark::Run,   \
                                 Iter,                                        \
                                 Tolerance,                                   \
                                 Desc));                                      \
void Name ## _ ## Library ##_## Benchmark::Run()

#define MANAK_AUTO_BENCHMARK_CASE(Name, Library)                              \
MANAK_AUTO_BENCHMARK_CASE_ITD(Name, Library, MANAK_DEFAULT_ITERATIONS, MANAK_DEFAULT_TOLERANCE, "")

#define MANAK_AUTO_BENCHMARK_CASE_T(Name, Library, tol)                       \
MANAK_AUTO_BENCHMARK_CASE_T(Name, Library, MANAK_DEFAULT_ITERATIONS, tol, "")

#define MANAK_AUTO_BENCHMARK_CASE_I(Name, Library, Iter)                      \
MANAK_AUTO_BENCHMARK_CASE_ITD(Name, Library, Iter, MANAK_DEFAULT_TOLERANCE, "")




#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_ITD(Name, Library, f_name, Iter, tol, Desc) \
( manak::CTBenchmarkObject(#Name, #Library, Iter, tol, Desc, f_name) )

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(Name, Library, f_name)                   \
( manak::CTBenchmarkObject(#Name, #Library, MANAK_DEFAULT_ITERATIONS, MANAK_DEFAULT_TOLERANCE, "", f_name) )
