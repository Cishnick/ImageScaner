#include "filterhandler.h"

FilterHandler::FilterHandler(int windowSize) :
    windowSize(windowSize),
    offset(windowSize / 2)
{

}


void FilterHandler::loadWindow(vbyte_t::iterator src, int stride, vbyte_t &window)
{
    for(int i = 0; i < windowSize; i++)
    {
        loadWindow(src - stride*(offset - i), window.begin() + windowSize*i);
    }
}

void FilterHandler::loadWindow(vbyte_t::iterator src, vbyte_t::iterator window)
{
    src -= offset;
    for(int i = 0; i != windowSize; i++, window++, src++)
    {
        *window = *src;
    }
}

// =========================== FilterMedian ======================================

FilterMedian::FilterMedian(int windowSize) :FilterHandler (windowSize)
{
    offset = windowSize / 2;
}

void FilterMedian::handle(image_t &img)
{
    src = img.data;
    vbyte_t win(static_cast<size_t>(windowSize*windowSize));
    int f_width  = static_cast<int>(img.width) - 2*offset;
    int w = static_cast<int>(img.width);

    for(auto itLine = src.begin() + w*offset + offset,
        itData = img.data.begin() + w*offset + offset;
        itLine < src.end() - offset*w; itLine += w, itData += w - f_width)
    {
        for(auto itCol = itLine; itCol < itLine + f_width; itCol++, itData++)
        {
            loadWindow(itCol, w, win);
            sort(win);
            *itData = win[static_cast<size_t>(offset) + 1];
        }
    }
    return;
}

void FilterMedian::setOption(optionFilters *opt)
{
    auto temp = dynamic_cast<optionFilterMedian*>(opt);
    if(temp)
    {
        this->windowSize = temp->windowSize;
        this->offset = this->windowSize / 2;
    }
}

void FilterMedian::sort(vbyte_t &v)
{
    std::sort(v.begin(), v.end());
}

// ----------------------------- FilterBilateral ------------------------------

FilterBilateral::FilterBilateral(int windowSize, double sigmaI, double sigmaS) :
    FilterHandler(windowSize), Si(sigmaI), Ss(sigmaS)
{

}

void FilterBilateral::handle(image_t &img)
{
    src = img.data;
    int f_width  = static_cast<int>(img.width) - 2*offset;
    int w = static_cast<int>(img.width);

    vbyte_t win(static_cast<size_t>(windowSize*windowSize));
    int x = offset, y = offset;
    for(auto itLine = src.begin() + w*offset + offset,
        itData = img.data.begin() + w*offset + offset;
        itLine < src.end() - offset*w; itLine += w, itData += w - f_width, y++)
    {
        for(auto itCol = itLine; itCol < itLine + f_width; itCol++, itData++, x++)
        {
            loadWindow(itCol, w, win);
            calcWindow(itData.base(), win, x, y);
        }
        x = offset;
    }
}

void FilterBilateral::setOption(optionFilters *opt)
{
    auto temp = dynamic_cast<optionFilterBilateral*>(opt);
    if(temp)
    {
        this->windowSize = temp->windowSize;
        this->offset = this->windowSize / 2;
        this->Si = temp->sigmaI;
        this->Ss = temp->sigmaR;
    }
}

void FilterBilateral::calcWindow(uint8_t* res, const vbyte_t &win, int x, int y)
{
    double If = 0.;
    double Wp = 0.;
    int xk = 0, yk = 0;
    int i0 = win[static_cast<size_t>(offset + 1)];
    auto itW = win.begin();

    for(int i = 0; i < windowSize; i++)
    {
        for(int j = 0; j < windowSize; j++, itW++)
        {
            xk = x - (offset - j);
            yk = y - (offset - i);
            double gi = f((i0 - *itW)*(i0 - *itW), Si);
            double gs = f(norm(x, y, xk, yk), Ss);
            double w = gi*gs;
            If += (*itW) * w;
            Wp += w;
        }
    }
    If /= Wp;
    *res = static_cast<uint8_t>(If);
}
// ----------------------------- FilterConvolution ------------------------
FilterConvolution::FilterConvolution(std::vector<double> kernel, double K, uint8_t offs) :
    FilterHandler (static_cast<int>(sqrt(kernel.size()))),
    offset_kernel(offs)
{
    // Транспонируем ядро. Это нужно для упрощения перемножения матриц.
    for(int i = 0; i < windowSize; i++)
    {
        for(int j = 0; j < windowSize; j++)
        {
            auto t = kernel[i*windowSize + j];
            kernel[i*windowSize + j] = kernel[j*windowSize + i];
            kernel[j*windowSize + i] = t;
        }
    }
    // Нормируем
    for(auto &k : kernel)
    {
        k *= K;
    }
    this->kernel = kernel;
}

void FilterConvolution::handle(image_t &img)
{
    src = img.data;
    int f_width  = static_cast<int>(img.width) - 2*offset;
    int w = static_cast<int>(img.width);
    vbyte_t win(static_cast<size_t>(windowSize*windowSize));

    for(auto itLine = src.begin() + w*offset + offset,
        itData = img.data.begin() + w*offset + offset;
        itLine < src.end() - offset*w; itLine += w, itData += w - f_width)
    {
        for(auto itCol = itLine; itCol < itLine + f_width; itCol++, itData++)
        {
            loadWindow(itCol, w, win);
            applyKernel(itData.base(), win);
        }
    }
}

void FilterConvolution::setOption(optionFilters *opt)
{
    // TODO: параметры для свертки
    Q_UNUSED(opt);
    return;
}

void FilterConvolution::applyKernel(uint8_t* resPix, vbyte_t &win)
{
    double res = 0;

    auto itK = kernel.begin();
    auto itW = win.begin();
    for(;itK < kernel.end(); itK++, itW++)
    {
        res += (*itK)*(*itW);
    }
    if(res < 0)
        res = 0;
    if(res > 255)
        res = 255;
    *resPix = static_cast<uint8_t>(res) + offset_kernel;
}
