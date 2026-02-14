#pragma once

class IMatrix;

namespace IMatrixTest
{
void TestConstInterface(const IMatrix &, bool usesAsserts = false);
void TestInterface(IMatrix &, bool usesAsserts = false);
}; // namespace IMatrixTest
