#include <gflags/gflags.h>
#include <glog/logging.h>
#include <glog/stl_logging.h>
#include <iostream>

DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
DEFINE_string(languages, "english,french,german",
"comma-separated list of languages to offer in the 'lang' menu");

void fatal_function() {
    // print stack info
    LOG(FATAL) << "This is a fatal";
}

int main(int argc, char *argv[])
{
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);

    FLAGS_stderrthreshold = google::INFO;

    // Optional: parse command line flags
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::cout << "log_dir: " << FLAGS_log_dir << std::endl;
    std::cout << "big_menu: " << std::boolalpha << FLAGS_big_menu << std::noboolalpha
              << ", languages: "  << FLAGS_languages << std::endl;
    LOG(WARNING) << "big_menu: " << std::boolalpha << FLAGS_big_menu << std::noboolalpha
              << ", languages: "  << FLAGS_languages;

    // glog/stl_logging.h allows logging STL containers.
    std::vector<int> x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    LOG(INFO) << "ABC, it's easy as " << x;

    LOG(ERROR) << "This is a error";
    fatal_function();

    google::ShutdownGoogleLogging();
    return 0;
}
