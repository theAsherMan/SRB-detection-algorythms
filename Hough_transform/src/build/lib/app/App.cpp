#include "App.hpp"

double culling_factor = 0.25;

int main(int argc, char*argv[])
{
    cout << endl << "===    PROCESSING FITS FILES   ===" << endl << endl;
    FITS::setVerboseMode(true);
    vector<fs::path>* fitsFilePaths = attainAllFITSFilePathsInDirectory(FITSSOURCEFOLDER);
    cout << fitsFilePaths->size() << " files found for processing" << endl;
    for(int ii=0; ii<fitsFilePaths->size(); ii++)
    {
        const auto entry = fitsFilePaths->at(ii);
        const auto result_path = deturmineResultsPathForSource(entry);
        ensurePathExists(result_path);
        searchForFrbInFile(entry, result_path);
    }
}

vector<fs::path>* attainAllFITSFilePathsInDirectory(string directoryPath)
{
    vector<fs::path>* fitsFiles = new vector<fs::path>();
    for (auto &entry : fs::recursive_directory_iterator(directoryPath))
    {
        if(isFitsFile(entry))
        {
            fitsFiles->push_back(entry.path());
        }
    }
    return fitsFiles;
}

bool isFitsFile(const fs::directory_entry& entry)
{
    return entry.is_regular_file() && entry.path().extension() == ".fits";
}

fs::path deturmineResultsPathForSource(fs::path source)
{
    auto relative_path = fs::relative(source, FITSSOURCEFOLDER);
    return FITSRESULTSFOLDER / relative_path.replace_extension(".cad");
}

void ensurePathExists(fs::path path)
{
    const auto parent_path = path.parent_path();
    if(!fs::exists(parent_path))
    {
        ensurePathExists(parent_path);
        fs::create_directory(parent_path);
    }
}

void searchForFrbInFile(fs::path source_path, fs::path result_path)
{
    auto raw_fits = FITS(source_path.string(), Read, true);
    logProcessingFitsFileStart(raw_fits.name());
    
    PHDU& fits = raw_fits.pHDU();
    
    cout << "::file dimentions::" << endl;
    int time_length = fits.axis(0);
    int freq_range = fits.axis(1);
    cout << "    time step count: " << time_length << endl;
    cout << "    frequency range: " << freq_range << endl;

    cout << "::file details extracted::" << endl;
    double min_freq = readKey(fits, "CRVAL2");
    double d_freq = readKey(fits, "CDELT2");
    double d_time = readKey(fits, "CDELT1");
    string unit_freq;   fits.readKey("CUNIT2", unit_freq);
    string unit_time;   fits.readKey("CUNIT1", unit_time);
    cout << "   lowest frequency: " << min_freq << endl;
    cout << "     frequency step: " << d_freq << endl;
    cout << "          time step: " << d_time << endl;
    cout << "    frequency units: " << unit_freq << endl;
    cout << "         time units: " << unit_time << endl;

    cout << "::file details calculated::" << endl;
    double max_freq = min_freq + d_freq * freq_range;
    double min_time = d_time;
    double max_time = min_time + d_time * time_length;
    cout << "  highest frequency: " << max_freq << endl;
    cout << "         start time: " << min_time << endl;
    cout << "           end time: " << max_time << endl;
    
    cout << "::DM range::" << endl;
    double min_disp = 0;
    double max_disp = 1100;
    cout << "          lowest DM: " << min_disp << endl;
    cout << "         highest DM: " << max_disp << endl;

    int dm_bucket_count = calculateDmBucketCount(d_time, min_disp, max_disp, min_freq, max_freq);

    valarray<double> flatData(time_length * freq_range);
    fits.read(flatData);
    cout << " == starting regridding == " << endl;
    auto stopwatch = StopWatch::start();
    
    auto regrid_scaling_factor = 3; // hard coded value in Jai Mings code.  may be subject to change should more accurate measure be found
    auto data = new VisualSpace(time_length, freq_range, &flatData);
    auto regridder = LinearToSquareRegrider(data, regrid_scaling_factor, min_freq, max_freq, true);
    regridder.data();
    long time_to_regrid = stopwatch.lap();
    cout << "               time: " << roundl(time_to_regrid / 1000000.0) << "ms" << endl;
    cout << " == regridding complete == " << endl;

    cout << "::regridded DM range::" << endl;
    double max_theta = regridder.dmToTheta(min_disp, freq_range, d_time);   // DMs map to negitive values
    double min_theta = regridder.dmToTheta(max_disp, freq_range, d_time);
    cout << "       lowest theta: " << min_theta << endl;
    cout << "      highest theta: " << max_theta << endl;
    cout << " theta bucket count: " << dm_bucket_count << endl;

    cout << " == start preprocessing == " << endl;
    stopwatch.lap();
    auto preproc = Preprocesor(regridder.data(), false);
    preproc.data();
    long time_to_preprocess = stopwatch.lap();
    cout << "               time: " << round(time_to_preprocess / 1000000.0) << "ms" << endl;
    cout << " == preprocessing complete == " << endl;

    cout << " == preforming transform == " << endl;
    stopwatch.lap();
    
    auto transformer = LinearHoughTransformer(min_theta, max_theta, dm_bucket_count, regridder.data());
    auto results = transformer.getPeaks(9, 0.174533, culling_factor, 5);

    long transform_time = stopwatch.lap();
    cout << "               time: " << round(transform_time / 1000000.0) << "ms" << endl;
    cout << " == transform complete == " << endl;
    cout << endl;
    cout << " --- results --- " << endl;
    for (auto line : results)
    {
        cout << "     +++     " << endl;
        cout << "     +++     " << endl;
        cout << "     theta: " << line.theta << endl;
        cout << "       rho: " << line.rho << endl;
        cout << "        DM: " << regridder.thetaToDm(line.theta, freq_range, d_time) << endl;
        cout << "start time: " << transformer.getXValueWhereLineIntersectsTopOfSpace(line) << endl;
        cout << " no. votes: " << line.brightness << endl;
    }
}

void logProcessingFitsFileStart(string fileName)
{
    string declearation = "  PROCESSING FILE  ";
    string namePrintoutLine = "filename: "+fileName;
    int lineLength = namePrintoutLine.length();
    lineLength = max(lineLength, int(declearation.length()));
    cout << endl << endl;
    for(int ii=0; ii<lineLength; ii++)
    {
        cout << "=";
    }
    cout << endl;
    cout << declearation << endl;
    cout << namePrintoutLine << endl;
    for(int ii=0; ii<lineLength; ii++)
    {
        cout << "=";
    }
    cout << endl;
    cout.flush();
}

double readKey(PHDU &fits, const string &key)
{
    string key_string;
    fits.readKey(key, key_string);
    return stod(key_string);
}

int calculateDmBucketCount(double d_time, double min_disp, double max_disp, double min_freq, double max_freq)
{
    const double d_dm = calculateD_dm(d_time, max_disp, min_freq, max_freq);
    const double dm_range = max_disp - min_disp;
    return int(ceil(dm_range / d_dm));
}

double calculateD_dm(double d_time, double max_disp, double min_freq, double max_freq)
{
    const double k_dm = 4.148*1000;
    const double const_factor = k_dm*(pow(min_freq, -2) - pow(max_freq, -2));
    const double max_time_delay = const_factor*max_disp;
    const double almost_max_time_delay = max_time_delay - d_time;
    const double almost_max_disp = almost_max_time_delay/const_factor;
    return max_disp - almost_max_disp;
}