#include "helloTraingleApp.hpp"
#include <iostream>

int main() {
  HelloTraingleApplication app;

  try {
    app.run();
  } catch (const std::exception e) {

    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
