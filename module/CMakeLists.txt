add_library(boost_url_module)

target_sources(boost_url_module
        PUBLIC
        FILE_SET CXX_MODULES FILES
        grammar.cpp module.cpp
)

add_library(boost::module::url ALIAS boost_url_module)

target_link_libraries(boost_url_module PUBLIC Boost::url)