/*
every file named '~.in' means, cmake wil touch this file
edit the file or at least copy the file 
to some other directory 
*/

#pragma once

#include <cstdint>
#include <string_view>

/*
project(CPPProjectCMake 
VERSION 
1.0.0 
LANGUAGES C CXX)

'CPPProjectCMake' be PROJECT_NAME
*/
static constexpr std::string_view project_name = "CPPProjectCMake";
static constexpr std::string_view project_version = "1.0.0";

/*
1.0.0 above
- 1 : major
- 0 : minor
- 0 : patch

*/
static constexpr std::int32_t project_version_major{1};
static constexpr std::int32_t project_version_minor{0};
static constexpr std::int32_t project_version_patch{0};
