#include <windows.h>
#include <iostream>
#include <iomanip> // for std::hex, std::setfill, std::setw

using namespace std;
int main() {
    // Open pendrive - replace 'E' with your drive letter
    HANDLE hDevice = CreateFile(
        "\\\\.\\E:",                // Device path, use your pendrive letter
        GENERIC_READ,               // Open for reading
        FILE_SHARE_READ | FILE_SHARE_WRITE, // Share mode
        NULL,                      // No security attributes
        OPEN_EXISTING,             // Open existing device
        0,                         // Normal file attributes
        NULL                       // No template
    );

    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open device. Run as administrator. Error: " << GetLastError() << std::endl;
        return 1;
    }

    char buffer[512];     // Buffer to hold one block
    DWORD bytesRead;

    // Read first sector (512 bytes)
    BOOL success = ReadFile(hDevice, buffer, sizeof(buffer), &bytesRead, NULL);
    if (!success || bytesRead != sizeof(buffer)) {
        std::cerr << "Failed to read from device. Error: " << GetLastError() << std::endl;
        CloseHandle(hDevice);
        return 1;
    }
    cout<<bytesRead<<endl;
    std::cout << "Successfully read 512 bytes from pendrive" << std::endl;

    // Process your buffer here (e.g., parse superblock)



// Assuming buffer is char buffer[512];
for (int i = 0; i < 512; ++i) {
    // Print each byte as two hex digits with leading zeros
    std::cout << std::hex << std::setfill('0') << std::setw(2) 
              << (static_cast<unsigned int>(static_cast<unsigned char>(buffer[i]))) << " ";
    
    // Print a newline every 16 bytes for readability
    if ((i + 1) % 16 == 0)
        std::cout << std::endl;
}

// Reset to decimal output if needed later
std::cout << std::dec;

    CloseHandle(hDevice);
    return 0;
}
