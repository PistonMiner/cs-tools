#pragma once

#include <string>

void setupConsoleCodePage();
void resetConsoleCodePage();
std::string sjis2utf8(const std::string &in);