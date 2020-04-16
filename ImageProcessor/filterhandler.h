#ifndef FILTERHANDLER_H
#define FILTERHANDLER_H
#include "ihandler.h"

// Общий базовый класс для всех фильтров
class FilterHandler : public IHandler
{
public:

    FilterHandler(int windowSize);

    virtual ~FilterHandler() override = default;

    virtual void setOption(optionFilters* opt) = 0;

protected:

    void loadWindow(vbyte_t::iterator src, int stride, vbyte_t& window);

    void loadWindow(vbyte_t::iterator src, vbyte_t::iterator window);

    // Размер окна для фильтрации
    int windowSize;
    int offset;
    vbyte_t src;

};


// Медианный фильтр
class FilterMedian : public FilterHandler
{
public:
    FilterMedian(int windowSize);

    virtual ~FilterMedian() override = default;

    virtual void handle(image_t &img) override;

    virtual const char* getName() const override
    {
        return "Median Filter";
    }

    virtual void setOption(optionFilters *opt) override;

private:
    void sort(vbyte_t& v);
};

// Билатеральный фильтр
class FilterBilateral : public FilterHandler
{
public:
    FilterBilateral(int windowSize, double sigmaI, double sigmaS);

    virtual ~FilterBilateral() override = default;

    virtual void handle(image_t &img) override;

    virtual const char* getName() const override
    {
        return "Bilateral Filter";
    }

    virtual void setOption(optionFilters *opt) override;
private:

    inline double f(double x, double sigma)
    {
        return exp(-(x)/(2 * sigma*sigma)) / (2 * M_PI * sigma*sigma);
    }

    inline double norm(int x1, int y1, int x2, int y2)
    {
        return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
    }

    void calcWindow(uint8_t* res, vbyte_t const& win, int x, int y);


    double Si, Ss;
};


class FilterConvolution : public FilterHandler
{
public:
    //  kernel - квадратное ядро свертки
    FilterConvolution(std::vector<double> kernel, double K = 1., uint8_t offs = 0);

    virtual void handle(image_t &img) override;

    virtual const char* getName() const override
    {
        return "Convolution Filter";
    }


    virtual void setOption(optionFilters *opt) override;
private:

    void applyKernel(uint8_t* resPix, vbyte_t & win);

    std::vector<double> kernel;

    uint8_t offset_kernel;
};

#endif // FILTERHANDLER_H
