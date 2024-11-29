#include "LinearHoughTransformer.hpp"

using namespace std;

LinearHoughTransformer::LinearHoughTransformer(double min_slope, double max_slope, int number_of_slope_buckets, VisualSpace* linearSpace)
{
    this->linearSpace = linearSpace;
    this->voteSpace = NULL;
    this->accumulatorSpace = NULL;
    this->min_slope = min_slope;
    this->max_slope = max_slope;
    
    this->thetas = vector<IndexThetaMaping>();
    double slope_range = max_slope - min_slope;
    this->delta_slope = slope_range/number_of_slope_buckets;
    for(int ii=0; ii<number_of_slope_buckets; ii++)
    {
        thetas.push_back(IndexThetaMaping(min_slope + ii * delta_slope, delta_slope, min_slope));
    }
    this->indexToTheta = new IndexThetaMappingFactory(delta_slope, min_slope);
}
LinearHoughTransformer::~LinearHoughTransformer()
{
    if(voteSpace != NULL)
    {
        delete(voteSpace);
    }
    if(accumulatorSpace != NULL)
    {
        delete(accumulatorSpace);
    }
    if(indexToTheta != NULL)
    {
        delete(indexToTheta);
    }
}

list<HoughLineDescriptor> LinearHoughTransformer::getPeaks(int min_distance, int min_angle, double culling_factor, int max_peaks_returned)
{
    if(voteSpace == NULL)
    {
        transform();
    }

    peaks = list<HoughLineDescriptor>();

    double threshold = calcThreshold(culling_factor);

    for(int ii=0; ii<voteSpace->getWidth(); ii++)
    {
        for(int jj=0; jj<voteSpace->getHieght(); jj++)
        {
            addPeakIfValid(ii, jj, threshold, min_distance, min_angle);
        }
    }
    peaks.sort([](HoughLineDescriptor l1, HoughLineDescriptor l2){return l1.brightness > l2.brightness;});
    if(peaks.size() > max_peaks_returned)
    {
        auto knife = peaks.begin();
        advance(knife, max_peaks_returned);
        peaks.erase(knife, peaks.end());
    }
    return peaks;
}

void LinearHoughTransformer::addPeakIfValid(int ii, int jj, double threshold, int min_distance, int min_angle)
{
    int votes = voteSpace->point(ii , jj)->getValue();

    if(votes >= threshold)
    {
        for(int distance = max(0, ii - min_distance); distance < min(voteSpace->getWidth(), ii + min_distance); distance++)
        {
            for(int angle = max(0, jj - min_angle); angle < min(voteSpace->getHieght(), jj + min_angle); angle++)
            {
                if(votes < voteSpace->point(distance, angle)->getValue())
                {
                    return;
                }
            }
        }
        peaks.push_back(HoughLineDescriptor(thetas.at(ii).theta(), jj, votes));
    }
}

vector<IndexThetaMaping> LinearHoughTransformer::getThetas()
{
    return thetas;
}
void LinearHoughTransformer::transform()
{
    setHoughSpace(thetas.size(), ceil(linearSpace->getImageRadius()));

    for(int x = 0; x < linearSpace->getWidth(); x++)
    {
        for(int y = 0; y < linearSpace->getHieght(); y++)
        {
            plotPointInHoughSpace(x,y);
        }
    }
}
void LinearHoughTransformer::setHoughSpace(int width, int hieght)
{
    if(voteSpace != NULL)
    {
        delete(voteSpace);
    }
    if(accumulatorSpace != NULL)
    {
        delete(accumulatorSpace);
    }
    voteSpace = new VisualSpace(width, hieght);
    accumulatorSpace = new VisualSpace(width, hieght);
}
void LinearHoughTransformer::plotPointInHoughSpace(int x, int y)
{
    VSPoint* point = linearSpace->point(x,y);
    if(point->getValue() > 0)
    {
        for(auto indexThetaPair : thetas)
        {
            VoteForLineOfSlopePassingThroughPoint(indexThetaPair, point);
        }
    }
}
double LinearHoughTransformer::calcThreshold(double culling_factor)
{
    int highest_votes = this->voteSpace->max();
    double non_zero_mean = voteSpace->meanOfPositives();
    int effective_range = highest_votes - non_zero_mean;
    return non_zero_mean + culling_factor * effective_range;
}
void LinearHoughTransformer::VoteForLineOfSlopePassingThroughPoint(IndexThetaMaping indexThetaPair, VSPoint* point)
{
    double theta = indexThetaPair.theta();
    double rho = point->getX()*cos(theta) + point->getY()*sin(theta);
    if(rho >= 0 && rho < voteSpace->getHieght() - 1)
    {
        addLineToHoughSpace(indexThetaPair, rho, point);
    }
}
void LinearHoughTransformer::addLineToHoughSpace(IndexThetaMaping indexThetaPair, double rho, VSPoint* point)
{
    int x = indexThetaPair.index();
    int y = int(round(rho));
    voteSpace->point(x, y)->increaseValueUnsafe(1);
    accumulatorSpace->point(x, y)->increaseValueUnsafe(point->getValue());
}
HoughLineDescriptor LinearHoughTransformer::sampleFromHoughSpace(IndexThetaMaping indexThetaPair, double rho)
{
    double theta = indexThetaPair.theta();
    int x = indexThetaPair.index();
    double brightness = voteSpace->point(x, rho)->getValue();
    return HoughLineDescriptor(theta, rho, brightness);
}
HoughLineDescriptor LinearHoughTransformer::sampleFromHoughSpace(int x, int y)
{
    double brightness = voteSpace->point(x, y)->getValue();
    double rho = voteSpace->point(x, y)->getY();
    double theta = indexToTheta->mapFromIndex(x).theta();
    return HoughLineDescriptor(theta, rho, brightness);
}
double LinearHoughTransformer::getXValueWhereLineIntersectsTopOfSpace(HoughLineDescriptor line)
{
    double rho = line.rho;
    double theta = line.theta;

    double y = this->linearSpace->getHieght()-1;
    double x = rho/cos(theta) - y*tan(theta);
    return x;
}