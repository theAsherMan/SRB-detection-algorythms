#pragma once

#include <CCfits/CCfits>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <chrono>

#include "../LinearTransformer/LinearHoughTransformer.hpp"
#include "../regriding/LinearToSquareRegrider.hpp"
#include "../visualSpace/VisualSpace.hpp"
#include "../stopwatch/StopWatch.hpp"
#include "../preprocesing/Preprocesor.hpp"

using namespace std;
using namespace CCfits;

namespace fs = filesystem;

int main(int argc, char *argv[]);
vector<fs::path> *attainAllFITSFilePathsInDirectory(string directoryPath);
double convertDmToTheta(double dm);
bool isFitsFile(const fs::directory_entry &file);
fs::path deturmineResultsPathForSource(fs::path source);
void ensurePathExists(fs::path path);
void searchForFrbInFile(fs::path source_path, fs::path result_path);
double readKey(PHDU &fits, const string &key);
int calculateDmBucketCount(double d_time, double min_disp, double max_disp, double min_freq, double max_freq);
double calculateD_dm(double d_time, double max_disp, double min_freq, double max_freq);
void logProcessingFitsFileStart(string fileName);

#define FITSSOURCEFOLDER "./fitsFiles"
#define FITSRESULTSFOLDER "./results"