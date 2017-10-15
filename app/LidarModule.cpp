/** 
 *  @file    LidarModule.cpp
 *  @author  Karan Vivek Bhargava
 *  @copyright MIT License
 *  
 *  @brief ENPM808X, Midsemester project
 *
 *  @section DESCRIPTION
 *  
 *  This program is controlling a robot's heading direction
 *  using sensor fusion.
 *
 */

#include <vector>
#include <string>
#include "LidarModule.hpp"

/**
 * @brief      Constructs the object.
 */
LidarModule::LidarModule() {
  std::string str = "left";  // Dummy data for the image_left.png
  if (str == "left") {
    std::vector<std::vector<float>> input;
    for (int i = 0; i < 6; i++) {
      std::vector<float> p;
      if (i == 0) {
        p = {2, 1.2, 2};
      } else if (i == 1) {
        p = {1, 1, 3};
      } else {
        p = {10000, 10000, 1};
      }
      input.push_back(p);
    }
    setInput(input);
  }
  diagnostic_ = true;
}

/**
 * @brief      Destroys the object.
 */
LidarModule::~LidarModule() {}

/**
 * @brief      Sets the input image.
 *
 * @param[in]  input  The input image
 */
void LidarModule::setInput(std::vector<std::vector<float>> input) {
  inputPts_ = input;  // Set the input points
}

/**
 * @brief      Gets the diagnostic.
 *
 * @return     The diagnostic truth value.
 */
bool LidarModule::getDiagnostic() {
  return diagnostic_;
}

/**
 * @brief      Project the points onto the ground
 */
void LidarModule::flatten() {
  for (auto i : inputPts_) {
    std::vector<float> p;
    for (int j = 0; j < 2; j++) {
      p.push_back(i[j]);
    }
    flattenedPts_.push_back(p);
  }

  // // For debugging purposes
  // for (auto i: flattenedPts_) {
  //   for (auto j: i) {
  //     std::cout << j << ", ";
  //   }
  //   std::cout << std::endl;
  // }
}

/**
 * @brief      Calculates the probabilities.
 *
 * @return     The probabilities for heading directions.
 */
std::vector<float> LidarModule::computeProbabilities() {
  flatten();
  // Calculate the eucledian distances
  std::vector<float> dist;
  for (auto i : flattenedPts_) {
    float d = sqrt(pow(i[0], 2) + pow(i[1], 2));
    if (d > 10000) {
      d = 10000;
    }
    dist.push_back(d);
  }

  // Calculate the probabilties
  std::vector<float> outputValues;
  float var = 10;
  float mean = 0;
  for (unsigned int i=0; i < flattenedPts_.size(); i++) {
    float p =  exp(-pow((dist[i]-mean), 2.0)/(2.0*var));  // Use a gaussian
    outputValues.push_back(p);
  }

  // // Display Probabilities
  // std::cout << "Probabilities: ";
  // for (int i = 0 ; i < flattenedPts_.size(); i++) {
  //   std::cout << outputValues[i] << ", ";
  // }
  // std::cout << std::endl;
  return outputValues;
}
