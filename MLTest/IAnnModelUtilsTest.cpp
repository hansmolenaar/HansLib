#include <gtest/gtest.h>
#include <array>

#include "IAnnModelUtils.h"
#include "AnnLayerLinear.h"
#include "AnnLayerLogistic.h"
#include "AnnWeightsNoBias.h"
#include "AnnWeightsSingleBias.h"
#include "AnnWeightsWithBias.h"
#include "ParameterSet.h"
#include "AnnCostFunctionSE.h"
#include "AnnModel.h"
#include "Single.h"
#include "AnnDataSet.h"
#include "Functors.h"
#include "IRealFunctionUtils.h"
#include "IRealFunction.h"
#include "MyAssert.h"
#include "IMatrix.h"
#include "ISingleVariableRealValuedFunction.h"
#include "ISingleVariableRealValuedFunctionUtils.h"

TEST(IAnnModelUtilsTest, FeedForwardBasic)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightsSingleBias averageWeight(1, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &averageWeight };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, 0.0 });

   const ML::AnnModel model(layers, matrices, costFunction);

   const auto dims = ML::IAnnModelUtils::getLayerDimensions(model);
   ASSERT_EQ(dims.size(), 1);
   ASSERT_EQ(dims[0], 1);

   auto result = ML::IAnnModelUtils::feedForward(model, std::vector<double>{0.1}, parameterSet);
   auto output = result->getOutput();
   ASSERT_DOUBLE_EQ(Utilities::Single(output), 0.05);

   result = ML::IAnnModelUtils::feedForward(model, std::vector<double>{0.3}, parameterSet);
   output = result->getOutput();
   ASSERT_DOUBLE_EQ(Utilities::Single(output), 0.15);
}


TEST(IAnnModelUtilsTest, FeedForwardBasicError)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightsWithBias averageWeight(1, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &averageWeight };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, 0.0 });

   const ML::AnnModel model(layers, matrices, costFunction);

   ML::AnnDataSet dataSet(1, 1);
   dataSet.addSample({ 0.1 }, { 0.2 });
   dataSet.addSample({ 0.3 }, { 0.25 });
   dataSet.addSample({ 0.6 }, { 0.4 });
   dataSet.addSample({ 0.7 }, { 0.7 });

   ASSERT_DOUBLE_EQ(ML::IAnnModelUtils::getCost(model, dataSet, parameterSet), 0.0825);
}

TEST(IAnnModelUtilsTest, FeedForwardSlightlyMoreComplex)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightsSingleBias averageWeight(2, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &averageWeight };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, 0.1, 0.0 });

   const ML::AnnModel model(layers, matrices, costFunction);

   ML::AnnDataSet dataSet(2, 1);
   dataSet.addSample({ 0.1, 0.1 * 0.1 }, { 0.2 });
   dataSet.addSample({ 0.3, 0.3 * 0.3 }, { 0.25 });
   dataSet.addSample({ 0.6, 0.6 * 0.6 }, { 0.4 });
   dataSet.addSample({ 0.7, 0.7 * 0.7 }, { 0.7 });

   ASSERT_DOUBLE_EQ(ML::IAnnModelUtils::getCost(model, dataSet, parameterSet), 0.0625895);
}


TEST(IAnnModelUtilsTest, FeedForwardWithHiddenLayer)
{
   // See https://hmkcode.com/ai/backpropagation-step-by-step/
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear hiddenLayer(2);
   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &hiddenLayer, &outputLayer };

   const ML::AnnWeightsNoBias averageWeightHidden(2, 2);
   const ML::AnnWeightsNoBias averageWeightOutput(2, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &averageWeightHidden, &averageWeightOutput };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.11, 0.21, 0.12, 0.08 });
   parameterSet.add({ 0.14, 0.15 });

   const ML::AnnModel model(layers, matrices, costFunction);

   ML::AnnDataSet dataSet(2, 1);
   dataSet.addSample({ 2, 3 }, { 1 });

   const double expect = 0.5 * std::pow((1 - 0.191), 2);
   ASSERT_DOUBLE_EQ(ML::IAnnModelUtils::getCost(model, dataSet, parameterSet), expect);
}

TEST(IAnnModelUtilsTest, SetParameterDerivativesSuperSimple)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightsSingleBias averageWeight(1, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &averageWeight };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, 0.0 });

   const ML::AnnModel model(layers, matrices, costFunction);
   auto result = ML::IAnnModelUtils::feedForward(model, std::vector<double>{0.7}, parameterSet);
   auto derivs = ML::ParameterSet::CreateUsingDimensions(parameterSet);
   ML::IAnnModelUtils::setParameterDerivatives(model, *result, std::vector<double>{0.7}, parameterSet, derivs);
   ASSERT_DOUBLE_EQ(derivs.at(0)[0], -0.245);
   ASSERT_DOUBLE_EQ(derivs.at(0)[1], -0.35);
}


