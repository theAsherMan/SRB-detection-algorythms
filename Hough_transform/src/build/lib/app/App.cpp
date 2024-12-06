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
        auto result_path = mirrorPath(FITSSOURCEFOLDER, FITSRESULTSFOLDER, entry);
        auto log_bin_path = mirrorPath(FITSSOURCEFOLDER, DEBUGOUTPUTFOLDER, entry).parent_path();

        result_path.replace_extension(".cad");
        ensurePathExists(result_path);
        ensurePathExists(log_bin_path);
        searchForFrbInFile(entry, result_path, log_bin_path);
    }
    cout << "---===+++===---" << endl;
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

fs::path mirrorPath(fs::path source, fs::path destination, fs::path absolutePath)
{
    auto relative_path = fs::relative(absolutePath, source);
    return destination / relative_path;
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

void searchForFrbInFile(fs::path source_path, fs::path result_path, fs::path log_bin_path)
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
    double min_disp = 100;
    double max_disp = 1100;
    cout << "          lowest DM: " << min_disp << endl;
    cout << "         highest DM: " << max_disp << endl;

    int dm_bucket_count = calculateDmBucketCount(d_time, min_disp, max_disp, min_freq, max_freq);

    valarray<double> flatData(time_length * freq_range);
    fits.read(flatData);

    VisualSpace* data = new VisualSpace(time_length, freq_range, &flatData);
    logFitsFile(log_bin_path, "before", data);
    auto stopwatch = StopWatch::start();

    cout << " == starting regridding == " << endl;

    
    auto regridder = LinearToSquareRegrider(data, regrid_scaling_factor, min_freq, max_freq, true);
    regridder.data();
    long time_to_regrid = stopwatch.lap();
    cout << "               time: " << roundl(time_to_regrid / 1000000.0) << "ms" << endl;
    cout << " == regridding complete == " << endl;
    logFitsFile(log_bin_path, "regriding", regridder.data());

    cout << "::regridded DM range::" << endl;
    double max_theta = regridder.dmToTheta(min_disp, freq_range, d_time);   // DMs map to negitive values
    double min_theta = regridder.dmToTheta(max_disp, freq_range, d_time);
    cout << "       lowest theta: " << min_theta << endl;
    cout << "      highest theta: " << max_theta << endl;
    cout << " theta bucket count: " << dm_bucket_count << endl;

    data = regridder.data();

    cout << " == start preprocessing == " << endl;
    stopwatch.lap();
    auto preproc = Preprocesor(data, false);
    preproc.data();
    long time_to_preprocess = stopwatch.lap();
    cout << "               time: " << round(time_to_preprocess / 1000000.0) << "ms" << endl;
    cout << " == preprocessing complete == " << endl;
    logFitsFile(log_bin_path, "preprocessing", preproc.data());

    data = preproc.data();
    cout << " == preforming transform == " << endl;
    stopwatch.lap();
    auto transformer = LinearHoughTransformer(min_theta, max_theta, dm_bucket_count, data);
    auto results = transformer.getPeaks(9, 0.174533, culling_factor, 5);

    long transform_time = stopwatch.lap();
    cout << "               time: " << round(transform_time / 1000000.0) << "ms" << endl;
    cout << "        theta range: " << transformer.getThetas()[0].theta() << " to " << transformer.getThetas()[transformer.getThetas().size()-1].theta() << endl;
    cout << " == transform complete == " << endl;

    cout << endl;
    auto _ = VisualSpace(data->getWidth(), data->getHieght());
    auto drawing = FrbDrawer(&_, min_freq, max_freq, d_time);
    cout << " --- results --- " << endl;
    for (auto line : results)
    {
        double theta = line.theta;
        double rho = line.rho;
        double dm = regridder.thetaToDm(line.theta, freq_range, d_time);
        double start_time = transformer.getXValueWhereLineIntersectsTopOfSpace(line);
        double intensity = line.brightness;
        cout << "     +++     " << endl;
        cout << "     +++     " << endl;
        cout << "     theta: " << theta << endl;
        cout << "       rho: " << rho << endl;
        cout << "        DM: " << dm << endl;
        cout << "start time: " << start_time << endl;
        cout << " no. votes: " << intensity << endl;
        drawing.draw(dm, start_time, intensity);
    }
    FitsWriter(log_bin_path/"found.fits").writeData(drawing.data());
    printExpectedResults(source_path.parent_path(), log_bin_path, data->getWidth(), data->getHieght(), min_freq, max_freq, d_time);
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

void logFitsFile(fs::path log_bin_path, string filename, VisualSpace* data)
{
    
    fs::path filePath = (log_bin_path/filename).replace_extension(".fits");
    FitsWriter(filePath).writeData(data);
}

void printExpectedResults(fs::path directoryPath, fs::path log_bin_path, int width, int height, double min_freq, double max_freq, double time_step)
{
    auto path = directoryPath/"lable.csv";
    if(fs::exists(path))
    {
        cout << "correct frbs" << endl;
        auto _ = VisualSpace(width, height);
        auto drawing = FrbDrawer(&_, min_freq, max_freq, time_step);
        auto headers = Csv::getHeaders(path);
        Csv answers(path);
        for(auto frb : answers)
        {
            cout << "---------------------------------" << endl;
            cout << "        DM: " << stod(frb["DM"]) << endl;
            cout << "start time: " << stoi(frb["StartTimeIndex"]) << endl;
            drawing.draw(stod(frb["DM"]), stoi(frb["StartTimeIndex"]), 1);
        }
        cout << "---------------------------------" << endl;
        FitsWriter(log_bin_path/"correct.fits").writeData(drawing.data());
    }else{
        cout<<"file has no lable.  cannot deturmine accuracy of measurement"<<endl;
    }
}