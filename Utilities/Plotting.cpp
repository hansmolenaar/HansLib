#include "Plotting.h"
#include "MultiIndex.h"

#include "MultiIndex.h"
#include "Plotting.h"

#include <fstream>
#include <sstream>
#include <vector>

namespace
{
std::filesystem::path getOutputDir()
{
    return std::filesystem::path("/home/hans/Work");
}
} // namespace

std::string Plotting::PlotFunction(std::initializer_list<std::function<double(double)>> functions, double xmin,
                                   double xmax, int npoints)
{
    const std::vector<std::vector<double>> values = EvaluateFunctions(functions, xmin, xmax, npoints);
    std::ostringstream result;
    ToStream(result, values);
    return result.str();
}

std::vector<std::vector<double>> Plotting::EvaluateFunctions(
    std::initializer_list<std::function<double(double)>> functions, double xmin, double xmax, int npoints)
{
    std::vector<std::vector<double>> result;

    Utilities::MyAssert(npoints > 1);
    Utilities::MyAssert(xmax > xmin);

    const double delx = (xmax - xmin) / (npoints - 1.0);
    for (int n = 0; n < npoints; ++n)
    {
        std::vector<double> row;
        double x;
        if (n == 0)
        {
            x = xmin;
        }
        else if (n == npoints - 1)
        {
            x = xmax;
        }
        else
        {
            x = xmin + delx * n;
        }
        row.push_back(x);

        for (auto &fie : functions)
        {
            const double y = fie(x);
            row.push_back(y);
        }
        result.push_back(row);
    }

    return result;
}

void Plotting::ToStream(std::ostream &stream, const std::vector<std::vector<double>> &values)
{
    std::vector<std::string> headers{"x"};
    for (size_t n = 0; n < values.front().size() - 1; ++n)
    {
        headers.push_back("y" + std::to_string(n));
    }
    ToStream(stream, headers, values);
}

std::filesystem::path Plotting::GenerateFullFilePath(const std::string &name)
{
    std::filesystem::path path{getOutputDir()};
    const auto fileName = name + ".txt";
    path /= fileName;
    return path;
}

std::ofstream Plotting::GetFile(const std::string &name)
{
    const auto &fullPath = Plotting::GenerateFullFilePath(name);
    std::ofstream ofs(fullPath);
    if (!ofs.is_open())
        throw MyException(std::string("Unable to create file ") + fullPath.string());
    return ofs;
}

void Plotting::ToStream(std::ostream &stream, const std::vector<std::string> &headers,
                        const std::vector<std::vector<double>> &values)
{
    bool isFirst = true;
    for (const auto &header : headers)
    {
        if (!isFirst)
        {
            stream << " , ";
        }
        else
        {
            isFirst = false;
        }
        stream << header;
    }
    stream << "\n";

    const auto numColumns = headers.size();
    for (const auto &row : values)
    {
        Utilities::MyAssert(row.size() == numColumns);
        isFirst = true;
        for (double val : row)
        {
            if (!isFirst)
            {
                stream << " , ";
            }
            else
            {
                isFirst = false;
            }
            stream << val;
        }
        stream << "\n";
    }
}

std::vector<std::vector<double>> Plotting::EvaluateFunctionsOnUnit(
    size_t dim, std::initializer_list<std::function<double(std::vector<double>)>> functions, int npoints)
{
    Utilities::MyAssert(npoints > 1);
    Utilities::MyAssert(functions.size() > 0);
    std::vector<std::vector<double>> result;
    const auto mi = MultiIndex<size_t>::Create(std::vector<size_t>(dim, npoints));
    const double delx = 1.0 / (npoints - 1);
    std::vector<double> row(dim + functions.size());
    std::vector<size_t> indices(dim);
    for (size_t n = 0; n < mi.getFlatSize(); ++n)
    {
        mi.toMultiplet(n, indices);
        str::transform(indices, row.begin(), [&delx](size_t i) { return i * delx; });
        str::transform(functions, row.begin() + dim,
                       [&row, dim](const auto &ff) { return ff(std::vector<double>(row.begin(), row.begin() + dim)); });
        result.push_back(row);
    }

    return result;
}

void Plotting::MdFunctionsOnUnityToStream(std::ostream &stream, size_t dim,
                                          std::initializer_list<std::function<double(std::vector<double>)>> functions,
                                          int npoints, std::vector<std::string> headers)
{
    if (headers.empty())
    {
        for (size_t n = 0; n < dim; ++n)
            headers.push_back("x_" + std::to_string(n));
        for (size_t n = 0; n < functions.size(); ++n)
            headers.push_back("y_" + std::to_string(n));
    }
    Utilities::MyAssert(headers.size() == dim + functions.size());
    const auto values = Plotting::EvaluateFunctionsOnUnit(dim, functions, npoints);
    ToStream(stream, headers, values);
}

void Plotting::MdFunctionsOnUnityToFile(const std::string &name, size_t dim,
                                        std::initializer_list<std::function<double(std::vector<double>)>> functions,
                                        int npoints, std::vector<std::string> headers)
{
    std::ofstream file = Plotting::GetFile(name);
    MdFunctionsOnUnityToStream(file, dim, functions, npoints, headers);
    file.close();
}

void Plotting::ToFile(const std::vector<std::string> &headers, const std::vector<std::vector<double>> &values,
                      const std::string &name)
{
    std::ofstream file = GetFile(name);
    ToStream(file, headers, values);
    file.close();
}