TEST(IAnnModelUtilsTest, SetParameterDerivativesSimple)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightsSingleBias averageWeight(2, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &averageWeight };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, 0.1, 0.0 });

   const ML::AnnModel model(layers, matrices, costFunction);
   auto forwardResult = ML::IAnnModelUtils::feedForward(model, std::vector<double>{0.7, 0.49}, parameterSet);
   auto derivs = ML::ParameterSet::CreateUsingDimensions(parameterSet);
   const std::vector<double> ideal{ 0.7 };
   ML::IAnnModelUtils::setParameterDerivatives(model, *forwardResult, ideal, parameterSet, derivs);
   ASSERT_DOUBLE_EQ(derivs.at(0)[0], -0.2107);
   ASSERT_DOUBLE_EQ(derivs.at(0)[1], -0.14749);
   ASSERT_DOUBLE_EQ(derivs.at(0)[2], -0.301);

   constexpr double learningRate = 1;
   ML::IAnnModelUtils::backPropagation(model, *forwardResult, ideal, learningRate, parameterSet);
   ASSERT_DOUBLE_EQ(parameterSet.at(0)[2], 0.301);
}



TEST(IAnnModelUtilsTest, GeeksExample)
{
   // See https://www.geeksforgeeks.org/backpropagation-in-neural-network/
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLogistic hiddenLayer(2);
   const ML::AnnLayerLogistic outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &hiddenLayer, &outputLayer };

   const ML::AnnWeightsNoBias weightHidden(2, 2);
   const ML::AnnWeightsNoBias weightOutput(2, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &weightHidden, &weightOutput };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.2, 0.2, 0.3, 0.3 });
   parameterSet.add({ 0.3, 0.9 });

   const ML::AnnModel model(layers, matrices, costFunction);
   auto forwardResult = ML::IAnnModelUtils::feedForward(model, std::vector<double>{0.35, 0.7}, parameterSet);
   ASSERT_DOUBLE_EQ(Utilities::Single(forwardResult->getOutput()), 0.66507363952475640);

   const std::vector<double> ideal{ 0.5 };

   ML::AnnArray neuronError(ML::IAnnModelUtils::getLayerDimensions(model));
   ML::IAnnModelUtils::setErrors(model, *forwardResult, ideal, parameterSet, neuronError);

   const Functors::AreClose areClose(1.0e-10); // Values in article?
   ASSERT_TRUE(areClose(neuronError.getValuesAt(0)[0], 0.0027275877458127758));
   ASSERT_TRUE(areClose(neuronError.getValuesAt(0)[1], 0.0080714273160845482));
   ASSERT_TRUE(areClose(neuronError.getValuesAt(1)[0], 0.036770267688329382));

   ML::ParameterSet parameterDerivs = ML::ParameterSet::CreateUsingDimensions(parameterSet);
   ML::IAnnModelUtils::setParameterDerivatives(model, *forwardResult, ideal, parameterSet, parameterDerivs);
   constexpr double expect = 0.00095465571103447143;
   ASSERT_TRUE(areClose(parameterDerivs.at(0)[0], expect));

   constexpr double learningRate = 1;
   ML::IAnnModelUtils::backPropagation(model, *forwardResult, ideal, learningRate, parameterSet);
   ASSERT_TRUE(areClose(parameterSet.at(0)[0], 0.2 - expect));
}


