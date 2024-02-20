#include <windows.h>
#include <thread>
#include <iomanip>
#include <cmath>
#include <iostream>
bool thread_exit_code = false;
float distance_from_origin = 0;
float velocity = 0;
float accelleration = 1;
float seconds;
float second_two;
constexpr double sleep_duration_seconds = 0.001;
auto sleep_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(sleep_duration_seconds));
void PhysicsHandler() {
    while (!thread_exit_code) {
        auto start = std::chrono::high_resolution_clock::now();
        distance_from_origin = distance_from_origin + (velocity * second_two);
        int order_of_magnitude = std::log10(std::fabs(distance_from_origin));
        int precision = 6 - order_of_magnitude;
        if (precision < 0) {
            precision = 0;
        }
        std::cout << "\033[0;0H";
        std::cout << std::fixed << std::setprecision(precision);
        std::cout << "Distance from origin: " << distance_from_origin << std::endl;
        std::cout << std::fixed << std::setprecision(precision);
        std::cout << "Velocity: " << velocity << std::endl;
        std::cout << "Acceleration: " << accelleration << std::endl;
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = (std::chrono::duration<float>(end - start));
        second_two = duration.count();
    }
}
int main() {
    HANDLE consoleInput = GetStdHandle(STD_INPUT_HANDLE);
    if (consoleInput == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to get console input handle." << std::endl;
        return 1;
    }
    DWORD mode;
    if (!GetConsoleMode(consoleInput, &mode)) {
        std::cerr << "Failed to get console mode." << std::endl;
        return 1;
    }
    mode |= ENABLE_PROCESSED_INPUT;
    if (!SetConsoleMode(consoleInput, mode)) {
        std::cerr << "Failed to set console mode. Error code: " << GetLastError() << std::endl;
        return 1;
    }
    std::thread threadone(PhysicsHandler);
    INPUT_RECORD inputRecord;
    DWORD numEventsRead;
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        if (!ReadConsoleInput(consoleInput, &inputRecord, 1, &numEventsRead)) {
            std::cerr << "Failed to read console input." << std::endl;
            return 1;
        }
        if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {
            WORD keyCode = inputRecord.Event.KeyEvent.wVirtualKeyCode;
            if (keyCode == 'w' || keyCode == 'W') {
                velocity = velocity + (accelleration * seconds);
            }
            else if (keyCode == 's' || keyCode == 'S') {
                velocity = velocity - (accelleration * seconds);
            }
            else if (keyCode == 'q' || keyCode == 'Q') {
                accelleration = accelleration + 1;
            }
            else if (keyCode == 'e' || keyCode == 'E') {
                accelleration = accelleration - 1;
            }
            if (keyCode == VK_ESCAPE) {
                thread_exit_code = true;
                std::cout << "exiting" << std::endl;
                break;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = (std::chrono::duration<float>(end - start));
        seconds = duration.count();
    }
    threadone.join();
    system("pause");
    return 0;
}