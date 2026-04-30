#include <gtest/gtest.h>

#include "Iota.h"
#include "Single.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"
#include "UndirectedGraphSpectrum.h"

#include <iostream>

using namespace Graph;
using namespace Utilities;

namespace
{

} // namespace

TEST(UndirectedGraphSpectrumTest, Singleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const UndirectedGraphSpectrum spectrum(*graph);
    const auto eigenSolution = spectrum.getEigenSolution();
    ASSERT_EQ(eigenSolution.getAvailableEigenValues().size(), 1);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues().front(), 0.0, 1.0e-12);
}

TEST(UndirectedGraphSpectrumTest, Path2)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    const UndirectedGraphSpectrum spectrum(*graph);
    const auto eigenSolution = spectrum.getEigenSolution();
    ASSERT_EQ(eigenSolution.getAvailableEigenValues().size(), 2);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues().back(), 2.0, 1.0e-12);
}

TEST(UndirectedGraphSpectrumTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const UndirectedGraphSpectrum spectrum(*graph);
    const auto eigenSolution = spectrum.getEigenSolution();
    ASSERT_EQ(eigenSolution.getAvailableEigenValues().size(), 3);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[0], 0.0, 1.0e-12);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[1], 1.0, 1.0e-12);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[2], 3.0, 1.0e-12);
    std::cout << "Path3\n" << eigenSolution;
}

TEST(UndirectedGraphSpectrumTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    const UndirectedGraphSpectrum spectrum(*graph);
    const auto eigenSolution = spectrum.getEigenSolution();
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[0], 0.0, 1.0e-12);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[1], 3.0, 1.0e-12);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[2], 3.0, 1.0e-12);
}

TEST(UndirectedGraphSpectrumTest, Complete4)
{
    const auto graph = UndirectedGraphLibrary::Get_CompleteGraph(4);
    const UndirectedGraphSpectrum spectrum(*graph);
    const auto eigenSolution = spectrum.getEigenSolution();
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[0], 0.0, 1.0e-12);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[1], 4.0, 1.0e-12);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[2], 4.0, 1.0e-12);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[3], 4.0, 1.0e-12);
}

TEST(UndirectedGraphSpectrumTest, Claw)
{
    const auto graph = UndirectedGraphLibrary::Get_Claw();
    const UndirectedGraphSpectrum spectrum(*graph);
    const auto eigenSolution = spectrum.getEigenSolution();
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[0], 0.0, 1.0e-12);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[1], 1.0, 1.0e-12);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[2], 1.0, 1.0e-12);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[3], 4.0, 1.0e-12);
    std::cout << "Claw\n" << eigenSolution;
}

TEST(UndirectedGraphSpectrumTest, RemoveMe)
{
    auto eigenSolution = UndirectedGraphSpectrum(*UndirectedGraphLibrary::Get_Claw()).getEigenSolution();
    std::cout << "Claw\n" << eigenSolution << "\n\n";

    eigenSolution = UndirectedGraphSpectrum(*UndirectedGraphLibrary::Get_Diamond()).getEigenSolution();
    std::cout << "Diamond\n" << eigenSolution << "\n\n";

    eigenSolution = UndirectedGraphSpectrum(*UndirectedGraphLibrary::Get_Star({1, 2, 3})).getEigenSolution();
    std::cout << "Star123\n" << eigenSolution << "\n\n";

    eigenSolution = UndirectedGraphSpectrum(*UndirectedGraphLibrary::Get_Bull()).getEigenSolution();
    std::cout << "Bull\n" << eigenSolution << "\n\n";

    eigenSolution = UndirectedGraphSpectrum(*UndirectedGraphLibrary::Get_Butterfly()).getEigenSolution();
    std::cout << "Butterfly\n" << eigenSolution << "\n\n";

    eigenSolution =
        UndirectedGraphSpectrum(*UndirectedGraphFromG6::Create(UndirectedGraphFromG6::fish)).getEigenSolution();
    std::cout << "Fist\n" << eigenSolution << "\n\n";

    eigenSolution =
        UndirectedGraphSpectrum(*UndirectedGraphFromG6::Create(UndirectedGraphFromG6::house)).getEigenSolution();
    std::cout << "House\n" << eigenSolution << "\n\n";

    eigenSolution =
        UndirectedGraphSpectrum(*UndirectedGraphFromG6::Create(UndirectedGraphFromG6::X84)).getEigenSolution();
    std::cout << "X84\n" << eigenSolution << "\n\n";

    eigenSolution =
        UndirectedGraphSpectrum(*UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3)).getEigenSolution();
    std::cout << "Pan3\n" << eigenSolution << "\n\n";
}