TEST(IAnnModelUtilsTest, NeuroticExample)
{
   // See https://theneuralblog.com/forward-pass-backpropagation-example/
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLogistic hiddenLayer(2);
   const ML::AnnLayerLogistic outputLayer(2);
   std::vector<const ML::IAnnLayer*> layers{ &hiddenLayer, &outputLayer };

   const ML::AnnWeightsSingleBias weightHidden(2, 2);
   const ML::AnnWeightsSingleBias weightOutput(2, 2);
   std::vector<const ML::IAnnWeights*> matrices{ &weightHidden, &weightOutput };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.1, 0.3, 0.2, 0.4, 0.25 });
   parameterSet.add({ 0.5, 0.6, 0.7, 0.8, 0.35 });

   ML::AnnDataSet dataSet(2, 2);
   dataSet.addSample({ 0.1, 0.5 }, { 0.05, 0.95 });

   const ML::AnnModel model(layers, matrices, costFunction);
   auto forwardResult = ML::IAnnModelUtils::feedForward(model, dataSet.getNthInput(0), parameterSet);

   const Functors::AreClose areClose(8.0e-3); // Rounded numbers in article
   ASSERT_TRUE(areClose(forwardResult->getOutput()[0], 0.73492));
   ASSERT_TRUE(areClose(forwardResult->getOutput()[1], 0.77955));

   const double error = ML::IAnnModelUtils::getCost(model, dataSet, parameterSet);
   ASSERT_TRUE(areClose(error, 0.24908));

   ML::ParameterSet parameterDerivs = ML::ParameterSet::CreateUsingDimensions(parameterSet);
   ML::IAnnModelUtils::setParameterDerivatives(model, *forwardResult, dataSet.getNthOutput(0), parameterSet, parameterDerivs);
   ASSERT_TRUE(areClose(parameterDerivs.at(1)[0], 0.08020));
   ASSERT_TRUE(areClose(parameterDerivs.at(1)[1], 0.08211));
   ASSERT_TRUE(areClose(parameterDerivs.at(1)[2], -0.01760));
   ASSERT_TRUE(areClose(parameterDerivs.at(1)[3], -0.01802));
   ASSERT_TRUE(areClose(parameterDerivs.at(0)[0], 0.00110));


   constexpr double learningRate = 0.6;
   ML::IAnnModelUtils::backPropagation(model, *forwardResult, dataSet.getNthOutput(0), learningRate, parameterSet);

   ASSERT_TRUE(areClose(parameterSet.at(1)[0], 0.45187));
   ASSERT_TRUE(areClose(parameterSet.at(1)[1], 0.55073));
   ASSERT_TRUE(areClose(parameterSet.at(1)[2], 0.71056));
   ASSERT_TRUE(areClose(parameterSet.at(1)[3], 0.81081));

   ASSERT_TRUE(areClose(parameterSet.at(0)[0], 0.09933));
   ASSERT_TRUE(areClose(parameterSet.at(0)[1], 0.29667));
   ASSERT_TRUE(areClose(parameterSet.at(0)[2], 0.19919));
   ASSERT_TRUE(areClose(parameterSet.at(0)[3], 0.39597));
}

TEST(IAnnModelUtilsTest, BuggedBiasedExample)
{
   // See https://medium.com/@karna.sujan52/back-propagation-algorithm-numerical-solved-f60c6986b643
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLogistic hiddenLayer(2);
   const ML::AnnLayerLogistic outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &hiddenLayer, &outputLayer };

   const ML::AnnWeightsWithBias weightHidden(2, 2);
   const ML::AnnWeightsWithBias weightOutput(2, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &weightHidden, &weightOutput };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, -0.3, 0.2, 0.5, 0.6, -0.4 });
   parameterSet.add({ 0.1, 0.3, 0.8 });

   ML::AnnDataSet dataSet(2, 1);
   dataSet.addSample({ 1, 1 }, { 0 });
   dataSet.addSample({ 0, 1 }, { 1 });

   const ML::AnnModel model(layers, matrices, costFunction);
   auto forwardResult = ML::IAnnModelUtils::feedForward(model, dataSet.getNthInput(0), parameterSet);

   const Functors::AreClose areClose(1.0e-3);
   ASSERT_TRUE(areClose(Utilities::Single(forwardResult->getOutput()), 0.7391));

   ML::AnnArray neuronError(ML::IAnnModelUtils::getLayerDimensions(model));
   ML::IAnnModelUtils::setErrors(model, *forwardResult, dataSet.getNthOutput(0), parameterSet, neuronError);

   ASSERT_TRUE(areClose(neuronError.getValuesAt(0)[1], 0.010451));
   ASSERT_TRUE(areClose(neuronError.getValuesAt(0)[0], 0.0030481));
   ASSERT_TRUE(areClose(neuronError.getValuesAt(1)[0], 0.1425));
}

namespace
{
   class NumericalCheck : public ISingleVariableRealValuedFunction
   {
   public:
      NumericalCheck(const ML::IAnnModel& model, const ML::IAnnDataSet& dataSet, size_t layer, size_t param, const ML::IParameterSet& parameterSet);
      double operator()(double parameterValue) const;
      double operator()() const;

      double Evaluate(double x)const override;
      double Derivative(double x)const override;
      bool IsNonConstant() const override { return false; }
      bool HasDerivative() const override { return true; }

   private:
      const ML::IAnnModel& m_model;
      const ML::IAnnDataSet& m_dataSet;
      const ML::IParameterSet& m_parameterSet;
      size_t m_layer;
      size_t m_param;
   };

