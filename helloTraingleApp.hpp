#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class HelloTraingleApplication {
public:
  void run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  GLFWwindow *window;
  VkInstance instance;

  void createInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Traingle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    uint32_t support = glfwVulkanSupported();
    if (support == GLFW_FALSE) {
      throw std::runtime_error("vulkan is not supported");
    }

    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    std::vector<const char *> requiredExtensions;
    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
      requiredExtensions.emplace_back(glfwExtensions[i]);
    }
    requiredExtensions.emplace_back(
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    createInfo.enabledExtensionCount = (uint32_t)(requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    uint32_t extensionCount = 0;
    VkResult result = vkEnumerateInstanceExtensionProperties(
        nullptr, &extensionCount, nullptr);
    if (result != VK_SUCCESS) {
      throw std::runtime_error("failed create instance");
    }

    std::vector<VkExtensionProperties> extensions(extensionCount);
    result = vkEnumerateInstanceExtensionProperties(
        nullptr, &extensionCount, extensions.data());

    std::cout << "avaiable extensions: \n";

    for (const auto &extension : extensions) {
      std::cout << '\t' << extension.extensionName << '\n';
    }

    // TODO: glfwGetRequiredInstanceExtensions are in included in supported
    // extension list

    result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
      throw std::runtime_error("failed create instance");
    }
  }

  void initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
  }

  void initVulkan() { createInstance(); }
  void mainLoop() {
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
    }
  }
  void cleanup() {
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
  }
};