   NumericalCheck::NumericalCheck(const ML::IAnnModel& model, const ML::IAnnDataSet& dataSet, size_t layer, size_t param, const ML::IParameterSet& parameterSet) :
      m_model(model), m_dataSet(dataSet), m_parameterSet(parameterSet), m_layer(layer), m_param(param)
   {
   }

   double NumericalCheck::operator()(double parameterValue) const
   {
      ML::ParameterSet parameterSet = ML::ParameterSet::CopyFrom(m_parameterSet);
      auto params = parameterSet.getModifiable(m_layer);
      params[m_param] = parameterValue;
      auto forwardResult = ML::IAnnModelUtils::feedForward(m_model, m_dataSet.getNthInput(0), parameterSet);
      return Utilities::Single(forwardResult->getOutput());
   }

   double NumericalCheck::operator()()const
   {
      auto forwardResult = ML::IAnnModelUtils::feedForward(m_model, m_dataSet.getNthInput(0), m_parameterSet);
      return Utilities::Single(forwardResult->getOutput());
   }

   double NumericalCheck::Evaluate(double x) const
   {
      return (*this)(x);
   }

   double NumericalCheck::Derivative(double x) const
   {
      ML::ParameterSet parameterSet = ML::ParameterSet::CopyFrom(m_parameterSet);
      auto params = parameterSet.getModifiable(m_layer);
      params[m_param] = x;

      auto forwardResult = ML::IAnnModelUtils::feedForward(m_model, m_dataSet.getNthInput(0), parameterSet);
      ML::ParameterSet parameterSetDerivs = ML::ParameterSet::CreateUsingDimensions(m_parameterSet);
      ML::IAnnModelUtils::setParameterDerivatives(m_model, *forwardResult, m_dataSet.getNthOutput(0), parameterSet, parameterSetDerivs);
      return parameterSetDerivs.at(m_layer)[m_param];
   }

   void DoCheck(const ML::IAnnModel& model, const ML::IParameterSet& parameterSet, std::span<const double> input, std::span<const double> output)
   {
      const ML::AnnDataSet dataSet(input, output);
      for (size_t layer = 0; layer < parameterSet.getNumLayers(); ++layer)
      {
         const auto parameterLayer = parameterSet.at(layer);
         for (size_t param = 0; param < parameterLayer.size(); ++param)
         {
            NumericalCheck nc(model, dataSet, layer, param, parameterSet);
            ISingleVariableRealValuedFunctionUtils::CheckDerivative(nc, parameterLayer[param], 0.01);
         }
      }
   }
}

TEST(IAnnModelUtilsTest, NumericalCheckSingleParameter)
{
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightsNoBias weightOutput(1, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &weightOutput };

   const ML::AnnModel model(layers, matrices, costFunction);

   ML::ParameterSet parameterSet;
   constexpr double singleParameter = 2.0;
   parameterSet.add({ singleParameter });

   const std::vector<double> input{ 1.0 };
   auto forwardResult = ML::IAnnModelUtils::feedForward(model, input, parameterSet);
   const ML::AnnDataSet dataSet(forwardResult->getInput(), forwardResult->getOutput());

   const Functors::AreClose areClose(1.0e-10);
   const double expect = dataSet.getNthOutput(0).front();
   ASSERT_TRUE(areClose(Utilities::Single(forwardResult->getOutput()), expect));

   constexpr size_t c_layer = 0;
   constexpr size_t c_param = 0;
   const NumericalCheck nc(model, dataSet, c_layer, c_param, parameterSet);
   ASSERT_TRUE(areClose(nc(), expect));
   ASSERT_FALSE(areClose(nc(singleParameter + 1), expect));
   ASSERT_TRUE(areClose(nc(singleParameter), expect));
   ASSERT_TRUE(areClose(nc.Evaluate(parameterSet.at(c_layer)[c_param]), expect));

   const ML::AnnDataSet anotherDataSet(std::vector<double>{2.0}, std::vector<double>{3.0});
   NumericalCheck nc2(model, anotherDataSet, c_layer, c_param, parameterSet);
   ISingleVariableRealValuedFunctionUtils::CheckDerivative(nc2, singleParameter, 0.01);
}

#if false
TEST(IAnnModelUtilsTest, NumericalCheckSingleBias)
{
   const ML::AnnCostFunctionSE costFunction;
   const ML::AnnLayerLogistic outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightsSingleBias weightOutput(1, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &weightOutput };

   const ML::AnnModel model(layers, matrices, costFunction);

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.1, 0.2 });

   DoCheck(model, parameterSet, std::vector<double>{ 0.9 });
}
#endif